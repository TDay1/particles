#include <iostream>
#include <string>
#include "../headers/Simulation.h"
#include "../headers/Output.h"

int main() {  

    // Below are the simulation variables:
    // How long the sim runs for
    double SIM_TIME = 5.0f;
    // Timestep size of the sim
    double TIMESTEP_SIZE = 0.00005f;
    // Logging samples per second
    int OUTPUT_FRAME_RATE = 30;
    // The acceleration of the tank along the x axis
    double ACCELERATION_X = 10.0f;
    // The acceleration of the tank along the y axis
    double ACCELERATION_Y = -10.0f;
    // How many particles to simulate
    int NUMBER_OF_PARTICLES = 500;


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
        if (stepCounter % sampleEvery == 0){
            logger->log_visualise(sim);
            logger->log_cg(sim);
        }

        // Every second print simulation time that has elapsed.
        if (stepCounter % (sampleEvery*OUTPUT_FRAME_RATE) == 0){
            std::cout << "Completed " << currentTime << "s" << std::endl;
        }

        // increment current time
        currentTime += TIMESTEP_SIZE;
        stepCounter++;
    }

    // Sim complete
    std::cout << "Simulation complete" << std::endl;

    return 0;
}
