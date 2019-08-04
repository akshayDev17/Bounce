#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color, float radius);
    glm::vec3 position;
    float rotation;
    float radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void tick_x();
    double speed_x;
    double speed_y;
    double acceleration;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
