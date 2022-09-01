#include "../headers/Output.h"

Output::Output(std::string location)
{
    outputLocation = location;
}

void Output::log(Simulation *sim)
{

    float **outputVector = new float *[sim->particleCount];
    for (int i = 0; i < sim->particleCount; i++)
    {
        outputVector[i] = new float[2];
    }

    for (int particleIndex = 0; particleIndex < sim->particleCount; particleIndex++)
    {
        Particle *particle = sim->particles.at(particleIndex);
        outputVector[particleIndex][0] = particle->position[0];
        outputVector[particleIndex][1] = particle->position[0];
    }

    // Print to debug
    /*for (int i = 0; i < sim->particleCount; i++)
    {
        std::cout << outputVector[i][0] << ", " << outputVector[i][1] << std::endl;
    }*/

    // Write data to text file
    std::fstream iofile;                                        // declare file pointer
    iofile.open("./" + outputLocation + ".txt", std::ios::out); // Open file

    // Save position array
    iofile << '[' << '\n';
    for (int i = 0; i < sim->particleCount; i++)
    {
        iofile << outputVector[i][0] << '\t' << outputVector[i][1] << '\n';
    }
    iofile << ']' << '\n';

    
    // Clean up
    iofile.close();
}

// taken  from plotting c++ to python
void Output::run()
{
    // Call Python to plot data
    std::string command = "python debug/PlotingScripts/Plot2D_text.py " + outputLocation;
    int commLen = command.length();
    char *commandchar = new char[commLen + 1];          // declaring character array
    commandchar = strcpy(commandchar, command.c_str()); // copying the contents of the string to char array
    system(commandchar);                                // Creates the directory incase it does not exist
    delete[] commandchar;
}

