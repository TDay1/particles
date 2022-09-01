#include "../headers/Simulation.h"

Simulation::Simulation(int nParticles, float timestep) {
        particleCount = nParticles;
        deltaT = timestep;
        
        setup();
}

void Simulation::setup(){
    // Generate particles
    particles = std::vector<Particle*>();
    particles.reserve(particleCount);

    float positionMax = 400.0f;
    float positionMin = 0.0f;

    for (int i = 0; i <particleCount; i++) {
        float positionX = (rand() / static_cast<float>(RAND_MAX)) * positionMax;
        float positionY = (rand() / static_cast<float>(RAND_MAX)) * positionMax;
        particles.push_back(new Particle(5, positionX, positionY, 0.0f, 0.0f, 0.0f, -9.8f));
    }

}

void Simulation::step(){
    // Step all the particle velocities
    for (int i = 0; i <particleCount; i++) {
        particles.at(i)->step_velocity(deltaT);
    }

    for (int i = 0; i <particleCount; i++) {
        particles.at(i)->step_position(deltaT);
    }

}
