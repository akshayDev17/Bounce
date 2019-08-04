#include "main.h"
#include "timer.h"
#include "ball.h"
#include "floor.h"
#include "water.h"
#include "plank.h"
#include "spikes.h"
#include "heart.h"

#include<stdlib.h>

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
 * Customizable functions *
 **************************/

Ball Player;

Ball enemies[12];

Heart he;


Plank brown_plank, green_plank;

Spikes spike;

Floor f;

Water w;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

int left_control, right_control, flight_control,
jump_control, in_pond, touch_pond, bounce_from_plank_control = 0;

float ball_y;


Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
	// clear the color and depth in the frame buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the loaded shader program
	// Don't change unless you know what you are doing
	glUseProgram (programID);

	// Eye - Location of camera. Don't change unless you are sure!!
	// glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
	// Target - Where is the camera looking at.  Don't change unless you are sure!!
	// glm::vec3 target (0, 0, 0);
	// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
	// glm::vec3 up (0, 1, 0);

	// Compute Camera matrix (view)
	// Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
	// Don't change unless you are sure!!
	Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	// Don't change unless you are sure!!
	glm::mat4 VP = Matrices.projection * Matrices.view;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	// Don't change unless you are sure!!
	glm::mat4 MVP;  // MVP = Projection * View * Model

	// Scene render

    for(int i = 0; i < 12; i++)
        enemies[i].draw(VP);
	f.draw(VP);
	w.draw(VP);
    Player.draw(VP);
    brown_plank.draw(VP);
    green_plank.draw(VP);
    spike.draw(VP);
    he.draw(VP);
}

void tick_input(GLFWwindow *window) {
	int left  = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);
	int jump = glfwGetKey(window, GLFW_KEY_SPACE);
	int down = glfwGetKey(window, GLFW_KEY_DOWN);
	if (left) {
		// Do something
        float x = Player.position.x;
		if(x - 0.07 < -3.69){
			left_control = 1;
		}
		if(!left_control){
            float y = Player.position.y;
            Player.set_position(x - 0.07, y);
			if(x - 0.07 > -3.69)
			{
				left_control = 0;
				right_control = 0;
			}
		}
	}
	if(right){
        float x = Player.position.x;
		if(x + 0.07 > 3.69){
			right_control = 1;
		}
		if(!right_control){
            float y = Player.position.y;
            Player.set_position(x + 0.07, y);
			if(x + 0.07 < 3.69)
			{
				left_control = 0;
				right_control = 0;
			}
		}
	}
	if(jump){
        float y = Player.position.y;
        if(Player.position.y + Player.speed_y > 4.3){
			flight_control = 1;
		}
		if(!flight_control){
			if(!jump_control){
                Player.speed_y = 0.25;
				flight_control = 0;
				jump_control = 1;
                Player.tick();
			}
		}
	}
}

void water_function(){
    float p_x, sq_y, p_y;
    float left_end = w.position.x - 1;
    float right_end = w.position.x + 1;
    float water_radius = 1 - Player.radius;
    if(Player.position.x > left_end &&
            Player.position.x < right_end &&
            Player.position.y - Player.radius <= -1.9 &&
            in_pond == 0){
		in_pond = 1;
        if(Player.position.x < left_end + Player.radius){
            Player.position.x = -1 - Player.radius;
            Player.position.y = -1.9;
			touch_pond = 1;
		}
        if(Player.position.x > right_end - Player.radius){
            Player.position.x = right_end - Player.radius;
            Player.position.y = -1.9;
			touch_pond = 1;
		}
	}
	if(in_pond && !touch_pond){
        Player.position.y -= 0.012;
        p_x = w.position.x - Player.position.x;
        sq_y = water_radius*water_radius - p_x*p_x;
        p_y = w.position.y - pow(sq_y, 0.5) ;
        if(Player.position.y <= p_y){
			touch_pond = 1;
            Player.position.y = p_y;
		}
	}
    if(in_pond && touch_pond &&
            Player.position.y - Player.radius > w.position.y - 1){
        if(Player.position.x < w.position.x)
            Player.position.x += 0.01;
		else
            Player.position.x -= 0.01;
	}

	if(touch_pond){
        p_x = w.position.x - Player.position.x;
        sq_y = water_radius*water_radius - p_x*p_x;
        Player.position.y = w.position.y - pow(sq_y, 0.5) ;
	}

	if(in_pond){
        if(Player.position.x <= left_end + Player.radius){
            Player.position.x = left_end;
            Player.position.y = -1.9 + Player.radius;
		}
        if(Player.position.x >= right_end - Player.radius){
            Player.position.x = right_end;
            Player.position.y = -1.9 + Player.radius;
		}
	}
    if(Player.position.x >= right_end ||
            Player.position.x <= left_end ||
            Player.position.y-Player.radius>-1.9){
		in_pond = 0;
		touch_pond = 0;
	}
}

void reset_enemy_position(int i){
	//if(enemies[i].position.x - 0.3 >= 4)

	float pos_y = rand() % 3700;
	float pos_x;
	pos_y = pos_y / 1000;
	if(i % 3 == 0){
		pos_x = rand() % 70;
		pos_x = pos_x / 100 - 5;
	}
	else if(i % 3 == 1){
		pos_x = rand() % 70;
		pos_x = pos_x / 100 - 6;
	}
	else if(i % 3 == 2){
		pos_x = rand() % 70;
		pos_x = pos_x / 100 - 7;
	}
	enemies[i].set_position(pos_x, pos_y);
}

