#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include "Eigen/Dense"
#include <string>
#include <unordered_map>
#include <utility>

// Particle parameters that are independent of other particles
struct PaticleParameters
{
    const int kM; // mass
    const int kQ; // charge

    // Constructor
    PaticleParameters(int kM, int kQ);
};

// Particle parameters that are dependent of other particles, i.e. Lennard-Jones Potential
struct PairParameters
{
    const double kEps;
    const double kR0;

    // Constructor
    PairParameters(double kEps, double kR0);
};

// Particle parameters for ionisation
struct IonisationParameters
{
    const int kN, kL, kM;
    const double kEi;

    // Constructor
    IonisationParameters(int kN, int kL, int kM, double kEi);
};

// Parameters maps
extern std::unordered_map<std::string, PaticleParameters>
    ParticleParametersMap;

extern std::unordered_map<std::pair<std::string, std::string>, PairParameters>
    PairParametersMap;

extern std::unordered_map<std::string, IonisationParameters>
    IonisationParametersMap;

// Calculate potential from Lennard-Jones Potential between two types of particles
double calculateLJPotential(const std::string type1, const std::string type2, const Eigen::Vector3d &r1, const Eigen::Vector3d &r2);
// Calculate force from Lennard-Jones Potential between two types of particles
Eigen::Vector3d calculateLJForce(const std::string type1, const std::string type2, const Eigen::Vector3d &r1, const Eigen::Vector3d &r2);

// Calculate force from Electromagnetic field
Eigen::Vector3d calculateEMForce(const std::string type, const Eigen::Vector3d v, const Eigen::Vector3d E, const Eigen::Vector3d B);

// Function to create sorted pair of strings
inline std::pair<std::string, std::string> createPair(std::string str1, std::string str2)
{
    if (str1.compare(str2) < 0)
    {
        return {str1, str2};
    }
    else
    {
        return {str2, str1};
    }
}

#endif /* end of include guard: INTERACTIONS_H */