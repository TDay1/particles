# pragma once
#include <iostream>

class Particle {
  public:
    Particle(float initRadius,
            float positionX, float positionY,
            float velocityX, float velocityY,
            float accelerationX, float accelerationY);
   
    void step_acceleration();

    void step_velocity(float deltaT);

    void step_position(float deltaT);

    float radius;
    float acceleration[2];
    float velocity[2];
    float position[2];
};
