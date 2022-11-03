#include <iostream>
#include <string>
#include <chrono>
#include "../headers/main.h"
#include "../headers/Simulation.h"
#include "../headers/Output.h"
#define TIMESTEP_SIZE 0.00005f

int main() {  

    // Allocate Simulation config
    Simulation* simulation;// = static_cast<Simulation*>(malloc(sizeof(Simulation)));
    cudaMallocManaged(&simulation, sizeof(Simulation));
    //cudaMemset(simulation, 0,sizeof(Simulation));

    // The sim is configured below:

    // How long the sim runs for (seconds)
    simulation->simTime = 5.0f;
    // Timestep size of the sim
    simulation->timestepSize = TIMESTEP_SIZE;
    // Logging samples per second
    simulation->outputFrameRate = 30;
    // The acceleration of the tank along the x axis
    simulation->accelerationX = 10.0f;
    // The acceleration of the tank along the y axis
    simulation->accelerationY = -10.0f;
    // How many particles to simulate
    simulation->numParticles = 500;
    // Size of each particle
    simulation->radius = 1.0f;

    // Setup simulation
    ParticleData* particleData = setupSimulation(simulation);

    // Setup simulation logger
    Output *logger = new Output(simulation);

    // Simulation tracking variables
    int stepCounter = 0;
    double currentTime = 0.0f;
    int sampleEvery = ( ( static_cast<int> (1/simulation->timestepSize)) / simulation->outputFrameRate);
   
    // Measure loop time
    auto StartTime = std::chrono::high_resolution_clock::now();

    while (currentTime < simulation->simTime) {
        // Step the simulation
        stepSimulation(simulation, particleData);

        // log
        if (stepCounter % sampleEvery == 0){
            logger->log_visualise(particleData);
            //logger->log_cg(sim);
        }

        // Print progress
        

        // increment simulation progress
        currentTime += TIMESTEP_SIZE;
        stepCounter++;
    }
    
    // Sim complete
    auto FinishTime = std::chrono::high_resolution_clock::now();
    auto TotalTime = std::chrono::duration_cast<std::chrono::microseconds>(FinishTime - StartTime);
    std::cout << "Main loop completed in: " << TotalTime.count() << std::endl;

    cleanupSimulation(simulation, particleData);
    return 0;
}
