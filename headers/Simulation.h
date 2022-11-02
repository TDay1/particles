# pragma once
#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include "../headers/Particle.h"

// Holds displacement, velocity, acceleration data of all particles
typedef struct ParticleData {
  double* positionX;
  double* positionY;
  double* velocityX;
  double* velocityY;
  double* accelerationX;
  double* accelerationY;
} ParticleData;

// Structure holds the simulation config data
typedef struct Simulation {
    double simTime;
    double timestepSize;
    int outputFrameRate;
    double accelerationX;
    double accelerationY;
    int numParticles;
    double radius;
} Simulation;

ParticleData* setupSimulation(Simulation* simulation);
void stepSimulation(Simulation* simulation, ParticleData* particleData);
__global__
void stepVelocities(Simulation* simulation, ParticleData* particleData);
__global__
void stepPositions(Simulation* simulation, ParticleData* particleData);
void particleCollisions(Simulation* simulation, ParticleData* particleData);
__global__
void wallCollisions(Simulation* simulation, ParticleData* particleData);
void cleanupSimulation(Simulation* simulation, ParticleData* particleData);
