# pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "../headers/Simulation.h"

class Output {
  public:
    Output();
   
    void log_visualise(Simulation *sim);
    void log_cg(Simulation *sim);


    void run();

    std::string visOutputLocation;
    std::string cgOutputLocation;

};
