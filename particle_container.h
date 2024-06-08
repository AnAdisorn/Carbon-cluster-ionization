#ifndef PARTICLE_CONTAINER_H
#define PARTICLE_CONTAINER_H

#include "Eigen/Dense"
#include <string>
#include <vector>

using Eigen::Vector3d;
using std::string;
using std::vector;

struct particle
{
    string name;
    string type;
    Vector3d a = Vector3d::Zero();
    Vector3d v = Vector3d::Zero();
    Vector3d x = Vector3d::Zero();
};

class ParticleContainer
{
public:
    // bool checkIndex(int i) const;

    const string getName(int i) const;
    void setName(int i, const string &name);

    const string getType(int i) const;
    void setType(int i, const string &type);

    const Vector3d getAcceleration(int i) const;
    void setAcceleration(int i, const Vector3d &a);
    void resetAcceleration(int i);
    void resetAllAcceleration();
    void addAcceleration(int i, const Vector3d &a);

    const Vector3d getVelocity(int i) const;
    void setVelocity(int i, const Vector3d &v);
    void resetVelocity(int i);
    void resetAllVelocity();
    void addVelocity(int i, const Vector3d &v);

    const Vector3d getPosition(int i) const;
    void setPosition(int i, const Vector3d &x);
    void addPosition(int i, const Vector3d &x);

    void addParticle(string name, string type, const Vector3d &v, const Vector3d &x);
    void removeParticle(int i);

private:
    vector<particle> particles;
};

#endif /* end of include guard: PARTICLE_CONTAINER_H */