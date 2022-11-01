#include <iostream>
#include <string>
#include "../headers/Simulation.h"
#include "../headers/Output.h"

int main() {  

    // Allocate Simulation config
    Simulation* simulation = static_cast<Simulation*>(malloc(sizeof(Simulation)));

    // The sim is configured below:

    // How long the sim runs for (seconds)
    simulation->simTime = 5.0f;
    // Timestep size of the sim
    simulation->timestepSize = 0.00005f;
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
        currentTime += simulation->timestepSize;
        stepCounter++;
    }
    

    /*
    // Sample rate for logging
    int sampleEvery = ( ( static_cast<int> (1/TIMESTEP_SIZE)) / OUTPUT_FRAME_RATE);

    // Setup sim
    Simulation *sim = new Simulation(NUMBER_OF_PARTICLES, TIMESTEP_SIZE, ACCELERATION_X, ACCELERATION_Y);
    Output *logger = new Output();

    // run sim main
    int stepCounter = 0;

    // Sim loop
    double currentTime = 0.0f;
    while (currentTime < SIM_TIME) {
        
        // run simulation
        sim->step();

        // If this step in the sim is a sample step, sample it.
        //if (stepCounter % sampleEvery == 0){
        //    logger->log_visualise(sim);
        //    logger->log_cg(sim);
        //}

        // Every second print simulation time that has elapsed.
        if (stepCounter % (sampleEvery*OUTPUT_FRAME_RATE) == 0){
            std::cout << "Completed " << currentTime << "s" << std::endl;
        }

        // increment current time
        currentTime += TIMESTEP_SIZE;
        stepCounter++;
    }

    */
    // Sim complete
    std::cout << "Simulation complete" << std::endl;

    cleanupSimulation(simulation, particleData);
    return 0;
}
