#include "../headers/Simulation.h"

// Initialise the simulation and its variables
ParticleData* setupSimulation(Simulation* simulation) {
    // Allocate a simulation structure
    ParticleData* particleData = static_cast<ParticleData*>(malloc(sizeof(ParticleData)));

    // Allocate vectors that hold particle data
    particleData->positionX = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
    particleData->positionY = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
    particleData->velocityX = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
    particleData->velocityY = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
    particleData->accelerationX = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));
    particleData->accelerationY = static_cast<double*>(calloc(simulation->numParticles, sizeof(double)));

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
    stepVelocities(simulation, particleData);

    // resolve collisions
    collisions(simulation, particleData);

    // Step positions
    stepPositions(simulation, particleData);
}

void stepVelocities(Simulation* simulation, ParticleData* particleData) {
    // Can be vectorised
    for (int i = 0; i < simulation->numParticles; i++) {
        particleData->velocityX[i] = particleData->velocityX[i] + simulation->accelerationX * simulation->timestepSize;
        particleData->velocityY[i] = particleData->velocityY[i] + simulation->accelerationY * simulation->timestepSize;
    }
}

void stepPositions(Simulation* simulation, ParticleData* particleData) {
    // Can be vectorised
    for (int i = 0; i < simulation->numParticles; i++) {
        particleData->positionX[i] = particleData->positionX[i] + particleData->velocityY[i] * simulation->timestepSize;
        particleData->positionY[i] = particleData->positionY[i] + particleData->velocityY[i] * simulation->timestepSize;
    }
}

void collisions(Simulation* simulation, ParticleData* particleData) {

    // fluid (particle-particle) collisions    
    for (int i = 0; i < simulation->numParticles; i++) {
        for (int j = i+1; j < simulation->numParticles; j++) {
            double radius = simulation->radius;

            // Pythag
            double distance = sqrt(pow(particleData->positionX[i]-particleData->positionX[j], 2) + pow(particleData->positionY[i]-particleData->positionY[j], 2));

            // 1. is there a collision?
            if (distance < 2*radius) {
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

                // Conservation of momentum in 1D
                // Let mass = 1
                // momentum one and two end up being equal to innerNormal1 and innerNormal2 because m1=m2

                // Update the velocities
                particleData->velocityX[i] = (tx * innerTangentI + nx * innerNormalI);
                particleData->velocityY[i] = (ty * innerTangentI + ny * innerNormalI);
                particleData->velocityX[j] = (tx * innerTangentJ + nx * innerNormalJ);
                particleData->velocityY[j] = (ty * innerTangentJ + ny * innerNormalJ);


                // Move so not overlapping
                double overlap = 0.5f * (distance - (2.0f*radius));

                particleData->positionX[i] -= overlap * (particleData->positionX[i] - particleData->positionX[j])/distance;
                particleData->positionY[i] -= overlap * (particleData->positionY[i] - particleData->positionY[j])/distance;

                particleData->positionX[j] += overlap * (particleData->positionX[i] - particleData->positionX[j])/distance;
                particleData->positionY[j] += overlap * (particleData->positionY[i] - particleData->positionY[j])/distance;
            }
        }
    }



    // Evaluate wall conditions
    for (int i = 0; i < simulation->numParticles; i++) {

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
    free(simulation);

    // Free the particle data
    free(particleData->positionX);
    free(particleData->positionY);
    free(particleData->velocityX);
    free(particleData->velocityY);
    free(particleData->accelerationX);
    free(particleData->accelerationY);
    free(particleData);
}
