#include "../headers/Simulation.h"

// Initialise the simulation and its variables
ParticleData* setupSimulation(Simulation* simulation) {
    // Allocate a simulation structure
    ParticleData* particleData;
    cudaMallocManaged( &particleData, sizeof(ParticleData));

    // Allocate vectors that hold particle data
    /*particleData->positionX = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
      particleData->positionY = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
      particleData->velocityX = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
      particleData->velocityY = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
      particleData->accelerationX = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
      particleData->accelerationY = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));*/
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

    stepVelocities<<<blocks, threads>>>(simulation, particleData);
    cudaDeviceSynchronize();

    particleCollisions(simulation, particleData, distances);

    // Resolve particle-wall collisions
    wallCollisions<<<blocks, threads>>>(simulation, particleData);
    cudaDeviceSynchronize();

    // Step positions
    stepPositions<<<blocks, threads>>>(simulation, particleData);
    cudaDeviceSynchronize();
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

void particleCollisions(Simulation* simulation, ParticleData* particleData, double* distances) {
    //int index = threadIdx.x + blockIdx.x * blockDim.x;
    //int stride = blockDim.x * gridDim.x;
    
    // fluid (particle-particle) collisions    
    for (int i = 0; i < simulation->numParticles; i += 1) {
        for (int j = i+1; j < simulation->numParticles; j++) {
            double radius = simulation->radius;

            // Pythag
            //double distance = sqrt(pow(particleData->positionX[i]-particleData->positionX[j], 2) 
            //+ pow(particleData->positionY[i]-particleData->positionY[j], 2));

            double distance = distances[i * simulation->numParticles + j];

            // 1. is there a collision?
            // Calculate the Normal
            double nx = (particleData->positionX[j] - particleData->positionX[i]) / distance;
            double ny = (particleData->positionY[j] - particleData->positionY[i]) / distance;

            // inner product the normal and velocity
            double innerNormalI = particleData->velocityX[i] * nx + particleData->velocityY[i] * ny;
            double innerNormalJ = particleData->velocityX[j] * nx + particleData->velocityY[j] * ny;


            // Calculate the tangent
            double tx = -ny;
            double ty = nx;

            // inner product the tangent and velocity
            double innerTangentI = particleData->velocityX[i] * tx + particleData->velocityY[i] * ty;
            double innerTangentJ = particleData->velocityX[j] * tx + particleData->velocityY[j] * ty;

            double overlap = 0.5f * (distance - (2.0f*radius));

            double newPositionXI = particleData->positionX[i] - overlap * (particleData->positionX[i] - particleData->positionX[j])/distance;
            double newPositionYI = particleData->positionY[i] - overlap * (particleData->positionY[i] - particleData->positionY[j])/distance;
            double newPositionXJ = particleData->positionX[j] + overlap * (particleData->positionX[i] - particleData->positionX[j])/distance;
            double newPositionYJ = particleData->positionY[j] + overlap * (particleData->positionY[i] - particleData->positionY[j])/distance;

            // Update the velocities
            if (distance < 2*radius) {
                // 1D conservation of momentum
                particleData->velocityX[i] = (tx * innerTangentI + nx * innerNormalI);
                particleData->velocityY[i] = (ty * innerTangentI + ny * innerNormalI);
                particleData->velocityX[j] = (tx * innerTangentJ + nx * innerNormalJ);
                particleData->velocityY[j] = (ty * innerTangentJ + ny * innerNormalJ);


                // Move so not overlapping
                particleData->positionX[i] = newPositionXI;
                particleData->positionY[i] = newPositionYI;

                particleData->positionX[j] = newPositionXJ;
                particleData->positionY[j] = newPositionYJ;
            }
        }
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
