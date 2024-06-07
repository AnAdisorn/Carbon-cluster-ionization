#include "Particle.h"

// **Particle Class**

// Constructor for Particle class
// Sets mass, charge, and position
// Also calls ResetAll() to initialize force and velocity to zero.
Particle::Particle(const double &mass, const double &charge, const Vector3d &position)
{
    m = mass;
    q = charge;
    x = position;
    ResetAll(); // set force and velocity to zero
}

void Particle::ResetForce()
{
    // Sets all components of force to zero
    f.setZero();
}

void Particle::SetForce(const Vector3d &f_new)
{
    // Sets the force to a new value
    f = f_new;
}

void Particle::AddForce(const Vector3d &df)
{
    // Adds a force increment to the current force
    f += df;
}

void Particle::ResetVelocity()
{
    // Sets all components of velocity to zero
    v.setZero();
}

void Particle::SetVelocity(const Vector3d &v_new)
{
    // Sets the velocity to a new value
    v = v_new;
}

void Particle::AddVelocity(const Vector3d &dv)
{
    // Adds a velocity increment to the current velocity
    v += dv;
}

void Particle::SetPostion(const Vector3d &x_new)
{
    // Sets the position to a new value
    x = x_new;
}

void Particle::AddPostion(const Vector3d &dx)
{
    // Adds a position increment to the current position
    x += dx;
}

void Particle::ResetAll()
{
    // Resets both force and velocity to zero
    ResetForce();
    ResetVelocity();
}

void Particle::UpdatePosition(const double &dt)
{
    // 1st step for velocity Verlet integrator
    // Updates position based on current velocity and force
    AddPostion(v * dt + 0.5 * f / m * dt * dt);
    f_prev = f;
    ResetForce();
}

void Particle::UpdateVelocity(const double &dt)
{
    // 2nd step for velocity Verlet integrator
    // Updates velocity based on average force
    AddPostion(0.5 * (f_prev + f) / m * dt);
    ResetForce(); // reset force for the next integration step
}

// **Atom Class (derived from Particle)**

// Defines the force interaction between an Atom and another Particle
void Atom::PPForce(Particle &obj)
{
    Vector3d dr = obj.x - x;        // relative postion
    double r = dr.norm();           // relative distance
    Vector3d df = Vector3d::Zero(); // interaction force
    // if other object is also atom type
    if (dynamic_cast<const Atom *>(&obj))
    {
        // Add Lennard-Jones force (applicable between atoms)
        df += 12 * eps * ((pow(r0, 12) / pow(r, 13)) - (pow(r0, 6) / pow(r, 7))) * dr;
    }
    // Add Coulomb force (applicable to all charged particles)
    df += q * obj.q / (r * r) * dr;
    AddForce(-df);
    obj.AddForce(df);
}

// **Electron Class (derived from Particle)**

// Defines the force interaction between an Electron and another Particle
// (Coulomb force only)
void Electron::PPForce(Particle &obj)
{
    Vector3d dr = obj.x - x; // relative postion
    double r = dr.norm();    // relative distance
    Vector3d df = q * obj.q / (r * r) * dr;
    AddForce(-df);
    obj.AddForce(df);
}
