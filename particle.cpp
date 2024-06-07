#include "Particle.h"

Particle::Particle(const double &mass, const double &charge, const Vector3d &position)
{
    m = mass;
    q = charge;
    x = position;
    ResetAll(); // set force and velocity to zero
}

void Particle::ResetForce()
{
    f.setZero();
}

void Particle::SetForce(const Vector3d &f_new)
{
    f = f_new;
}

void Particle::AddForce(const Vector3d &df)
{
    f += df;
}

void Particle::ResetVelocity()
{
    v.setZero();
}

void Particle::SetVelocity(const Vector3d &v_new)
{
    v = v_new;
}

void Particle::AddVelocity(const Vector3d &dv)
{
    v += dv;
}

void Particle::SetPostion(const Vector3d &x_new)
{
    x = x_new;
}

void Particle::AddPostion(const Vector3d &dx)
{
    x += dx;
}

void Particle::ResetAll()
{
    ResetForce();
    ResetVelocity();
}

void Particle::UpdatePosition(const double &dt)
{
    // 1st step for velocity Verlet
    AddPostion(v * dt + 0.5 * f / m * dt * dt);
    f_prev = f;
    ResetForce();
}

void Particle::UpdateVelocity(const double &dt)
{
    // 2nd step for velocity Verlet
    AddPostion(0.5 * (f_prev + f) * dt);
    ResetForce(); // reset force for the next integration step
}

void Atom::PPForce(Particle &obj)
{
    Vector3d dr = obj.x - x;        // relative postion
    double r = dr.norm();           // relative distance
    Vector3d df = Vector3d::Zero(); // interaction force
    // if other object is also atom type
    if (dynamic_cast<const Atom *>(&obj))
    {
        // Add Lennard-Jones force
        df += 12 * eps * ((pow(r0, 12) / pow(r, 13)) - (pow(r0, 6) / pow(r, 7))) * dr;
    }
    // Add Coulomb force
    df += q * obj.q / (r * r) * dr;
    AddForce(df);
    obj.AddForce(-df);
}

void Electron::PPForce(Particle &obj)
{
    Vector3d dr = obj.x - x; // relative postion
    double r = dr.norm();    // relative distance
    Vector3d df = q * obj.q / (r * r) * dr;
    AddForce(df);
    obj.AddForce(-df);
}
