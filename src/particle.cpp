#include <iostream>

class Particle {        // The class
  public:              // Access specifier
    Particle(float initRadius,
            float positionX, float positionY,
            float velocityX, float velocityY,
            float accelerationX, float accelerationY) {

        radius = initRadius;

        acceleration[0] = accelerationX;
        acceleration[1] = accelerationY;

        velocity[0] = velocityX;
        velocity[1] = velocityY;

        position[0] = positionX;
        position[1] = positionY;

    }

    void step_acceleration(){
        // Nothing here yet
    }

    void step_velocity(float deltaT){
        // v = v + a deltaT
        velocity[0] = velocity[0] + acceleration[0] * deltaT;
        velocity[1] = velocity[1] + acceleration[1] * deltaT;
    }

    void step_position(float deltaT){
        // s = s + v deltaT
        position[0] = position[0] + velocity[0] * deltaT;
        position[1] = position[1] + velocity[1] * deltaT;
    }

    private:
        float radius;
        float acceleration[2];
        float velocity[2];
        float position[2];
};