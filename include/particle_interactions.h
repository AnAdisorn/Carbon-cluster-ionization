#ifndef PARTICLE_INTERACTIONS_H
#define PARTICLE_INTERACTIONS_H

#include "Eigen/Dense"
#include <string>
#include "parameters.h"

// Calculate potential from Lennard-Jones Potential between two types of particles
double calculateLJPotential(const std::string type1, const std::string type2, const Eigen::Vector3d &r1, const Eigen::Vector3d &r2);
// Calculate force from Lennard-Jones Potential between two types of particles
Eigen::Vector3d calculateLJForce(const std::string type1, const std::string type2, const Eigen::Vector3d &r1, const Eigen::Vector3d &r2);

// Calculate force from Electromagnetic field
Eigen::Vector3d calculateEMForce(const std::string type, const Eigen::Vector3d v, const Eigen::Vector3d E, const Eigen::Vector3d B);

#endif /* end of include guard: PARTICLE_INTERACTIONS_H */