#include <iostream>
#include <string>
#include "../headers/Simulation.h"
#include "../headers/Output.h"

int main() {    
    float SIM_TIME = 5.0f;
    float TIMESTEP_SIZE = 0.001f;
    float currentTime = 0.0f;
    
    // Setup sim
    Simulation *sim = new Simulation(10);
    Output *logger = new Output(std::string("test"));

    // run sim
    while (currentTime < SIM_TIME) {
        sim->step();
        logger->log(sim);
        currentTime += TIMESTEP_SIZE;
    }

    // Sim complete
    std::cout << "Simulation complete" << std::endl;
    
    return 0;
}
