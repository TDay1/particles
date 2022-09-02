#include "../headers/Simulation.h"

Simulation::Simulation(int nParticles, float timestep) {
        particleCount = nParticles;
        deltaT = timestep;
        
        setup();
}

void Simulation::setup(){
    // Generate particles
    particles = std::vector<Particle*>();
    particles.reserve(particleCount);

    float positionMax = 100.0f;
    float positionMin = 0.0f;

    float radius = 1.0f;

    for (int i = 0; i < particleCount; i++) {
                
        float positionX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/90.0f))+5;
        float positionY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/90.0f)+5);

        particles.push_back(new Particle(radius, positionX, positionY, 0.0f, 0.0f,  -3.0f, -6.0f));

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

            float radius = particleI->radius;

            // Pythag
            float distance = sqrt(pow(particleI->position[0]-particleJ->position[0], 2) + pow(particleI->position[1]-particleJ->position[1], 2));

            if (distance < 2*radius) {
                // Calculate the Normal
                float nx = (particleJ->position[0] - particleI->position[0]) / distance;
                float ny = (particleJ->position[1] - particleI->position[1]) / distance;

                // Tangent
                float tx = -ny;
                float ty = nx;

                // dot product tangent
                float dpTan1 = particleI->velocity[0] * tx + particleI->velocity[1] * ty;
                float dpTan2 = particleJ->velocity[0] * tx + particleJ->velocity[1] * ty;

                // Dot prod norm
                float dpNorm1 = particleI->velocity[0] * nx + particleI->velocity[1] * ny;
                float dpNorm2 = particleJ->velocity[0] * nx + particleJ->velocity[1] * ny;


			    // Conservation of momentum in 1D
                // Let mass = 1
                // momentum 1 and two end up being equal to dpNorm1 and dpNorm2 because m1=m2

                // Update the velocities
                particleI->velocity[0] = (tx * dpTan1 + nx * dpNorm1);
                particleI->velocity[1] = (ty * dpTan1 + ny * dpNorm1);
                particleJ->velocity[0] = (tx * dpTan2 + nx * dpNorm2);
                particleJ->velocity[1]= (ty * dpTan2 + ny * dpNorm2);

                // Calculate how long ago collision occured
                float collisionTime = 0.0f; // zero being now
                for (float step = deltaT/10; step < deltaT; step += deltaT/10){
                    float x1 = particleI->position[0] - particleI->velocity[0] * step;
                    float y1 = particleI->position[1] - particleI->velocity[1] * step;

                    float x2 = particleJ->position[0] - particleJ->velocity[0] * step;
                    float y2 = particleJ->position[1] - particleJ->velocity[1] * step;

                    float stepDistance = sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
                    
                    if (stepDistance < 2*radius){
                        collisionTime = step - deltaT/10;
                    }
                }



                // Move so not overlapping
                float overlap = 0.5f * (distance - (2*radius));

                particleI->position[0] -= overlap * (particleI->position[0] - particleJ->position[0])/distance;
                particleI->position[1] -= overlap * (particleI->position[1] - particleJ->position[1])/distance;

                particleJ->position[0] += overlap * (particleI->position[0] - particleJ->position[0])/distance;
                particleJ->position[1] += overlap * (particleI->position[1] - particleJ->position[1])/distance;

                // Now move particle missed amount of time
                particleI->step_position(collisionTime);
                particleJ->step_position(collisionTime);



            }

        }

        
    }

    float radius = 1.0f;

    for (int i = 0; i < particleCount; i++) {
        // Every particle
        for (int dimension = 0; dimension < 2; dimension++){
            // Every dimension
            if (particles.at(i)->position[dimension]-radius < 0.0f){
                // However much less than 0 is the overlap
                float overlap = fabs(particles.at(i)->position[dimension]-radius);

                // Time overlapping = overlap/v
                float timeSinceCollision = overlap/particles.at(i)->velocity[dimension];

                // Reset particle position so not overlapping
                particles.at(i)->position[dimension] += overlap;

                // Flip axis velocity
                particles.at(i)->velocity[dimension] *= -1;

                // Give particle back its lost time
                particles.at(i)->step_position(timeSinceCollision);

            } else if (particles.at(i)->position[dimension]+radius > 100.0f){
                // However much less than 0 is the overlap
                float overlap = fabs(particles.at(i)->position[dimension]+radius - 100);

                // Time overlapping = overlap/v
                float timeSinceCollision = overlap/particles.at(i)->velocity[dimension];

                // Reset particle position so not overlapping
                particles.at(i)->position[dimension] -= overlap;

                // Flip axis velocity
                particles.at(i)->velocity[dimension] *= -1;

                // Give particle back its lost time
                particles.at(i)->step_position(timeSinceCollision);

            }           

        }
        
    }


}