void tick_elements() {

    water_function();

    brown_plank.tick_x();

    green_plank.tick_x();

    spike.tick_x();

    if(spike.position.x >= -2.5 || spike.position.x <= -3.5)
        spike.speed_x = -spike.speed_x;


    if(bounce_from_plank_control){
        Player.speed_x = 0;
        left_control = 0;
        right_control = 0;
    }
    else
        Player.tick_x();

    if(Player.speed_y < -0.3 || Player.speed_y > 0.4
            || Player.position.y <= -1.5)
        bounce_from_plank_control = 1;

    if(brown_plank.position.x > 4.3)
        brown_plank.set_position(brown_plank.position.x - 8.3, brown_plank.position.y);

    if(green_plank.position.x > 4.3)
        green_plank.set_position(green_plank.position.x - 8.3, green_plank.position.y);

    if(kill_enemy(Player.bounding_box(), brown_plank.bounding_box(), 0.55)){
        if(Player.position.x < brown_plank.position.x){
            if(Player.speed_x != 0)
                Player.speed_x += 0.05;
            else
                Player.speed_x = 0.05;
            bounce_from_plank_control = 0;
            left_control = 1;
            right_control = 1;
            Player.speed_y += 0.22;
        }
        else{
            if(Player.speed_y != 0)
                Player.speed_y += 0.22;
            else
                Player.speed_y = 0.25;
        }
        brown_plank.set_position(brown_plank.position.x - 8.3, brown_plank.position.y);
    }

    if(kill_enemy(Player.bounding_box(), green_plank.bounding_box(), 0.55)){
        if(Player.position.x < green_plank.position.x){
            if(Player.speed_x != 0)
                Player.speed_x += 0.05;
            else
                Player.speed_x = 0.05;
            bounce_from_plank_control = 0;
            left_control = 1;
            right_control = 1;
            Player.speed_y += 0.22;
        }
        else{
            if(Player.speed_y != 0)
                Player.speed_y += 0.22;
            else
                Player.speed_y = 0.25;
        }
        green_plank.set_position(green_plank.position.x - 8.3, green_plank.position.y);
    }

    for(int i = 0; i< 12; i++){
		enemies[i].tick_x();
        if(kill_enemy(Player.bounding_box(),
                    enemies[i].bounding_box(), Player.radius + enemies[i].radius - 0.1)
                && !(Player.speed_y > 0)){
			reset_enemy_position(i);
            Player.speed_y = 0.2;
            Player.tick();

        }
        if(enemies[i].position.x - 0.3 >= 4)
            reset_enemy_position(i);

    }

    if(jump_control)
        Player.tick();
    if(Player.position.y <= -1.6 && !in_pond){
        jump_control = 0;
        flight_control = 0;
        Player.set_position(Player.position.x, -1.6);
    }

    if(Player.position.y <= -2.6 && in_pond){
        jump_control = 0;
        flight_control = 0;
        Player.set_position(Player.position.x, -2.6);
    }

    /*if (detect_collision(Player.bounding_box(), ball2.bounding_box())) {
    }*/
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    //Initialize floor
    f = Floor(0, -2.9, COLOR_BROWN);

    //Initialize pond of water
    w = Water(-1, -1.9, COLOR_SKYBLUE);

    //Initiliaze the main character
    Player  = Ball(2, -1.6, COLOUR_WHITE, 0.3);

    he = Heart(0,0, COLOR_SKYBLUE);

    spike = Spikes(-3.5, -1.9, COLOR_RED_BROWN);

    //Initialize the enemy balls of normal behaviour
    for(int i = 0; i < 12; i++){
        float pos_y = rand() % 3700;
        float pos_x;
        pos_y = pos_y / 1000;
        if(i % 3 == 0){
            pos_x = rand() % (70+i);
            pos_x = pos_x / 100 - 5;
            enemies[i] = Ball(pos_x, pos_y, COLOR_RED, 0.35);
            enemies[i].speed_x = -0.03;
        }
        else if(i % 3 == 1){
            pos_x = rand() % (70+i);
            pos_x = pos_x / 100 - 6;
            enemies[i] = Ball(pos_x, pos_y, COLOR_GREEN, 0.25);
            enemies[i].speed_x = -0.025;
        }
        else if(i % 3 == 2){
            pos_x = rand() % (70+i);
            pos_x = pos_x / 100 - 7;
            enemies[i] = Ball(pos_x, pos_y, COLOR_BLUE, 0.2);
            enemies[i].speed_x = -0.02;
        }
    }

    //Initialise enemy balls having a plank
    brown_plank = Plank(-4.7, 0, COLOR_BROWN);

    green_plank = Plank(-7.7, 1.5, COLOR_NEW_GREEN);


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    glfwSetWindowTitle(window, "Ball Destroyer");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BLACK.r / 256.0, COLOR_BLACK.g / 256.0, COLOR_BLACK.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1020;
    int height = 1020;


    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

/*bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}*/

bool kill_enemy(bounding_box_t a, bounding_box_t b, float param) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           ((a.y - b.y) * 2 < (a.height + b.height) &&
            a.y - b.y > param);
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
