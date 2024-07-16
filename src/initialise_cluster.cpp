#include "particle_container.h"
#include "particle_interactions.h"
#include "parameters.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
    if (!(argc - 1 == 3 || argc - 1 == 4))
    {
        std::cout << "Requires 3 or 4 integers but " << argc - 1 << " were given."
                  << std::endl;
        return -1;
    }

    // Get positions from arguments
    std::array<int, 3> cubic_sizes;
    for (int i = 1; i <= 3; ++i)
    {
        char *arg = argv[i];
        cubic_sizes[i - 1] = std::stoi(arg); // Use stoi for safer conversion
    }

    // Spacing (default or from argument)
    double d = 2.91;
    if (argc - 1 == 4)
    {
        d = std::stod(argv[4]); // Use stod for safer conversion
    }

    // Create container for particles
    ParticleContainer container;
    int c = 0;

    for (int i = 0; i < cubic_sizes[0]; ++i)
    {
        for (int j = 0; j < cubic_sizes[1]; ++j)
        {
            for (int k = 0; k < cubic_sizes[2]; ++k)
            {
                Vector3d pos = {i * d, j * d, k * d};
                Vector3d vel = Vector3d::Zero();
                container.addParticle("carbon_" + std::to_string(c), "C0", pos, vel);
                c++;
            }
        }
    }

    return 0;
}
