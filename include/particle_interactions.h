#ifndef PARTICLE_INTERACTIONS_H
#define PARTICLE_INTERACTIONS_H

#include "vector.h"
#include <string>
#include "parameters.h"

// Calculate potential from Lennard-Jones Potential between two types of particles
double calculateLJPotential(const std::string type1, const std::string type2, const Vector3d &r1, const Vector3d &r2);
// Calculate force from Lennard-Jones Potential between two types of particles
Vector3d calculateLJForce(const std::string type1, const std::string type2, const Vector3d &r1, const Vector3d &r2);

#endif /* end of include guard: PARTICLE_INTERACTIONS_H */