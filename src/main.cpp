#include <iostream>
#include "../headers/Simulation.h"

int main() {
    std::cout << "Hello World!" << std::endl;
    
    // Setup sim
    Simulation *sim = new Simulation(10);
    
    // run sim
    float SIM_TIME = 5.0f;
    float TIMESTEP_SIZE = 0.001f;
    float currentTime = 0.0f;

    while (currentTime < SIM_TIME) {
        
        currentTime += TIMESTEP_SIZE;
    }

    // output sim


    return 0;
}
