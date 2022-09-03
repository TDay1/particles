# pragma once
#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include "../headers/Particle.h"

class Simulation {
  public:
    Simulation(int nParticles, double timestep);
    void setup();
    void step();
    void collisions();

    // Variables
    std::vector<Particle*> particles;
    int particleCount;
    double deltaT;
};
