#include <iostream>
#include <string>
#include "../headers/Simulation.h"
#include "../headers/Output.h"

int main() {  
    float SIM_TIME = 5.0f;
    float TIMESTEP_SIZE = 0.01f;
    float currentTime = 0.0f;

    int OUTPUT_FRAME_RATE = 30;
    int sampleEvery = ( ( static_cast<int> (1/TIMESTEP_SIZE)) / OUTPUT_FRAME_RATE);
    
    // Setup sim
    Simulation *sim = new Simulation(100, TIMESTEP_SIZE);
    Output *logger = new Output(std::string("out"));

    // run sim main
    int stepCounter = 0;
    while (currentTime < SIM_TIME) {
        sim->step();
        if (stepCounter % sampleEvery == 0){
            logger->log(sim);
        }
        currentTime += TIMESTEP_SIZE;
        stepCounter++;
    }

    // Sim complete
    std::cout << "Simulation complete" << std::endl;

    return 0;
}
