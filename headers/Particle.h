# pragma once
#include <iostream>

class Particle {
  public:
    Particle(double initRadius,
            double positionX, double positionY,
            double velocityX, double velocityY,
            double accelerationX, double accelerationY);
   
    void step_acceleration();

    void step_velocity(double deltaT);

    void step_position(double deltaT);

    double radius;
    double acceleration[2];
    double velocity[2];
    double position[2];
};
