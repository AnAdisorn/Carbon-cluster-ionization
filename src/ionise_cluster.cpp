#include "particle_container.h"
#include "field_interactions.h"
#include "parameters.h"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std::complex_literals; // Allow to use i as complex number, i.e. 1i

namespace filesys = std::filesystem;
typedef std::complex<double> cdouble;

Vector3cd vectorPotential(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3cd &r, const cdouble t)
{
    const cdouble kR_dot_n = r.dot(n);        // r . n
    const cdouble kB_r_n = b + 1i * kR_dot_n; // b + i r . n
    const Vector3cd kR_cross_n = r.cross(n);  // r x n

    cdouble a0 = c * e0 * pow(b, 2) / (w * pow(abs(kB_r_n), 2));                       // Magnitude of vector potential pulse
    Vector3cd vec = eps + w / c * r.dot(eps.cross(n)) * kR_cross_n / kB_r_n;           // Vector term
    cdouble exp1 = exp(-1i * w * (t - kR_dot_n / c));                                  // First exponential term
    cdouble exp2 = exp(-w / (2 * c * (b + 1i * kR_dot_n)) * kR_cross_n.squaredNorm()); // Second exponential term

    return a0 * vec * exp1 * exp2;
}

Vector3d electricField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t)
{
    return -w * vectorPotential(e0, w, b, n, eps, r, t).imag();
}

Vector3d magneticField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t)
{
    const double h = 1e-200;

    const Vector3cd kA = vectorPotential(e0, w, b, n, eps, r, t);

    Vector3cd dA_dx = (vectorPotential(e0, w, b, n, eps, r + Vector3d{h, 0, 0}, t) - kA) / h; // dA/dx
    Vector3cd dA_dy = (vectorPotential(e0, w, b, n, eps, r + Vector3d{0, h, 0}, t) - kA) / h; // dA/dy
    Vector3cd dA_dz = (vectorPotential(e0, w, b, n, eps, r + Vector3d{0, 0, h}, t) - kA) / h; // dA/dz

    double x, y, z; // Magnitic field in respective axis
    x = (dA_dy[2] - dA_dz[1]).real();
    y = (dA_dz[0] - dA_dx[2]).real();
    z = (dA_dx[1] - dA_dy[0]).real();

    return {x, y, z};
}

// printf("magnitude (t = %f) = %f\n", t_start + step * dt, e_mag/b_mag);

inline bool randomChance(double prob)
{
    return (static_cast<double>(rand()) / RAND_MAX < prob);
}

