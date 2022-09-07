#include "../headers/Simulation.h"

Simulation::Simulation(int nParticles, double timestep, double tankAccelerationX, double tankAccelerationY) {
        particleCount = nParticles;
        deltaT = timestep;
        setup(tankAccelerationX, tankAccelerationY);
}

void Simulation::setup(double particleAccelerationX, double particleAccelerationY){
    // Generate particles
    particles = std::vector<Particle*>();
    particles.reserve(particleCount);

    double radius = 1.0f;

    // Helpful for consistency across tests
    srand(1);

    for (int i = 0; i < particleCount; i++) {
                
        double positionX = (static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/90.0f))) + 5.0f;
        double positionY = (static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/90.0f))) + 5.0f;

        particles.push_back(new Particle(radius, positionX, positionY, 0.0f, 0.0f,  particleAccelerationX, particleAccelerationY));

    }

}

void Simulation::step(){
    // Step all the particle velocities
    for (int i = 0; i <particleCount; i++) {
        particles.at(i)->step_velocity(deltaT);
    }

    // Collision engine will overwrite some of the velocities and positions
    collisions();

    // Calculate particle position after step
    for (int i = 0; i <particleCount; i++) {
        particles.at(i)->step_position(deltaT);
    }
    
}


void Simulation::collisions(){

    // fluid (particle-particle) collisions    
    for (int i = 0; i < particleCount; i++) {
        for (int j = i+1; j < particleCount; j++) {
            // 1. is there a collision?
            Particle* particleI = particles.at(i);
            Particle* particleJ = particles.at(j);
            
            double radius = particles.at(i)->radius;

            // Pythag
            double distance = sqrt(pow(particleI->position[0]-particleJ->position[0], 2) + pow(particleI->position[1]-particleJ->position[1], 2));

            if (distance < 2*radius) {
                // Calculate the Normal
                double nx = (particleJ->position[0] - particleI->position[0]) / distance;
                double ny = (particleJ->position[1] - particleI->position[1]) / distance;

                // inner product the normal and velocity
                double innerNormalI = particleI->velocity[0] * nx + particleI->velocity[1] * ny;
                double innerNormalJ = particleJ->velocity[0] * nx + particleJ->velocity[1] * ny;


                // Calculate the tangent
                double tx = -ny;
                double ty = nx;

                // inner product the tangent and velocity
                double innerTangentI = particleI->velocity[0] * tx + particleI->velocity[1] * ty;
                double innerTangentJ = particleJ->velocity[0] * tx + particleJ->velocity[1] * ty;

			    // Conservation of momentum in 1D
                // Let mass = 1
                // momentum one and two end up being equal to innerNormal1 and innerNormal2 because m1=m2

                // Update the velocities
                particleI->velocity[0] = (tx * innerTangentI + nx * innerNormalI);
                particleI->velocity[1] = (ty * innerTangentI + ny * innerNormalI);
                particleJ->velocity[0] = (tx * innerTangentJ + nx * innerNormalJ);
                particleJ->velocity[1] = (ty * innerTangentJ + ny * innerNormalJ);


                // Move so not overlapping
                double overlap = 0.5f * (distance - (2.0f*radius));

                particleI->position[0] -= overlap * (particleI->position[0] - particleJ->position[0])/distance;
                particleI->position[1] -= overlap * (particleI->position[1] - particleJ->position[1])/distance;

                particleJ->position[0] += overlap * (particleI->position[0] - particleJ->position[0])/distance;
                particleJ->position[1] += overlap * (particleI->position[1] - particleJ->position[1])/distance;
            }
        }
    }


    
    // Every particle
    for (int i = 0; i < particleCount; i++) {
        // Every dimension

        for (int dimension = 0; dimension < 2; dimension++){
            double radius = particles.at(i)->radius;

            if (particles.at(i)->position[dimension]-radius < 0.0f){

                particles.at(i)->position[dimension] = 0.0f+radius;
                particles.at(i)->velocity[dimension] = 0.0f;
            }
            if (particles.at(i)->position[dimension]+radius > 100.0f){

                particles.at(i)->position[dimension] = 100.0f-radius;
                particles.at(i)->velocity[dimension] = 0.0f;

            }
        }
    }

}
