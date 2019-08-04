#include "heart.h"
#include "main.h"

Heart::Heart(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat vertex_buffer_data[369];
    double rad=M_PI/20;
    for(int i = 0; i < 20; i++){
        vertex_buffer_data[9*i] = cos(i*rad)*0.1+0.1;
        vertex_buffer_data[9*i + 1] = sin(i*rad)*0.1;
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = cos((i+1)*rad)*0.1+0.1;
        vertex_buffer_data[9*i + 4] = sin((i+1)*rad)*0.1;
        vertex_buffer_data[9*i + 5] = 0.0f;
        vertex_buffer_data[9*i + 6] = 0.1f;
        vertex_buffer_data[9*i + 7] = 0.0f;
        vertex_buffer_data[9*i + 8] = 0.0f;
    }

    for(int i = 0; i < 20; i++){
        vertex_buffer_data[9*(i+20)] = cos(i*rad)*0.1-0.1;
        vertex_buffer_data[9*(i+20) + 1] = sin(i*rad)*0.1;
        vertex_buffer_data[9*(i+20) + 2] = 0.0f;
        vertex_buffer_data[9*(i+20) + 3] = cos((i+1)*rad)*0.1-0.1;
        vertex_buffer_data[9*(i+20) + 4] = sin((i+1)*rad)*0.1;
        vertex_buffer_data[9*(i+20) + 5] = 0.0f;
        vertex_buffer_data[9*(i+20) + 6] = 0.0f-0.1;
        vertex_buffer_data[9*(i+20) + 7] = 0.0f;
        vertex_buffer_data[9*(i+20) + 8] = 0.0f;
    }
    vertex_buffer_data[360] = -0.2f;
    vertex_buffer_data[361] = 0.0f;
    vertex_buffer_data[362] = 0.0f;
    vertex_buffer_data[363] = 0.2f;
    vertex_buffer_data[364] = 0.0f;
    vertex_buffer_data[365] = 0.0f;
    vertex_buffer_data[366] = 0.0f;
    vertex_buffer_data[367] = -0.2f;
    vertex_buffer_data[368] = 0.0f;

    this->object = create3DObject(GL_TRIANGLES, 123, vertex_buffer_data, color, GL_FILL);
}

void Heart::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bounding_box_t Heart::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.1 };
    return bbox;
}
