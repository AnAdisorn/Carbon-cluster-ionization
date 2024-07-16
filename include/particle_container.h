#ifndef PARTICLE_CONTAINER_H
#define PARTICLE_CONTAINER_H

#include "vector.h"
#include <string>
#include <vector>

// Structure to represent a particle
struct particle
{
    std::string name;              // Name of the particle
    std::string type;              // Type of the particle
    Vector3d x = Vector3d::Zero(); // Position (default to zero)
    Vector3d v = Vector3d::Zero(); // Velocity (default to zero)
    Vector3d a = Vector3d::Zero(); // Acceleration (default to zero)
};

// Class to represent a container of particles
class ParticleContainer
{
public:
    // Function to check if an index is within bounds (not implemented here)
    // bool checkIndex(const int i) const;

    // Get the name of a particle at a specific index
    const std::string getName(const int i) const;

    // Set the name of a particle at a specific index
    void setName(const int i, const std::string &name);

    // Get the type of a particle at a specific index
    const std::string getType(const int i) const;

    // Set the type of a particle at a specific index
    void setType(const int i, const std::string &type);

    // Get the acceleration of a particle at a specific index
    const Vector3d getAcceleration(const int i) const;

    // Set the acceleration of a particle at a specific index
    void setAcceleration(const int i, const Vector3d &a);

    // Reset the acceleration of a particle to zero (i.e., a = 0)
    void resetAcceleration(const int i);

    // Reset the acceleration of all particles to zero
    void resetAllAcceleration();

    // Add a specified value to the acceleration of a particle
    void addAcceleration(const int i, const Vector3d &a);

    // Get the velocity of a particle at a specific index
    const Vector3d getVelocity(const int i) const;

    // Set the velocity of a particle at a specific index
    void setVelocity(const int i, const Vector3d &v);

    // Reset the velocity of a particle to zero (i.e., v = 0)
    void resetVelocity(const int i);

    // Reset the velocity of all particles to zero
    void resetAllVelocity();

    // Add a specified value to the velocity of a particle
    void addVelocity(const int i, const Vector3d &v);

    // Get the position of a particle at a specific index
    const Vector3d getPosition(const int i) const;

    // Set the position of a particle at a specific index
    void setPosition(const int i, const Vector3d &x);

    // Add a specified value to the position of a particle
    void addPosition(const int i, const Vector3d &x);

    // Add a new particle to the container
    void addParticle(std::string name, std::string type, const Vector3d &v, const Vector3d &x);

    // Remove a particle from the container by index
    void removeParticle(const int i);

private:
    // Internal storage for particles as a vector
    std::vector<particle> particles_;
};

#endif /* end of include guard: PARTICLE_CONTAINER_H */