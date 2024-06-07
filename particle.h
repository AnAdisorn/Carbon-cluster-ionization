#ifndef PARTICLE_H
#define PARTICLE_H

#include "Eigen/Dense"
using Eigen::Vector3d;

class Particle
{
public:
    double m;        // mass
    double q;        // charge
    Vector3d f;      // force
    Vector3d f_prev; // previous force (before postion update), for verlet intgrator
    Vector3d v;      // velocity
    Vector3d x;      // position

    // Constructor
    Particle(const double &mass, const double &charge, const Vector3d &position)
    {
        m = mass;
        q = charge;
        x = position;
    }

    void ResetForce()
    {
        f.setZero();
    }

    void SetForce(const Vector3d &f_new)
    {
        f = f_new;
    }

    void AddForce(const Vector3d &df)
    {
        f += df;
    }

    void ResetVelocity()
    {
        v.setZero();
    }

    void SetVelocity(const Vector3d &v_new)
    {
        v = v_new;
    }

    void AddVelocity(const Vector3d &dv)
    {
        v += dv;
    }

    void SetPostion(const Vector3d &x_new)
    {
        x = x_new;
    }

    void AddPostion(const Vector3d &dx)
    {
        x += dx;
    }

    void ResetAll()
    {
        ResetForce();
        ResetVelocity();
    }

    void UpdatePosition(const double &dt)
    {
        // 1st step for velocity Verlet
        AddPostion(v * dt + 0.5 * f / m * dt * dt);
        f_prev = f;
        ResetForce();
    }

    void UpdateVelocity(const double &dt)
    {
        // 2nd step for velocity Verlet
        AddPostion(0.5 * (f_prev + f) * dt);
        ResetForce(); // reset force for the next integration step
    }

    // virtual method for particle-particle interaction
    virtual void PPForce(Particle &obj) = 0;
};

class Atom : public Particle
{
public:
    double r0;  // LJ force bond length
    double eps; // LJ force bond potential energy
    void PPForce(Particle &obj)
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
};

#endif /* end of include guard: PARTICLE_H */