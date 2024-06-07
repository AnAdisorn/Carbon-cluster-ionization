#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

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
    Particle(const double &mass, const double &charge, const Vector3d &position);

    void ResetForce();
    void SetForce(const Vector3d &f_new);
    void AddForce(const Vector3d &df);

    void ResetVelocity();
    void SetVelocity(const Vector3d &v_new);
    void AddVelocity(const Vector3d &dv);

    void SetPostion(const Vector3d &x_new);
    void AddPostion(const Vector3d &dx);

    void ResetAll();

    void UpdatePosition(const double &dt);
    void UpdateVelocity(const double &dt);

    // virtual method declaration for particle-particle interaction
    virtual void PPForce(Particle &obj) = 0;
};

class Atom : public Particle
{
public:
    double r0;  // LJ force bond length
    double eps; // LJ force bond potential energy

    void PPForce(Particle &obj) override;
};

#endif /* end of include guard: PARTICLE_H */
