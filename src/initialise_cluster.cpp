#include "particle_container.h"
#include "particle_interactions.h"
#include "parameters.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
    // Check if given correct number of arguments
    if (!(argc - 1 == 5 || argc - 1 == 6))
    {
        printf("Requires 5 or 6 inpus but %i were given\n", argc - 1);
        return -1;
    }
    // Seed the random number generator (optional for better randomness)
    srand(0);

    std::array<int, 3> cubic_sizes; // Size of cubic sides
    double d = 2.91;                // Spacing (default or from argument)
    int steps;                      // Number of equilibrate steps
    double dt;                      // Time step size for equilibration

    // Get positions from arguments
    for (int i = 1; i <= 3; ++i)
    {
        char *arg = argv[i];
        cubic_sizes[i - 1] = std::stoi(arg); // Use stoi for safer conversion
    }

    // Check if spacing is given in the arguments
    if (argc - 1 == 6)
    {
        d = std::stod(argv[4]); // Use stod for safer conversion
    }

    // Assign number of equilibration steps and time step size
    steps = std::stoi(argv[argc - 2]);
    dt = std::stod(argv[argc - 1]);

    // Open files for writing outputs
    auto cwd = std::filesystem::current_path(); // current working directory
    std::filesystem::create_directories(cwd / "equilibration/positions");
    auto positions_dir = cwd / "equilibration/postions";
    std::ofstream pot_file(cwd / "equilibration/potential.pot"); // file collecting potential at each equilibration step

    // Create container for particles
    ParticleContainer container;
    size_t c = 0;

    // Initialise particle in grid
    for (int i = 0; i < cubic_sizes[0]; ++i)
    {
        for (int j = 0; j < cubic_sizes[1]; ++j)
        {
            for (int k = 0; k < cubic_sizes[2]; ++k)
            {
                Vector3d pos = {i * d, j * d, k * d};
                pos += d * randomUnitVector() * 0.5; // add randomness in postion by 50%
                Vector3d vel = Vector3d::Zero();
                container.addParticle("carbon_" + std::to_string(c), "C0", pos, vel);
                c++;
            }
        }
    }
    printf("Initialising %zu particles\n", c);

    // Equilibrate the particles via Lennard-Jones potential
    size_t n = container.size();
    for (int step = 0; step < steps; step++)
    {
        // Open position file to record all the positions in previous step
        std::ofstream pos_file(cwd / ("equilibration/positions/" + std::to_string(step) + ".pos"));
        // Lennard-Jones potential energy in previous step
        double total_potential_energy = 0;
        for (size_t i = 0; i < n; i++)
        {
            std::string type_i = container.getType(i);
            Vector3d r_i = container.getPosition(i);
            // Write position to postions file
            pos_file << r_i[0] << " " << r_i[1] << " " << r_i[2] << "\n";

            for (size_t j = i + 1; j < n; j++)
            {
                std::string type_j = container.getType(j);
                Vector3d r_j = container.getPosition(j);
                // Calculate the position adjustment according to Lennard-Jones potential
                auto move_pos = movePositionLJ(type_i, type_j, r_i, r_j, dt);
                container.addPositionChange(i, move_pos[0]);
                container.addPositionChange(j, move_pos[1]);
                // Calculate Lennard-Jones potential
                total_potential_energy += calculateLJPotential(type_i, type_j, r_i, r_j);
            }
        }
        // Apply the postion change to all particles
        container.applyPositionChangeAll();
        // Calculate mean potential (total potential per particle)
        double mean_potential_energy = total_potential_energy / n;
        // Write mean potential to potential file
        pot_file
            << mean_potential_energy << "\n";
        printf("Mean potential energy = %f\n", mean_potential_energy);
        // Close positions file
        pos_file.close();
    }
    pot_file.close();

    // Write final positions file
    std::ofstream pos_file(cwd / "equilibration/positions/final.pos");
    for (size_t i = 0; i < n; i++)
    {
        Vector3d r_i = container.getPosition(i);
        pos_file << r_i[0] << " " << r_i[1] << " " << r_i[2] << "\n";
    }
    pos_file.close();

    return 0;
}
