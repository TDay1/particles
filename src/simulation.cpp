#include <iostream>
#include <vector>
#include "../headers/Simulation.h"


Simulation::Simulation(int nParticles) {
        particleCount = nParticles;     
        
        setup();
}

void Simulation::setup(){
    // Generate particles
    particles = std::vector<Particle*>();
    particles.reserve(particleCount);

    float positionMax = 400.0f;
    float positionMin = 0.0f;

    for (int i = 0; i <particleCount; i++) {
        float positionX = static_cast <float> (rand()) / (static_cast <float> (positionMax/positionMin));
        float positionY = static_cast <float> (rand()) / (static_cast <float> (positionMax/positionMin));
        particles.push_back(new Particle(5, positionX, positionY, 0.0f, 0.0f, 9.8f, 9.8f));
    }

}

void Simulation::step(){

}
