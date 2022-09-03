#include "../headers/Simulation.h"

Simulation::Simulation(int nParticles, double timestep) {
        particleCount = nParticles;
        deltaT = timestep;
        
        setup();
}

void Simulation::setup(){
    // Generate particles
    particles = std::vector<Particle*>();
    particles.reserve(particleCount);

    double positionMax = 100.0f;
    double positionMin = 0.0f;

    double radius = 1.0f;

    srand(1);

    for (int i = 0; i < particleCount; i++) {
                
        double positionX = (static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/90.0f))) + 5.0f;
        double positionY = (static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/90.0f))) + 5.0f;

        particles.push_back(new Particle(radius, positionX, positionY, 0.0f, 0.0f,  50.0f, -50.0f));

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

    double frictionCoef = 1;

    // fluid (particle-particle) collisions    
    for (int i = 0; i < particleCount; i++) {
        for (int j = i+1; j < particleCount; j++) {
            // 1. is there a collision?
            Particle* particleI = particles.at(i);
            Particle* particleJ = particles.at(j);

            double radius = particleI->radius;

            // Pythag
            double distance = sqrt(pow(particleI->position[0]-particleJ->position[0], 2) + pow(particleI->position[1]-particleJ->position[1], 2));

            if (distance < 2*radius) {
                // Calculate the Normal
                double nx = (particleJ->position[0] - particleI->position[0]) / distance;
                double ny = (particleJ->position[1] - particleI->position[1]) / distance;

                // Tangent
                double tx = -ny;
                double ty = nx;

                // dot product tangent
                double dpTan1 = particleI->velocity[0] * tx + particleI->velocity[1] * ty;
                double dpTan2 = particleJ->velocity[0] * tx + particleJ->velocity[1] * ty;

                // Dot prod norm
                double dpNorm1 = particleI->velocity[0] * nx + particleI->velocity[1] * ny;
                double dpNorm2 = particleJ->velocity[0] * nx + particleJ->velocity[1] * ny;


			    // Conservation of momentum in 1D
                // Let mass = 1
                // momentum 1 and two end up being equal to dpNorm1 and dpNorm2 because m1=m2

                // Update the velocities
                particleI->velocity[0] = (tx * dpTan1 + nx * dpNorm1) * frictionCoef;
                particleI->velocity[1] = (ty * dpTan1 + ny * dpNorm1) *frictionCoef;
                particleJ->velocity[0] = (tx * dpTan2 + nx * dpNorm2) *frictionCoef;
                particleJ->velocity[1]= (ty * dpTan2 + ny * dpNorm2) *frictionCoef;


                // Move so not overlapping
                double overlap = 0.5f * (distance - (2.0f*radius));

                particleI->position[0] -= overlap * (particleI->position[0] - particleJ->position[0])/distance;
                particleI->position[1] -= overlap * (particleI->position[1] - particleJ->position[1])/distance;

                particleJ->position[0] += overlap * (particleI->position[0] - particleJ->position[0])/distance;
                particleJ->position[1] += overlap * (particleI->position[1] - particleJ->position[1])/distance;
                


                // Now move particle missed amount of time
                //particleI->step_position(deltaT/2);
                //particleJ->step_position(deltaT/2);
            }
        }
    }


    double radius = 1.0f;
    
    for (int i = 0; i < particleCount; i++) {
        // Every particle
        for (int dimension = 0; dimension < 2; dimension++){
            // Every dimension
            if (particles.at(i)->position[dimension]-radius < 0.0f){

                particles.at(i)->position[dimension] = 1.0f;

                particles.at(i)->velocity[dimension] = 0.0f;

            }
            if (particles.at(i)->position[dimension]+radius > 100.0f){

                particles.at(i)->position[dimension] = 99.0f;

                // Flip axis velocity
                particles.at(i)->velocity[dimension] = 0.0f;

            }
        }
    }

 
    
    /*
    for (int i = 0; i < particleCount; i++) {
        // Every particle
        for (int dimension = 0; dimension < 2; dimension++){
            // Every dimension
            if (particles.at(i)->position[dimension]-radius < 0.0f){
                // However much less than 0 is the overlap
                double overlap = fabs(particles.at(i)->position[dimension]-radius);

                // Time overlapping = overlap/v
                double timeSinceCollision = overlap/particles.at(i)->velocity[dimension];

                // Reset particle position so not overlapping
                particles.at(i)->position[dimension] = 1.0f;

                // Flip axis velocity
                if (particles.at(i)->velocity[dimension] < 0.0f){
                    particles.at(i)->velocity[dimension] *= -0.5f;
                }

                // Give particle back its lost time
                if(!isinf(timeSinceCollision)) {
                    particles.at(i)->step_position(timeSinceCollision);
                }
            }
            if (particles.at(i)->position[dimension]+radius > 100.0f){
                // However much less than 0 is the overlap
                double overlap = fabs(particles.at(i)->position[dimension]+radius - 100.0f);

                // Time overlapping = overlap/v
                double timeSinceCollision = overlap/particles.at(i)->velocity[dimension];

                particles.at(i)->position[dimension] = 99.0f;

                // Flip axis velocity
                if (particles.at(i)->velocity[dimension] > 0.0f){
                    particles.at(i)->velocity[dimension] *= -0.5f;
                }
                // Give particle back its lost time
                if(!isinf(timeSinceCollision)) {
                    particles.at(i)->step_position(timeSinceCollision);
                }
            }
        }
    }
    */

}
