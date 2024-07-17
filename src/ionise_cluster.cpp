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
    const cdouble kR_dot_n = r.dot(n);                        // r . n
    const double kB_r_n_norm_sqr = pow(abs(b + kR_dot_n), 2); // |b + r . n| ^ 2
    const Vector3cd kR_cross_n = r.cross(n);                  // r x n

    const cdouble kA0 = c * e0 * pow(b, 2) / (w * kB_r_n_norm_sqr); // Magnitude of vector potential pulse

    const Vector3cd kTemp_vec = w / c * r.dot(eps.cross(n)) * kR_cross_n / kB_r_n_norm_sqr; // Second term in vector of vector potential pulse
    const Vector3cd kVec_real = eps + kTemp_vec * b;                                        // Vector of vector potential pulse
    const Vector3cd kVec_imag = -kTemp_vec * kR_dot_n;                                      // Vector of vector potential pulse

    const cdouble kTemp_theta_1 = w * (t - kR_dot_n / c); // From exp(-i * kTemp_theta_1) of the first expotnential of vector potential pulse
    const cdouble kExp1_real = cos(kTemp_theta_1);        // First expotnential of vector potential pulse
    const cdouble kExp1_imag = -sin(kTemp_theta_1);       // First expotnential of vector potential pulse

    const double kTemp_real = -w * pow(kR_cross_n.norm(), 2) / (2 * c * kB_r_n_norm_sqr); // From exp(kTemp_real * (b - i * r . n)) of the second expotnential of vector potential pulse
    const double kReal_exp = exp(kTemp_real * b);
    const cdouble temp_theta_2 = kTemp_real * kR_dot_n;
    const cdouble kExp2_real = kReal_exp * cos(temp_theta_2);  // Second expotnential of vector potential pulse
    const cdouble kExp2_imag = -kReal_exp * sin(temp_theta_2); // Second expotnential of vector potential pulse

    return kA0 * (kVec_real * kExp1_real * kExp2_real - kVec_real * kExp1_imag * kExp2_imag - kVec_imag * kExp1_real * kExp2_imag - kVec_imag * kExp1_imag * kExp2_real);
}

Vector3d electricField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t)
{
    const cdouble h = 1e-200;
    return (-vectorPotential(e0, w, b, n, eps, r, t + 1i * h) / h).imag();
}

Vector3d magneticField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t)
{
    const cdouble h = 1e-200;

    const Vector3cd kA_dx = vectorPotential(e0, w, b, n, eps, r + Vector3cd{1i * h, 0, 0}, t) / h; // A(r + i*h_x) / h
    const Vector3cd kA_dy = vectorPotential(e0, w, b, n, eps, r + Vector3cd{0, 1i * h, 0}, t) / h; // A(r + i*h_y) / h
    const Vector3cd kA_dz = vectorPotential(e0, w, b, n, eps, r + Vector3cd{0, 0, 1i * h}, t) / h; // A(r + i*h_z) / h

    double x, y, z; // Magnitic field in respective axis
    x = (kA_dy[2] - kA_dz[1]).imag();
    y = (kA_dz[0] - kA_dx[2]).imag();
    z = (kA_dx[1] - kA_dy[0]).imag();

    return {x, y, z};
}

int main(int argc, char *argv[])
{
    // Check if given correct number of arguments
    if (argc - 1 != 4)
    {
        printf("Requires 4 inputs but %i were given\n", argc - 1);
        return -1;
    }

    // Process input arguments
    double e0 = std::stod(argv[1]); // Electric field amplitude
    double w = std::stod(argv[2]);  // Laser carrier frequency
    double b = std::stod(argv[3]);  // characheristic width of the pulse
    const Vector3d n = {0, 0, 1};   // Direction of propagation (z-axis)
    Vector3d eps;
    if (static_cast<std::string>(argv[4]).compare("linear") == 0)
    {
        eps = {1, 0, 0};
    }
    else if (static_cast<std::string>(argv[4]).compare("circular") == 0)
    {
        eps = {1, 1, 0};
    }
    else
    {
        printf("Palarisation must be either linear or circular, but %s was given\n", argv[4]);
        return -1;
    }

    // Configure directories for reading and writing
    auto cwd = filesys::current_path(); // current working directory
    auto final_positions = cwd / "equilibration/positions/final.pos";
    if (!(filesys::exists(final_positions)))
    {
        printf("Cannot find positions from initialisation: './equilibration/positions/final.pos'\n");
        return -1;
    }
    // Open files for reading final positions from equilibration
    std::ifstream pos_file(final_positions); // file collecting positions of particles at final equilibration step

    // Create container for particles
    ParticleContainer container;

    // Initialise particle in grid
    size_t c = 0;
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

        Vector3d vel = Vector3d::Zero();
        container.addParticle("carbon_" + std::to_string(c), "C0", pos, vel);
        c++;
    }

    printf("Loaded %zu particles\n", c);

    return 0;
}