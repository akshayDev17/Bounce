#include "plank.h"
#include "main.h"

Plank::Plank(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed_x = -0.035;
    GLfloat vertex_buffer_data[468] = {};
    double rad=M_PI/25;
    for(int i = 0; i < 50; i++){
        vertex_buffer_data[9*i] = cos(i*rad)*0.3;
        vertex_buffer_data[9*i + 1] = sin(i*rad)*0.3;
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = cos((i+1)*rad)*0.3;
        vertex_buffer_data[9*i + 4] = sin((i+1)*rad)*0.3;
        vertex_buffer_data[9*i + 5] = 0.0f;
        vertex_buffer_data[9*i + 6] = 0.0f;
        vertex_buffer_data[9*i + 7] = 0.0f;
        vertex_buffer_data[9*i + 8] = 0.0f;
    };

    vertex_buffer_data[450] = -0.9/pow(2, 0.5);
    vertex_buffer_data[451] = 0.3/pow(2, 0.5);
    vertex_buffer_data[452] = 0.0f;
    vertex_buffer_data[453] = -0.3*pow(2, 0.5);
    vertex_buffer_data[454] = 0.0f;
    vertex_buffer_data[455] = 0.0f;
    vertex_buffer_data[456] = -0.3/pow(2, 0.5);
    vertex_buffer_data[457] = 0.9/pow(2, 0.5);
    vertex_buffer_data[458] = 0.0f;
    vertex_buffer_data[459] = -0.3/pow(2, 0.5);
    vertex_buffer_data[460] = 0.9/pow(2, 0.5);
    vertex_buffer_data[461] = 0.0f;
    vertex_buffer_data[462] = -0.3*pow(2, 0.5);
    vertex_buffer_data[463] = 0.0f;
    vertex_buffer_data[464] = 0.0f;
    vertex_buffer_data[465] = 0.0f;
    vertex_buffer_data[466] = 0.3*pow(2, 0.5);
    vertex_buffer_data[467] = 0.0f;


    this->object = create3DObject(GL_TRIANGLES, 156, vertex_buffer_data, color, GL_FILL);
}

void Plank::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plank::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Plank::tick_x(){
    this->position.x -= this->speed_x;
}

bounding_box_t Plank::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 1, 1};
    return bbox;
}
