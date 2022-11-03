#include <immintrin.h>
#include <stdio.h>
#include "../headers/particleCollision.h"

void particleCollisions(Simulation* simulation, ParticleData* particleData) {

    //double particleCoordinates[4];
    //double particleVelocity[4];

    double radius = simulation->radius;
    // fluid (particle-particle) collisions    
    for (int i = 0; i < simulation->numParticles; i += 1) {
        for (int j = i+1; j < 500; j++) {
           
            double distance = sqrt(pow(particleData->positionX[i]-particleData->positionX[j], 2) 
                    + pow(particleData->positionY[i]-particleData->positionY[j], 2));
            if (distance < 2*radius) {
                
                // The following calculates the x and y normals of the velocities.

                // 1. is there a collision?
                // Calculate the Normal
                //double nx = (particleData->positionX[j] - particleData->positionX[i]) / distance;
                //double ny = (particleData->positionY[j] - particleData->positionY[i]) / distance;
                // Unforutnately AVX code is unreadable so this may be difficult to follow.
                
                // Load particle coordinates into a vector
                __m256d positionVector = _mm256_set_pd(particleData->positionY[i], particleData->positionY[j], particleData->positionX[i], particleData->positionX[j]);
                // fill a vector with the distances 
                __m256d distanceVector = _mm256_set1_pd(distance);
                // Divide positions by distance
                __m256d positionOverDistance = _mm256_div_pd(positionVector, distanceVector);
                // Rearrange positionOverDistance such that the differences between the I and J values may be taken
                __m256d shiftedPositionDistance = _mm256_permute4x64_pd(positionOverDistance, 0b10110001);
                __m256d deltaPositionOverDistance = _mm256_sub_pd(positionOverDistance, shiftedPositionDistance);
                double nx = deltaPositionOverDistance[0];
                double ny = deltaPositionOverDistance[2];

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
