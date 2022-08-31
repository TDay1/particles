#include <vector>
#include "../headers/Particle.h"

class Simulation {
  public:
    Simulation(int nParticles);
    void setup();
    void step();

    private:
        std::vector<Particle*> particles;
        int particleCount;
};
