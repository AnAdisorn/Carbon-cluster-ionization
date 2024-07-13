#ifndef FORCE_FIELD_H
#define FORCE_FIELD_H

#include "Eigen/Dense"
#include <string>
#include <unordered_map>
#include <utility>

// Particle parameters that are independent of other particles
struct IndependentParameters
{
    const double kM; // mass
    const double kQ; // charge
};

// Particle parameters that are dependent of other particles, i.e. Lennard-Jones Potential
struct PairParameters
{
    const double kEps;
    const double kR0;
};

const std::unordered_map<std::string, IndependentParameters>
    kIndependentParametersMap;

const std::unordered_map<std::pair<std::string, std::string>, PairParameters>
    kPairParametersMap;

// Calculate force from Lennard-Jones Potential (interaction force) between two types particles
Eigen::Vector3d calculateLJForce(const std::string type1, const std::string type2, const Eigen::Vector3d &r1, const Eigen::Vector3d &r2);

// Calculate force from Electromagnetic field
Eigen::Vector3d calculateEMForce(const std::string type, const Eigen::Vector3d v, const Eigen::Vector3d E, const Eigen::Vector3d B);

#endif /* end of include guard: FORCE_FIELD_H */