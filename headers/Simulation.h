# pragma once
#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include "../headers/main.h"

// Holds displacement, velocity, acceleration data of all particles
ParticleData* setupSimulation(Simulation* simulation);
void stepSimulation(Simulation* simulation, ParticleData* particleData);
__global__
void stepVelocities(Simulation* simulation, ParticleData* particleData);
__global__
void stepPositions(Simulation* simulation, ParticleData* particleData);
__global__
void wallCollisions(Simulation* simulation, ParticleData* particleData);
void cleanupSimulation(Simulation* simulation, ParticleData* particleData);
