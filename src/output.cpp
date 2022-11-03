#include "../headers/Output.h"

Output::Output(Simulation* simulationData) {

    visOutputLocation = std::string("./out_visualise");
    cgOutputLocation = std::string("./out_cg");


    // Clean up old outfiles
    std::fstream iofile;                                        // declare file pointer
    iofile.open(visOutputLocation + ".txt", std::ios::out); // Open file
    iofile.close();

    // Clean up old outfiles
    iofile.open(cgOutputLocation + ".txt", std::ios::out); // Open file
    iofile.close();

    simulation = simulationData;
    
}

void Output::log_visualise(ParticleData *particleData) {
    double **outputVector = new double *[simulation->numParticles];
    for (int i = 0; i < simulation->numParticles; i++)
    {
        outputVector[i] = new double[2];
    }

    for (int particleIndex = 0; particleIndex < simulation->numParticles; particleIndex++)
    {
        outputVector[particleIndex][0] = particleData->positionX[particleIndex];
        outputVector[particleIndex][1] = particleData->positionY[particleIndex];
    }

    // Write data to text file
    std::fstream iofile;                                        // declare file pointer
    iofile.open(visOutputLocation + ".txt", std::ios::app); // Open file

    // Save flattened position array
    for (int i = 0; i < simulation->numParticles; i++)
    {
        iofile << outputVector[i][0] << '\t' << outputVector[i][1] << '\t';
    }

    // End array
    iofile << '\n';
    
    // Clean up
    iofile.close();
}

// Logs the centre of gravity
/*void Output::log_cg(Simulation *sim) {
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
*/
