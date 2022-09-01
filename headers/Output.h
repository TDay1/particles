# pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "../headers/Simulation.h"

class Output {
  public:
    Output(std::string location);
   
    void log(Simulation *sim);

    void run();

    std::string outputLocation;
};
