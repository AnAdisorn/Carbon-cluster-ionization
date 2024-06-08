#ifndef PARTICLE_CONTAINER_H
#define PARTICLE_CONTAINER_H

#include "Eigen/Dense"
#include <string>
#include <vector>

using Eigen::Vector3d;
using std::string;
using std::vector;

// Structure to represent a particle
struct particle
{
    string name;                   // Name of the particle
    string type;                   // Type of the particle
    Vector3d a = Vector3d::Zero(); // Acceleration (initialized to zero)
    Vector3d v = Vector3d::Zero(); // Velocity (initialized to zero)
    Vector3d x = Vector3d::Zero(); // Position (initialized to zero)
};

// Class to represent a container of particles
class ParticleContainer
{
public:
    // Function to check if an index is within bounds (not implemented here)
    // bool checkIndex(int i) const;

    // Get the name of a particle at a specific index
    const string getName(int i) const;

    // Set the name of a particle at a specific index
    void setName(int i, const string &name);

    // Get the type of a particle at a specific index
    const string getType(int i) const;

    // Set the type of a particle at a specific index
    void setType(int i, const string &type);

    // Get the acceleration of a particle at a specific index
    const Vector3d getAcceleration(int i) const;

    // Set the acceleration of a particle at a specific index
    void setAcceleration(int i, const Vector3d &a);

    // Reset the acceleration of a particle to zero (i.e., a = 0)
    void resetAcceleration(int i);

    // Reset the acceleration of all particles to zero
    void resetAllAcceleration();

    // Add a specified value to the acceleration of a particle
    void addAcceleration(int i, const Vector3d &a);

    // Get the velocity of a particle at a specific index
    const Vector3d getVelocity(int i) const;

    // Set the velocity of a particle at a specific index
    void setVelocity(int i, const Vector3d &v);

    // Reset the velocity of a particle to zero (i.e., v = 0)
    void resetVelocity(int i);

    // Reset the velocity of all particles to zero
    void resetAllVelocity();

    // Add a specified value to the velocity of a particle
    void addVelocity(int i, const Vector3d &v);

    // Get the position of a particle at a specific index
    const Vector3d getPosition(int i) const;

    // Set the position of a particle at a specific index
    void setPosition(int i, const Vector3d &x);

    // Add a specified value to the position of a particle
    void addPosition(int i, const Vector3d &x);

    // Add a new particle to the container
    void addParticle(string name, string type, const Vector3d &v, const Vector3d &x);

    // Remove a particle from the container by index
    void removeParticle(int i);

private:
    // Internal storage for particles as a vector
    vector<particle> particles;
};

#endif /* end of include guard: PARTICLE_CONTAINER_H */