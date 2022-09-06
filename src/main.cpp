#include <iostream>
#include <string>
#include <chrono>
#include "../headers/Simulation.h"
#include "../headers/Output.h"

int main() {  
    double SIM_TIME = 5.0f;
    double TIMESTEP_SIZE = 0.00005f;

    int OUTPUT_FRAME_RATE = 30;
    int sampleEvery = ( ( static_cast<int> (1/TIMESTEP_SIZE)) / OUTPUT_FRAME_RATE);

    for (double NUM_PARTICLES = 50; NUM_PARTICLES <= 1000; NUM_PARTICLES+=50){

        // Using time point and system_clock
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        for(int i = 0; i<5; i++){

            double currentTime = 0.0f;

            // Setup sim
            Simulation *sim = new Simulation(NUM_PARTICLES, TIMESTEP_SIZE);
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

                currentTime += TIMESTEP_SIZE;
                stepCounter++;
            }

            // Sim complete
            //std::cout << "Simulation complete" << std::endl;
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  
        std::chrono::seconds elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        std::cout << "Five iterations of " << NUM_PARTICLES << " particles processed in" << elapsed_seconds.count() << "s\n";

    }

    return 0;
}
