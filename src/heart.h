#include "main.h"

#ifndef HEART_H
#define HEART_H


class Heart {
public:
    Heart() {}
    Heart(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // HEART_H
