# pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "../headers/Simulation.h"

class Output {
  public:
    Output(Simulation* simulationData);
   
    void log_visualise(ParticleData *particleData);
    void log_cg(Simulation *sim);


    void run();

    std::string visOutputLocation;
    std::string cgOutputLocation;
    Simulation* simulation;
};
