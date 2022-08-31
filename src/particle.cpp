#include <iostream>
#include "../headers/Particle.h"

Particle::Particle(float initRadius,
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

void Particle::step_acceleration(){
    // Nothing here yet
}

void Particle::step_velocity(float deltaT){
    // v = v + a deltaT
    velocity[0] = velocity[0] + acceleration[0] * deltaT;
    velocity[1] = velocity[1] + acceleration[1] * deltaT;
}

void Particle::step_position(float deltaT){
    // s = s + v deltaT
    position[0] = position[0] + velocity[0] * deltaT;
    position[1] = position[1] + velocity[1] * deltaT;
}
