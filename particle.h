#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#include "Eigen/Dense"
using Eigen::Vector3d;

// **Particle Class**

// This class represents a basic particle with mass, charge, position, velocity, and force.
class Particle
{
public:
    // Member variables
    double m;        // mass of the particle
    double q;        // charge of the particle
    Vector3d f;      // current force acting on the particle
    Vector3d f_prev; // previous force (before position update), used for Verlet integrator
    Vector3d v;      // velocity of the particle
    Vector3d x;      // position of the particle

    // Constructor
    // Initializes the particle with given mass, charge, and position.
    // Also sets force and velocity to zero by calling ResetAll().
    Particle(const double &mass, const double &charge, const Vector3d &position);

    // Function to set all force components to zero
    void ResetForce();

    // Function to set the force to a new value
    void SetForce(const Vector3d &f_new);

    // Function to add an increment to the current force
    void AddForce(const Vector3d &df);

    // Function to set all velocity components to zero
    void ResetVelocity();

    // Function to set the velocity to a new value
    void SetVelocity(const Vector3d &v_new);

    // Function to add an increment to the current velocity
    void AddVelocity(const Vector3d &dv);

    // Function to set the position to a new value
    void SetPostion(const Vector3d &x_new);

    // Function to add an increment to the current position
    void AddPostion(const Vector3d &dx);

    // Function to reset both force and velocity to zero
    void ResetAll();

    // Function to update the position of the particle based on current velocity and force (using Verlet integrator)
    void UpdatePosition(const double &dt);

    // Function to update the velocity of the particle based on average force (using Verlet integrator)
    void UpdateVelocity(const double &dt);

    // Pure virtual function declaration for particle-particle interaction.
    // Derived classes (like Atom and Electron) must provide their own implementation for this function.
    virtual void PPForce(Particle &obj) = 0;
};

// **Atom Class (derived from Particle)**

// This class represents an atom particle with additional properties for Lennard-Jones (LJ) force calculation.
class Atom : public Particle
{
public:
    double r0;  // LJ force bond length
    double eps; // LJ force bond potential energy

    // Override of the virtual PPForce function to calculate force interaction between an Atom and another Particle.
    void PPForce(Particle &obj) override;
};

// **Electron Class (derived from Particle)**

// This class represents an electron particle.
class Electron : public Particle
{
public:
    // Override of the virtual PPForce function to calculate force interaction between an Electron and another Particle
    void PPForce(Particle &obj) override;
};

#endif /* end of include guard: PARTICLE_H */
