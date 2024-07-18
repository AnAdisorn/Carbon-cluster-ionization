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

// Vector3cd vectorPotential(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3cd &r, const cdouble t)
// {
//     const cdouble kR_dot_n = r.dot(n);                        // r . n
//     const double kB_r_n_norm_sqr = pow(abs(b + kR_dot_n), 2); // |b + r . n| ^ 2
//     const Vector3cd kR_cross_n = r.cross(n);                  // r x n

//     const cdouble kA0 = c * e0 * pow(b, 2) / (w * kB_r_n_norm_sqr); // Magnitude of vector potential pulse

//     const Vector3cd kTemp_vec = w / c * r.dot(eps.cross(n)) * kR_cross_n / kB_r_n_norm_sqr; // Second term in vector of vector potential pulse
//     const Vector3cd kVec_real = eps + kTemp_vec * b;                                        // Vector of vector potential pulse
//     const Vector3cd kVec_imag = -kTemp_vec * kR_dot_n;                                      // Vector of vector potential pulse

//     const cdouble kTemp_phase = w * (t - kR_dot_n / c); // From exp(-i * kTemp_phase) of the first expotnential of vector potential pulse
//     const cdouble kPhase_real = cos(kTemp_phase);       // First expotnential of vector potential pulse
//     const cdouble kPhase_imag = -sin(kTemp_phase);      // First expotnential of vector potential pulse

//     const double kTemp_real = -w * pow(kR_cross_n.norm(), 2) / (2 * c * kB_r_n_norm_sqr); // From exp(kTemp_real * (b - i * r . n)) of the second expotnential of vector potential pulse
//     const double kReal_exp = exp(kTemp_real * b);
//     const cdouble kTemp_theta = kTemp_real * kR_dot_n;
//     const cdouble kEnvelope_real = kReal_exp * cos(kTemp_theta);  // Second expotnential of vector potential pulse
//     const cdouble kEnvelope_imag = -kReal_exp * sin(kTemp_theta); // Second expotnential of vector potential pulse

//     return kA0 * (kVec_real * kPhase_real * kEnvelope_real - kVec_real * kPhase_imag * kEnvelope_imag - kVec_imag * kPhase_real * kEnvelope_imag - kVec_imag * kPhase_imag * kEnvelope_real);
// }

// Vector3d electricField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t)
// {
//     const double h = 1e-200;
//     return (-vectorPotential(e0, w, b, n, eps, r, t + 1i * h)).imag() / h;
// }

// Vector3d magneticField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t)
// {
//     const double h = 1e-200;

//     const Vector3cd kA_dx = vectorPotential(e0, w, b, n, eps, r + Vector3cd{1i * h, 0, 0}, t); // A(r + i*h_x) / h
//     const Vector3cd kA_dy = vectorPotential(e0, w, b, n, eps, r + Vector3cd{0, 1i * h, 0}, t); // A(r + i*h_y) / h
//     const Vector3cd kA_dz = vectorPotential(e0, w, b, n, eps, r + Vector3cd{0, 0, 1i * h}, t); // A(r + i*h_z) / h

//     double x, y, z; // Magnitic field in respective axis
//     x = (kA_dy[2] - kA_dz[1]).imag() / h;
//     y = (kA_dz[0] - kA_dx[2]).imag() / h;
//     z = (kA_dx[1] - kA_dy[0]).imag() / h;

//     return {x, y, z};
// }

// double b = std::stod(argv[3]);  // characheristic width of the pulse
// Vector3d eps;
// double b_mag = magneticField(e0, w, b, n, eps, Vector3d::Zero(), t_start + step * dt).norm();
// double e_mag = electricField(e0, w, b, n, eps, Vector3d::Zero(), t_start + step * dt).norm();
// printf("magnitude (t = %f) = %f\n", t_start + step * dt, e_mag/b_mag);

Vector3d electricField(const double e0, const double w, const double period, const double phi, const double z, const double t)
{
    double mag = e0 * exp(-1.38 * pow(t / period, 2));
    double phase = w * (t - z / c);
    return {mag * cos(phase), mag * cos(phase + phi), 0};
}

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
    const double e0 = std::stod(argv[1]);     // Electric field amplitude
    const double w = std::stod(argv[2]);      // Laser carrier frequency
    const double period = std::stod(argv[3]); // Time period of pulse envelope
    const double dt = std::stod(argv[4]);     // Time step size
    const std::string pol = argv[5];          // Polarisation
    const std::string method = argv[6];       // Method for velocity update
    double phi;                               // Phase difference between x/y-axis
    const Vector3d n = {0, 0, 1};             // Direction of propagation (z-axis)
    if (pol.compare("linear") == 0)
    {
        // eps = {1, 0, 0};
        phi = 0;
    }
    else if (pol.compare("circular") == 0)
    {
        // eps = {1, 1, 0};
        phi = M_PI / 2;
    }
    else
    {
        printf("Palarisation must be either 'linear' or 'circular', but '%s' was given\n", pol.c_str());
        return -1;
    }

    // Create updateVelocity function pointer
    Vector3d (*updateVelocity)(const std::string, const Vector3d &, const Vector3d &, const Vector3d &, double);
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
        n_particle++;
    }
    pos_file.close();
    printf("Loaded %zu particles\n", n_particle);

    // Simulate ionisation by laser pulse
    size_t n_elctron = 0;
    int steps = 10 * period / dt;
    double t_start = -steps * dt / 2;
    printf("Number of steps = %i\n", steps);

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
        printf("step = %i, n_particle = %zu\n", step, n_particle);
        for (size_t i = 0; i < n_particle; i++)
        {
            std::string type = container.getType(i);
            Vector3d r = container.getPosition(i);
            Vector3d v = container.getVelocity(i);
            Vector3d e = electricField(e0, w, period, phi, r[2], t_start + step * dt);
            Vector3d b = n.cross(e) / c;
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
                printf("Found particle with unrecognised prefix: %s", type.c_str());
                return -1;
            }
            // Solving for new position and velocity
            r = updateHalfPosition(r, v, dt);
            v = updateVelocity(type, v, e, b, dt);
            r = updateHalfPosition(r, v, dt);
            // Update particles attribute
            container.setPosition(i, r);
            container.setVelocity(i, v);

            // Ionisation
            if (!(IonisationParametersMap.find(type) == IonisationParametersMap.end())) // ensure if polarizable according to ionisationParametersMap
            {
                if (randomChance(ionisationRate(type, e.norm()) * dt))
                {
                    // Add electron with same position and velocity
                    container.addParticle("electron_" + std::to_string(n_elctron), "e-", r, v);
                    n_elctron++;
                    // Change type of particle to the polarized one
                    container.setType(i, type_prefix + std::to_string(std::stoi(type.substr(1, 2)) + 1));
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
            printf("Found particle with unrecognised prefix: %s", type.c_str());
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