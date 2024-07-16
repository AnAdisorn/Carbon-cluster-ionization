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

const std::string ParticleContainer::getName(int i) const
{ // if (checkIndex(i))
    return particles_[i].name;
}

void ParticleContainer::setName(int i, const std::string &name)
{ // if (checkIndex(i))
    particles_[i].name = name;
}

const std::string ParticleContainer::getType(int i) const
{ // if (checkIndex(i))
    return particles_[i].type;
}

void ParticleContainer::setType(int i, const std::string &type)
{ // if (checkIndex(i))
    particles_[i].type = type;
}

const Eigen::Vector3d ParticleContainer::getAcceleration(int i) const
{ // if (checkIndex(i))
    return particles_[i].a;
}

void ParticleContainer::setAcceleration(int i, const Eigen::Vector3d &a)
{ // if (checkIndex(i))
    particles_[i].a = a;
}

void ParticleContainer::resetAcceleration(int i)
{ // if (checkIndex(i))
    particles_[i].a.setZero();
}

void ParticleContainer::resetAllAcceleration()
{ // if (checkIndex(i))
    for (size_t i = 0; i < particles_.size(); i++)
    {
        particles_[i].a.setZero();
    }
}

void ParticleContainer::addAcceleration(int i, const Eigen::Vector3d &a)
{ // if (checkIndex(i))
    particles_[i].a += a;
}

const Eigen::Vector3d ParticleContainer::getVelocity(int i) const
{ // if (checkIndex(i))
    return particles_[i].v;
}

void ParticleContainer::setVelocity(int i, const Eigen::Vector3d &v)
{ // if (checkIndex(i))
    particles_[i].v = v;
}

void ParticleContainer::resetVelocity(int i)
{ // if (checkIndex(i))
    particles_[i].v.setZero();
}

void ParticleContainer::resetAllVelocity()
{ // if (checkIndex(i))
    for (size_t i = 0; i < particles_.size(); i++)
    {
        particles_[i].v.setZero();
    }
}

void ParticleContainer::addVelocity(int i, const Eigen::Vector3d &v)
{ // if (checkIndex(i))
    particles_[i].v += v;
}

const Eigen::Vector3d ParticleContainer::getPosition(int i) const
{ // if (checkIndex(i))
    return particles_[i].x;
}

void ParticleContainer::setPosition(int i, const Eigen::Vector3d &x)
{ // if (checkIndex(i))
    particles_[i].v = x;
}

void ParticleContainer::addPosition(int i, const Eigen::Vector3d &x)
{ // if (checkIndex(i))
    particles_[i].v += x;
}

void ParticleContainer::addParticle(std::string name, std::string type, const Eigen::Vector3d &x, const Eigen::Vector3d &v)
{
    particle p;
    p.name = name;
    p.type = type;
    p.x = x;
    p.v = v;
    particles_.push_back(p);
    n_++;
}

void ParticleContainer::removeParticle(int i)
{ // if (checkIndex(i))
    particles_.erase(particles_.begin() + i);
    n_--
}

size_t size() const
{
    return n_;
}
