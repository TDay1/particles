#include <iostream>
#include <string>
#include "../headers/Simulation.h"
#include "../headers/Output.h"

int main() {  
    double SIM_TIME = 15.0f;
    double TIMESTEP_SIZE = 0.00005f;
    double currentTime = 0.0f;

    int OUTPUT_FRAME_RATE = 60;
    int sampleEvery = ( ( static_cast<int> (1/TIMESTEP_SIZE)) / OUTPUT_FRAME_RATE);
    
    // Setup sim
    Simulation *sim = new Simulation(500, TIMESTEP_SIZE);
    Output *logger = new Output();

    // run sim main
    int stepCounter = 0;

    // Sim loop
    while (currentTime < SIM_TIME) {
        
        // run simulation
        sim->step();

        if (stepCounter % sampleEvery == 0){
            logger->log_visualise(sim);
            logger->log_cg(sim);

        }

        if (stepCounter % (sampleEvery*OUTPUT_FRAME_RATE) == 0){
            std::cout << "Completed " << currentTime << "s" << std::endl;
        }

        currentTime += TIMESTEP_SIZE;
        stepCounter++;
    }

    // Sim complete
    std::cout << "Simulation complete" << std::endl;

    return 0;
}
