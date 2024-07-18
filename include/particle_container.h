#ifndef PARTICLE_CONTAINER_H
#define PARTICLE_CONTAINER_H

#include "vector.h"
#include <string>
#include <vector>

// Class to represent a particle
struct Particle
{
    std::string name;               // Name of the particle
    std::string type;               // Type of the particle
    Vector3d x = Vector3d::Zero();  // Position (default to zero)
    Vector3d dx = Vector3d::Zero(); // Accumulated change of position to be applied
    Vector3d v = Vector3d::Zero();  // Velocity (default to zero)
    Vector3d a = Vector3d::Zero();  // Acceleration (default to zero)
    Vector3d e = Vector3d::Zero();  // Electric field (default to zero)
    Vector3d b = Vector3d::Zero();  // Mlectric field (default to zero)
};

// Class to represent a container of particles
class ParticleContainer
{
public:
    // Function to check if an index is within bounds (not implemented here)
    // bool checkIndex(const int i) const;

    // Get the name of a particle at a specific index
    std::string getName(const size_t i) const;

    // Set the name of a particle at a specific index
    void setName(const size_t i, const std::string &name);

    // Get the type of a particle at a specific index
    std::string getType(const size_t i) const;

    // Set the type of a particle at a specific index
    void setType(const size_t i, const std::string &type);

    // Get the acceleration of a particle at a specific index
    Vector3d getAcceleration(const size_t i) const;

    // Set the acceleration of a particle at a specific index
    void setAcceleration(const size_t i, const Vector3d &a);

    // Reset the acceleration of a particle to zero (i.e., a = 0)
    void resetAcceleration(const size_t i);

    // Reset the acceleration of all particles to zero
    void resetAllAcceleration();

    // Add a specified value to the acceleration of a particle
    void addAcceleration(const size_t i, const Vector3d &a);

    // Get the velocity of a particle at a specific index
    Vector3d getVelocity(const size_t i) const;

    // Set the velocity of a particle at a specific index
    void setVelocity(const size_t i, const Vector3d &v);

    // Reset the velocity of a particle to zero (i.e., v = 0)
    void resetVelocity(const size_t i);

    // Reset the velocity of all particles to zero
    void resetAllVelocity();

    // Add a specified value to the velocity of a particle
    void addVelocity(const size_t i, const Vector3d &v);

    // Get the position of a particle at a specific index
    Vector3d getPosition(const size_t i) const;

    // Set the position of a particle at a specific index
    void setPosition(const size_t i, const Vector3d &x);

    // Add a specified value to the position of a particle
    void addPosition(const size_t i, const Vector3d &x);

    // Add a specified value to the accumulated position change of a particle
    void addPositionChange(const size_t i, const Vector3d &dx);

    // Appply the accumulated position change of a particle to position
    void applyPositionChange(const size_t i);

    // Appply the accumulated position change of all particle to position
    void applyPositionChangeAll();

    // Adding field values (electric and magnetic) to particle
    void addFields(size_t i, const Vector3d &e, const Vector3d &b);

    // Get field values (electric and magnetic) from particle
    std::array<Vector3d, 2> getFields(size_t i);

    // Add a new particle to the container
    void addParticle(std::string name, std::string type, const Vector3d &v, const Vector3d &x);

    // Remove a particle from the container by index
    void removeParticle(const size_t i);

    // Number of particles in container
    size_t size() const;

private:
    // Internal storage for particles as a vector of Particle
    std::vector<Particle> particles_;
    size_t n_ = 0;
};

#endif /* end of include guard: PARTICLE_CONTAINER_H */
