# pragma once
#include <vector>
#include <random>
#include <iostream>
#include "../headers/Particle.h"

class Simulation {
  public:
    Simulation(int nParticles, float timestep);
    void setup();
    void step();

    // Variables
    std::vector<Particle*> particles;
    int particleCount;
    float deltaT;
};
