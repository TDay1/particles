#include "../headers/Simulation.h"
#include "../headers/particleCollision.h"

// Initialise the simulation and its variables
ParticleData* setupSimulation(Simulation* simulation) {
    // Allocate a simulation structure
    ParticleData* particleData;

    cudaMallocManaged( &particleData, sizeof(ParticleData));
    cudaMallocManaged(&(particleData->positionX), simulation->numParticles*sizeof(double));
    cudaMallocManaged(&(particleData->positionY), simulation->numParticles*sizeof(double));
    cudaMallocManaged(&(particleData->velocityX), simulation->numParticles*sizeof(double));
    cudaMallocManaged(&(particleData->velocityY), simulation->numParticles*sizeof(double));

    // Initialise the particles locations
    srand(1);
    for (int i = 0; i < simulation->numParticles; i++) {
        // Give each particle an initial random X and Y location.
        particleData->positionX[i] = (static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/90.0f))) + 5.0f;
        particleData->positionY[i] = (static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/90.0f))) + 5.0f;
    }

    return particleData;
}

// Step the simulation one step
void stepSimulation(Simulation* simulation, ParticleData* particleData) {
    // Step Particle velocities

    // How many blocks to run kernel over:
    int blocks = (simulation->numParticles/1024) + 1;
    // Evenly distribute the number of threads over the blocks for peak performance
    int threads = (simulation->numParticles/blocks) + 1;

    particleCollisions(simulation, particleData);

    // Resolve particle-wall collisions
    wallCollisions<<<blocks, threads>>>(simulation, particleData);

    stepVelocities<<<blocks, threads>>>(simulation, particleData);
    
    // Step positions
    stepPositions<<<blocks, threads>>>(simulation, particleData);
}

__global__
void stepVelocities(Simulation* simulation, ParticleData* particleData) {
    //int index = threadIdx.x + blockIdx.x * blockDim.x;
    //int stride = blockDim.x * gridDim.x;
    //for (int i = index; i < simulation->numParticles; i += stride) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;

    if (i < simulation->numParticles) { 
        particleData->velocityX[i] = particleData->velocityX[i] + simulation->accelerationX * simulation->timestepSize;
        particleData->velocityY[i] = particleData->velocityY[i] + simulation->accelerationY * simulation->timestepSize;
    }
}

__global__
void stepPositions(Simulation* simulation, ParticleData* particleData) {
    //int index = threadIdx.x + blockIdx.x * blockDim.x;
    //int stride = blockDim.x * gridDim.x;
    //for (int i = index; i < simulation->numParticles; i += stride) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    if (i < simulation->numParticles) { 
        particleData->positionX[i] = particleData->positionX[i] + particleData->velocityY[i] * simulation->timestepSize;
        particleData->positionY[i] = particleData->positionY[i] + particleData->velocityY[i] * simulation->timestepSize;
    }
}



__global__
void wallCollisions(Simulation* simulation, ParticleData* particleData) {
    //int index = threadIdx.x + blockIdx.x * blockDim.x;
    //int stride = blockDim.x * gridDim.x;

    // Evaluate wall conditions
    //for (int i = index; i < simulation->numParticles; i += stride) {

    int i = threadIdx.x + blockIdx.x * blockDim.x;
    if (i < simulation->numParticles) { 
        // x dimension (can be its own kernel)
        if (particleData->positionX[i] - simulation->radius < 0.0f){

            particleData->positionX[i] = 0.0f + simulation->radius;
            particleData->velocityX[i] = 0.0f;
        }
        if (particleData->positionX[i] + simulation->radius > 100.0f){

            particleData->positionX[i] = 100.0f - simulation->radius;
            particleData->velocityX[i] = 0.0f;

        }

        // y dimension (can be its own kernel)
        if (particleData->positionY[i] - simulation->radius < 0.0f){

            particleData->positionY[i] = 0.0f + simulation->radius;
            particleData->velocityY[i] = 0.0f;
        }
        if (particleData->positionY[i] + simulation->radius > 100.0f){

            particleData->positionY[i] = 100.0f - simulation->radius;
            particleData->velocityY[i] = 0.0f;

        }
    }

}

void cleanupSimulation(Simulation* simulation, ParticleData* particleData) {
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
