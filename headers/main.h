#pragma once

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


