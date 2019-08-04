#include "water.h"
#include "main.h"

Water::Water(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat vertex_buffer_data[450] = {};
    double rad=M_PI/50;
    for(int i = 0; i < 50; i++){
        vertex_buffer_data[9*i] = -cos(i*rad);
        vertex_buffer_data[9*i + 1] = -sin(i*rad);
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = -cos((i+1)*rad);
        vertex_buffer_data[9*i + 4] = -sin((i+1)*rad);
        vertex_buffer_data[9*i + 5] = 0.0f;
        vertex_buffer_data[9*i + 6] = 0.0f;
        vertex_buffer_data[9*i + 7] = 0.0f;
        vertex_buffer_data[9*i + 8] = 0.0f;
    };

    this->object = create3DObject(GL_TRIANGLES, 150, vertex_buffer_data, color, GL_FILL);
}

void Water::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bounding_box_t Water::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 2, 1 };
    return bbox;
}
