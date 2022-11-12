#include "../headers/Simulation.h"
#include "../headers/particleCollision.h"

// Initialise the simulation and its variables
ParticleData *setupSimulation(Simulation *simulation) {
  // Allocate a simulation structure
  ParticleData *particleData;

  // Allocate memory for the particle data struct
  cudaMallocManaged(&particleData, sizeof(ParticleData));

  // Allocate memory for the X positions of the particles
  cudaMallocManaged(&(particleData->positionX),
                    simulation->numParticles * sizeof(double));

  // Allocate memory for the Y positions of the particles
  cudaMallocManaged(&(particleData->positionY),
                    simulation->numParticles * sizeof(double));

  // Allocate memory for the X-axis velocities of particles
  cudaMallocManaged(&(particleData->velocityX),
                    simulation->numParticles * sizeof(double));

  // Allocate memory for the y-axis velocities of particles
  cudaMallocManaged(&(particleData->velocityY),
                    simulation->numParticles * sizeof(double));

  // Give each particle an initial X and Y location.
  srand(1);
  for (int i = 0; i < simulation->numParticles; i++) {
    particleData->positionX[i] = (static_cast<double>(rand()) /
                                  (static_cast<double>(RAND_MAX / 90.0f))) +
                                 5.0f;
    particleData->positionY[i] = (static_cast<double>(rand()) /
                                  (static_cast<double>(RAND_MAX / 90.0f))) +
                                 5.0f;
  }

  return particleData;
}

// Step the simulation one step
void stepSimulation(Simulation *simulation, ParticleData *particleData) {
  // Step Particle velocities

  // How many blocks to run kernel over:
  int blocks = (simulation->numParticles / 1024) + 1;
  // Evenly distribute the number of threads over the blocks for peak
  // performance
  int threads = (simulation->numParticles / blocks) + 1;

  // Handle particle-particle collisions
  particleCollisions(simulation, particleData);

  // Resolve particle-wall collisions
  wallCollisions<<<blocks, threads>>>(simulation, particleData);

  // Step velocities
  stepVelocities<<<blocks, threads>>>(simulation, particleData);

  // Step positions
  stepPositions<<<blocks, threads>>>(simulation, particleData);
}

// Steps the velocities of the particles one timestep
__global__ void stepVelocities(Simulation *simulation,
                               ParticleData *particleData) {
  int i = threadIdx.x + blockIdx.x * blockDim.x;

  if (i < simulation->numParticles) {
    particleData->velocityX[i] =
        particleData->velocityX[i] +
        simulation->accelerationX * simulation->timestepSize;
    particleData->velocityY[i] =
        particleData->velocityY[i] +
        simulation->accelerationY * simulation->timestepSize;
  }
}

// Steps the positions of the particles one timestep
__global__ void stepPositions(Simulation *simulation,
                              ParticleData *particleData) {
  int i = threadIdx.x + blockIdx.x * blockDim.x;
  if (i < simulation->numParticles) {
    particleData->positionX[i] =
        particleData->positionX[i] +
        particleData->velocityX[i] * simulation->timestepSize;
    particleData->positionY[i] =
        particleData->positionY[i] +
        particleData->velocityY[i] * simulation->timestepSize;
  }
}

// Resolves particle-wall collisions
__global__ void wallCollisions(Simulation *simulation,
                               ParticleData *particleData) {
  
  int i = threadIdx.x + blockIdx.x * blockDim.x;
  
  if (i < simulation->numParticles) {
    
    // Resolve any collisions with the horizontal walls (x axis)
    if (particleData->positionX[i] - simulation->radius < 0.0f) {

      particleData->positionX[i] = 0.0f + simulation->radius;
      particleData->velocityX[i] = 0.0f;
    }
    if (particleData->positionX[i] + simulation->radius > 100.0f) {

      particleData->positionX[i] = 100.0f - simulation->radius;
      particleData->velocityX[i] = 0.0f;
    }

    // Resolve any collisions with the vertical walls (y axis)
    if (particleData->positionY[i] - simulation->radius < 0.0f) {

      particleData->positionY[i] = 0.0f + simulation->radius;
      particleData->velocityY[i] = 0.0f;
    }
    if (particleData->positionY[i] + simulation->radius > 100.0f) {

      particleData->positionY[i] = 100.0f - simulation->radius;
      particleData->velocityY[i] = 0.0f;
    }
  }
}

// Frees (host and CUDA) memory used by the simulation.
void cleanupSimulation(Simulation *simulation, ParticleData *particleData) {
  // Free the simulation
  cudaFree(simulation);

  // Free the particle data
  cudaFree(particleData->positionX);
  cudaFree(particleData->positionY);
  cudaFree(particleData->velocityX);
  cudaFree(particleData->velocityY);
  cudaFree(particleData->accelerationX);
  cudaFree(particleData->accelerationY);
  cudaFree(particleData);
}
