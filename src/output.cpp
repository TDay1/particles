#include "../headers/Output.h"

Output::Output(Simulation *simulationData) {

  visOutputLocation = std::string("./out_visualise");
  cgOutputLocation = std::string("./out_cg");

  // Clean up old outfiles
  std::fstream iofile; // declare file pointer
  iofile.open(visOutputLocation + ".txt", std::ios::out); // Open file
  iofile.close();

  // Clean up old outfiles
  iofile.open(cgOutputLocation + ".txt", std::ios::out); // Open file
  iofile.close();

  simulation = simulationData;
}

// Logs the visualisation output to a file
void Output::log_visualise(ParticleData *particleData) {
  double **outputVector = new double *[simulation->numParticles];
  for (int i = 0; i < simulation->numParticles; i++) {
    outputVector[i] = new double[2];
  }

  for (int particleIndex = 0; particleIndex < simulation->numParticles;
       particleIndex++) {
    outputVector[particleIndex][0] = particleData->positionX[particleIndex];
    outputVector[particleIndex][1] = particleData->positionY[particleIndex];
  }

  // Write data to text file
  std::fstream iofile; // declare file pointer
  iofile.open(visOutputLocation + ".txt", std::ios::app); // Open file

  // Save flattened position array
  for (int i = 0; i < simulation->numParticles; i++) {
    iofile << outputVector[i][0] << '\t' << outputVector[i][1] << '\t';
  }

  // End array
  iofile << '\n';

  // Clean up
  iofile.close();
}

// Logs the centre of gravity over time to file
void Output::log_cg(ParticleData *particleData) {

  double xCentreGravity = 0.0f;
  double yCentreGravity = 0.0f;

  for (int i = 0; i < simulation->numParticles; i++) {
    xCentreGravity += particleData->positionX[i];
    yCentreGravity += particleData->positionY[i];
  }

  xCentreGravity /= simulation->numParticles;
  yCentreGravity /= simulation->numParticles;

  std::fstream iofile;                                   // declare file pointer
  iofile.open(cgOutputLocation + ".txt", std::ios::app); // Open file

  // Save cg
  iofile << xCentreGravity << '\t' << yCentreGravity << '\n';

  // Clean up
  iofile.close();
}
