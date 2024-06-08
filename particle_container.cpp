#include "particle_container.h"

// bool ParticleContainer::checkIndex(int i) const
// {
//     if (i >= 0 && i < particles.size())
//     {
//         return true;
//     }
//     else
//     {
//         throw std::out_of_range("Index out of bounds for particle container");
//     }
// }

const string ParticleContainer::getName(int i) const
{ // if (checkIndex(i))
    return particles[i].name;
}

void ParticleContainer::setName(int i, const string &name)
{ // if (checkIndex(i))
    particles[i].name = name;
}

const string ParticleContainer::getType(int i) const
{ // if (checkIndex(i))
    return particles[i].type;
}

void ParticleContainer::setType(int i, const string &type)
{ // if (checkIndex(i))
    particles[i].type = type;
}

const Vector3d ParticleContainer::getAcceleration(int i) const
{ // if (checkIndex(i))
    return particles[i].a;
}

void ParticleContainer::setAcceleration(int i, const Vector3d &a)
{ // if (checkIndex(i))
    particles[i].a = a;
}

void ParticleContainer::resetAcceleration(int i)
{ // if (checkIndex(i))
    particles[i].a.setZero();
}

void ParticleContainer::resetAllAcceleration()
{ // if (checkIndex(i))
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i].a.setZero();
    }
}

void ParticleContainer::addAcceleration(int i, const Vector3d &a)
{ // if (checkIndex(i))
    particles[i].a += a;
}

const Vector3d ParticleContainer::getVelocity(int i) const
{ // if (checkIndex(i))
    return particles[i].v;
}

void ParticleContainer::setVelocity(int i, const Vector3d &v)
{ // if (checkIndex(i))
    particles[i].v = v;
}

void ParticleContainer::resetVelocity(int i)
{ // if (checkIndex(i))
    particles[i].v.setZero();
}

void ParticleContainer::resetAllVelocity()
{ // if (checkIndex(i))
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i].v.setZero();
    }
}

void ParticleContainer::addVelocity(int i, const Vector3d &v)
{ // if (checkIndex(i))
    particles[i].v += v;
}

const Vector3d ParticleContainer::getPosition(int i) const
{ // if (checkIndex(i))
    return particles[i].x;
}

void ParticleContainer::setPosition(int i, const Vector3d &x)
{ // if (checkIndex(i))
    particles[i].v = x;
}

void ParticleContainer::addPosition(int i, const Vector3d &x)
{ // if (checkIndex(i))
    particles[i].v += x;
}

void ParticleContainer::addParticle(string name, string type, const Vector3d &v, const Vector3d &x)
{
    particle p;
    p.name = name;
    p.type = type;
    p.v = v;
    p.x = x;
    particles.push_back(p);
}

void ParticleContainer::removeParticle(int i)
{ // if (checkIndex(i))
    particles.erase(particles.begin() + i);
}