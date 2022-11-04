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
           
            double distanceSquared = pow(particleData->positionX[i]-particleData->positionX[j], 2) 
                    + pow(particleData->positionY[i]-particleData->positionY[j], 2);

            // Colliding if distance < 2 (radius=1), thus for squared distance:
            if (distanceSquared < 4) {
                // The following calculates the x and y normals of the velocities.

                // 1. is there a collision?
                // Calculate the Normal
                // Unforutnately AVX code is unreadable so this may be difficult to follow.
                
                // Load particle coordinates into a vector
                __m256d positionVector = _mm256_set_pd(particleData->positionY[i], particleData->positionY[j], particleData->positionX[i], particleData->positionX[j]);
                // fill a vector with the distances 
                double distance = sqrt(distanceSquared);
                __m256d distanceVector = _mm256_set1_pd(distance);
                // Divide positions by distance
                __m256d positionOverDistance = _mm256_div_pd(positionVector, distanceVector);
                // Rearrange positionOverDistance such that the differences between the I and J values may be taken
                __m256d shiftedPositionDistance = _mm256_permute4x64_pd(positionOverDistance, 0b10110001);
                __m256d deltaPositionOverDistance = _mm256_sub_pd(positionOverDistance, shiftedPositionDistance);
                double nx = deltaPositionOverDistance[0];
                double ny = deltaPositionOverDistance[2];

                // Calculate the tangent
                double tx = -ny;
                double ty = nx;

                // AVX quadruple dot product
                // in only two AVX mul_pd and one hadd_pd we can calculate 4 dot products!

                // load x and y components of each velocity into vector
                __m256d velocityVector = _mm256_set_pd(particleData->velocityX[i], particleData->velocityY[i], particleData->velocityX[j], particleData->velocityY[j]);
                // Load x and y components of the normals into vector
                __m256d normVector = _mm256_set_pd(nx, ny, nx, ny);
                // Multiply
                __m256d normalDots = _mm256_mul_pd(velocityVector, normVector);

                // Load x and y components of tangents into vector
                __m256d tanVector = _mm256_set_pd(tx, ty, tx, ty);
                // Multiply
                __m256d tangentDots = _mm256_mul_pd(velocityVector, tanVector);
                
                // Now, since the vectors being dotted are 2x1, we can use hadd for the summing step 
                __m256d dotProds = _mm256_hadd_pd(normalDots, tangentDots);
                
                double innerNormalI = dotProds[2];
                double innerNormalJ = dotProds[0];
                double innerTangentI = dotProds[3];
                double innerTangentJ = dotProds[1];


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