int main(int argc, char *argv[])
{
    // Seed the random number generator (optional for better randomness)
    srand(0);

    // Check if given correct number of arguments
    if (argc - 1 != 6)
    {
        printf("Requires 6 inputs but %i were given\n", argc - 1);
        return -1;
    }

    // Process input arguments
    const double e0 = std::stod(argv[1]); // Electric field amplitude
    const double w = std::stod(argv[2]);  // Laser carrier frequency
    const double b = std::stod(argv[3]);  // Characheristic width of the pulse
    const double dt = std::stod(argv[4]); // Time step size
    const std::string pol = argv[5];      // Polarisation
    const std::string method = argv[6];   // Method for velocity update

    const Vector3d n = {0, 0, 1}; // Direction of propagation (z-axis)
    Vector3d eps;                 // polarization of laser field
    if (pol.compare("linear") == 0)
    {
        eps = {1, 0, 0};
    }
    else if (pol.compare("circular") == 0)
    {
        eps = {1, 1, 0};
    }
    else
    {
        printf("Palarisation must be either 'linear' or 'circular', but '%s' was given\n", pol.c_str());
        return -1;
    }

    // Create updateVelocity function pointer
    Vector3d (*updateVelocity)(const std::string type, const Vector3d &v, const Vector3d &e, const Vector3d &b, double t);
    if (method.compare("Boris") == 0)
    {
        updateVelocity = updateVelocityBoris;
    }
    else if (method.compare("Vay") == 0)
    {
        updateVelocity = updateVelocityVay;
    }
    else if (method.compare("HC") == 0)
    {
        updateVelocity = updateVelocityHC;
    }
    else
    {
        printf("Method must be either 'Boris', 'Vay' or 'HC', but '%s' was given\n", method.c_str());
        return -1;
    }

    // Configure directories for reading and writing
    auto cwd = filesys::current_path(); // current working directory
    auto final_positions_file = cwd / "equilibration/positions/final.pos";
    if (!(filesys::exists(final_positions_file)))
    {
        printf("Cannot find positions from initialisation: './equilibration/positions/final.pos'\n");
        return -1;
    }
    // Create directories for postions and velocity
    auto carbon_dir = cwd / ("ionisation/" + pol + "_" + method + "/carbon");
    auto electron_dir = cwd / ("ionisation/" + pol + "_" + method + "/electron");
    filesys::create_directories(carbon_dir);
    filesys::create_directory(electron_dir);

    // Open files for reading final positions from equilibration
    std::ifstream pos_file(final_positions_file); // file collecting positions of particles at final equilibration step

    // Create container for particles
    ParticleContainer container;

    // Load particle postions from equilibration
    size_t n_particle = 0; // number of particles
    Vector3d sum_pos = Vector3d::Zero();
    std::string line;
    while (getline(pos_file, line))
    {
        Vector3d pos; // position of a particle
        // Parse line to position
        size_t npos;
        for (int i = 0; i < 2; i++)
        {
            npos = line.find(" ");
            pos[i] = std::stod(line.substr(0, npos));
            line.erase(0, npos + 1);
        }
        pos[2] = std::stod(line);

        Vector3d vel = Vector3d::Zero(); // init with zero velocity
        container.addParticle("carbon_" + std::to_string(n_particle), "C0", pos, vel);
        sum_pos += pos;
        n_particle++;
    }
    pos_file.close();
    // Central cluster in the middle
    Vector3d mean_pos = sum_pos / n_particle;
    for (size_t i = 0; i < n_particle; i++)
    {
        container.addPosition(i, -mean_pos);
    }
    printf("Loaded %zu particles\n", n_particle);

    // Simulate ionisation by laser pulse
    size_t n_elctron = 0;
    int steps = 10 * (2 * M_PI / w) / dt;
    printf("Number of steps = %i\nDo you want to run simulation? (y/n): ", steps);
    char ans;
    std::cin >> ans;
    if (ans == 'n')
        return 0;

    printf("Start ionisation simulations\n");
    for (int step = 0; step < steps; step++)
    {
        // Open file to save positions and velocities
        std::ofstream carbon_pos_file(carbon_dir / (std::to_string(step) + ".pos"));
        std::ofstream carbon_vel_file(carbon_dir / (std::to_string(step) + ".vel"));
        std::ofstream carbon_ion_file(carbon_dir / (std::to_string(step) + ".ion"));
        std::ofstream electron_pos_file(electron_dir / (std::to_string(step) + ".pos"));
        std::ofstream electron_vel_file(electron_dir / (std::to_string(step) + ".vel"));
        // Get number of particles to loop through
        n_particle = container.size();
        std::cout << "\rstep = " << step << ", n_particle = " << n_particle << std::flush;

        // Half position update
        for (size_t i = 0; i < n_particle; i++)
        {
            std::string type = container.getType(i);
            std::string type_prefix = type.substr(0, 1);
            Vector3d r = container.getPosition(i);
            Vector3d v = container.getVelocity(i);
            // Write position and velocity to file
            if (type_prefix.compare("C") == 0)
            {
                carbon_pos_file << r[0] << " " << r[1] << " " << r[2] << "\n";
                carbon_vel_file << v[0] << " " << v[1] << " " << v[2] << "\n";
                carbon_ion_file << type.substr(1, 2) << "\n";
            }
            else if (type_prefix.compare("e") == 0)
            {
                electron_pos_file << r[0] << " " << r[1] << " " << r[2] << "\n";
                electron_vel_file << v[0] << " " << v[1] << " " << v[2] << "\n";
            }
            else
            {
                printf("Found particle with unrecognised prefix: %s", type.c_str());
                return -1;
            }
            r = updateHalfPosition(r, v, dt);
            container.setPosition(i, r);
        }

        // Calculate field
        for (size_t i = 0; i < n_particle; i++)
        {
            std::string type_i = container.getType(i);
            std::string type_prefix_i = type_i.substr(0, 1);
            Vector3d r_i = container.getPosition(i);
            Vector3d v_i = container.getVelocity(i);

            // Calculate E/B-field from laser pulse
            Vector3d e_field = electricField(e0, w, b, n, eps, r_i, step * dt);
            Vector3d b_field = magneticField(e0, w, b, n, eps, r_i, step * dt);
            container.addFields(i, e_field, b_field);

            // Calculate E/B-field between particles
            for (size_t j = i + 1; j < n_particle; j++)
            {
                std::string type_j = container.getType(j);
                std::string type_prefix_j = type_j.substr(0, 1);
                // if (type_prefix_i.compare(type_prefix_j) == 0) // check if the second particle is the same kind
                {
                    Vector3d r_j = container.getPosition(j);
                    Vector3d v_j = container.getVelocity(j);

                    auto fields = calculatePairFields(type_i, type_j, r_i, r_j, v_i, v_j);
                    container.addFields(i, fields[0], fields[2]);
                    container.addFields(j, fields[1], fields[3]);
                }
            }
        }
        // Velocity/Position/Ionisation updates
        for (size_t i = 0; i < n_particle; i++)
        {
            std::string type = container.getType(i);
            std::string type_prefix = type.substr(0, 1);
            Vector3d r = container.getPosition(i);
            Vector3d v = container.getVelocity(i);

            auto fields = container.getFields(i);
            // Calculate new position and velocity
            v = updateVelocity(type, v, fields[0], fields[1], dt);
            r = updateHalfPosition(r, v, dt);
            // Update particles attribute
            container.setPosition(i, r);
            container.setVelocity(i, v);

            // Ionisation
            if (!(IonisationParametersMap.find(type) == IonisationParametersMap.end())) // ensure if polarizable according to ionisationParametersMap
            {
                Vector3d e_field = electricField(e0, w, b, n, eps, r, step * dt);
                if (randomChance(1 - exp(-ionisationRate(type, e_field.norm()) * dt)))
                {
                    // Add electron with random position and velocity
                    container.addParticle("electron_" + std::to_string(n_elctron), "e-", r + bohrRadius(type) * randomUnitVector(), v);
                    n_elctron++;
                    // Change type of particle to the polarized one
                    container.setType(i, upperType(type));
                }
            }
        }

        carbon_pos_file.close();
        carbon_vel_file.close();
        carbon_ion_file.close();
        electron_pos_file.close();
        electron_vel_file.close();
    }

    // Writing final result
    // Open file to save positions and velocities
    std::ofstream carbon_pos_file(carbon_dir / "final.pos");
    std::ofstream carbon_vel_file(carbon_dir / "final.vel");
    std::ofstream carbon_ion_file(carbon_dir / "final.ion");
    std::ofstream electron_pos_file(electron_dir / "final.pos");
    std::ofstream electron_vel_file(electron_dir / "final.vel");
    // Get number of particles to loop through
    n_particle = container.size();
    for (size_t i = 0; i < n_particle; i++)
    {
        std::string type = container.getType(i);
        Vector3d r = container.getPosition(i);
        Vector3d v = container.getVelocity(i);
        // Get type substr
        std::string type_prefix = type.substr(0, 1);
        // Write position and velocity to file
        if (type_prefix.compare("C") == 0)
        {
            carbon_pos_file << r[0] << " " << r[1] << " " << r[2] << "\n";
            carbon_vel_file << v[0] << " " << v[1] << " " << v[2] << "\n";
            carbon_ion_file << type.substr(1, 2) << "\n";
        }
        else if (type_prefix.compare("e") == 0)
        {
            electron_pos_file << r[0] << " " << r[1] << " " << r[2] << "\n";
            electron_vel_file << v[0] << " " << v[1] << " " << v[2] << "\n";
        }
        else
        {
            printf("Found particle with unrecognisable prefix: %s", type.c_str());
            return -1;
        }
    }
    carbon_pos_file.close();
    carbon_vel_file.close();
    carbon_ion_file.close();
    electron_pos_file.close();
    electron_vel_file.close();

    return 0;
}