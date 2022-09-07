#include "../headers/Output.h"

Output::Output()
{

    visOutputLocation = std::string("./out_visualise");
    cgOutputLocation = std::string("./out_cg");


    // Clean up old outfiles
    std::fstream iofile;                                        // declare file pointer
    iofile.open(visOutputLocation + ".txt", std::ios::out); // Open file
    iofile.close();

        // Clean up old outfiles
    iofile.open(cgOutputLocation + ".txt", std::ios::out); // Open file
    iofile.close();
    
}

void Output::log_visualise(Simulation *sim)
{
    double **outputVector = new double *[sim->particleCount];
    for (int i = 0; i < sim->particleCount; i++)
    {
        outputVector[i] = new double[2];
    }

    for (int particleIndex = 0; particleIndex < sim->particleCount; particleIndex++)
    {
        Particle *particle = sim->particles.at(particleIndex);
        outputVector[particleIndex][0] = particle->position[0];
        outputVector[particleIndex][1] = particle->position[1];
    }

    // Write data to text file
    std::fstream iofile;                                        // declare file pointer
    iofile.open(visOutputLocation + ".txt", std::ios::app); // Open file

    // Save flattened position array
    for (int i = 0; i < sim->particleCount; i++)
    {
        iofile << outputVector[i][0] << '\t' << outputVector[i][1] << '\t';
    }

    // End array
    iofile << '\n';
    
    // Clean up
    iofile.close();
}

// Logs the centre of gravity
void Output::log_cg(Simulation *sim) {
    double xCentreGravity = 0.0f;
    double yCentreGravity = 0.0f;

    for (int i = 0; i < sim->particleCount; i++) {
        xCentreGravity += sim->particles.at(i)->position[0];
        //std::cout << sim->particles.at(i)->position[0] << std::endl;

        yCentreGravity += sim->particles.at(i)->position[1];
    }

    //std::cout << xCentreGravity << std::endl;

    xCentreGravity /= sim->particleCount;
    yCentreGravity /= sim->particleCount;

    //std::cout << xCentreGravity << std::endl;

    // Write data to text file
    std::fstream iofile;                                        // declare file pointer
    iofile.open(cgOutputLocation + ".txt", std::ios::app); // Open file

    // Save cg
    iofile << xCentreGravity << '\t' << yCentreGravity << '\n';
    
    // Clean up
    iofile.close();
}


// taken  from plotting c++ to python
void Output::run()
{
    // Call Python to plot data
    /*
    std::string command = "python debug/PlotingScripts/Plot2D_text.py " + visOutputLocation;
    int commLen = command.length();
    char *commandchar = new char[commLen + 1];          // declaring character array
    commandchar = strcpy(commandchar, command.c_str()); // copying the contents of the string to char array
    system(commandchar);                                // Creates the directory incase it does not exist
    delete[] commandchar;
    */
}

