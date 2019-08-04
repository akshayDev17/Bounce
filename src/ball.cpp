#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color, float radius) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;
    speed_x, speed_y = 0;
    acceleration = 0.015;
    GLfloat vertex_buffer_data[450] = {};
    double rad=M_PI/25;
    for(int i = 0; i < 50; i++){
        vertex_buffer_data[9*i] = cos(i*rad)*radius;
        vertex_buffer_data[9*i + 1] = sin(i*rad)*radius;
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = cos((i+1)*rad)*radius;
        vertex_buffer_data[9*i + 4] = sin((i+1)*rad)*radius;
        vertex_buffer_data[9*i + 5] = 0.0f;
        vertex_buffer_data[9*i + 6] = 0.0f;
        vertex_buffer_data[9*i + 7] = 0.0f;
        vertex_buffer_data[9*i + 8] = 0.0f;
    };

    this->object = create3DObject(GL_TRIANGLES, 150, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.y += this->speed_y;
    this->speed_y -= this->acceleration;
}

void Ball::tick_x(){
    this->position.x -= this->speed_x;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 2*radius, 2*radius };
    return bbox;
}
