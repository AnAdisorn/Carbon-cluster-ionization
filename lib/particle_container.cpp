#include "particle_container.h"

std::string ParticleContainer::getName(size_t i) const
{
    return particles_[i].name;
}

void ParticleContainer::setName(size_t i, const std::string &name)
{
    particles_[i].name = name;
}

std::string ParticleContainer::getType(size_t i) const
{
    return particles_[i].type;
}

void ParticleContainer::setType(size_t i, const std::string &type)
{
    particles_[i].type = type;
}

Vector3d ParticleContainer::getAcceleration(size_t i) const
{
    return particles_[i].a;
}

void ParticleContainer::setAcceleration(size_t i, const Vector3d &a)
{
    particles_[i].a = a;
}

void ParticleContainer::resetAcceleration(size_t i)
{
    particles_[i].a.setZero();
}

void ParticleContainer::resetAllAcceleration()
{
    for (size_t i = 0; i < particles_.size(); i++)
    {
        particles_[i].a.setZero();
    }
}

void ParticleContainer::addAcceleration(size_t i, const Vector3d &a)
{
    particles_[i].a += a;
}

Vector3d ParticleContainer::getVelocity(size_t i) const
{
    return particles_[i].v;
}

void ParticleContainer::setVelocity(size_t i, const Vector3d &v)
{
    particles_[i].v = v;
}

void ParticleContainer::resetVelocity(size_t i)
{
    particles_[i].v.setZero();
}

void ParticleContainer::resetAllVelocity()
{
    for (size_t i = 0; i < particles_.size(); i++)
    {
        particles_[i].v.setZero();
    }
}

void ParticleContainer::addVelocity(size_t i, const Vector3d &v)
{
    particles_[i].v += v;
}

Vector3d ParticleContainer::getPosition(size_t i) const
{
    return particles_[i].x;
}

void ParticleContainer::setPosition(size_t i, const Vector3d &x)
{
    particles_[i].x = x;
}

void ParticleContainer::addPosition(size_t i, const Vector3d &x)
{
    particles_[i].x += x;
}

void ParticleContainer::addPositionChange(const size_t i, const Vector3d &dx)
{
    particles_[i].dx += dx;
}

void ParticleContainer::applyPositionChange(const size_t i)
{
    particles_[i].x += particles_[i].dx;
    particles_[i].dx.setZero();
}

void ParticleContainer::applyPositionChangeAll()
{
    for (size_t i = 0; i < n_; i++)
    {
        applyPositionChange(i);
    }
}

void ParticleContainer::addFields(size_t i, Vector3d &e, Vector3d &b)
{
    particles_[i].e += e;
    particles_[i].b += b;
}

std::array<Vector3d, 2> ParticleContainer::getFields(size_t i)
{
    // Copy fields
    std::array<Vector3d, 2> fields = {particles_[i].e, particles_[i].b};
    // Reset fields to zero
    particles_[i].e.setZero();
    particles_[i].b.setZero();
    return fields;
}

void ParticleContainer::addParticle(std::string name, std::string type, const Vector3d &x, const Vector3d &v)
{
    Particle p;
    p.name = name;
    p.type = type;
    p.x = x;
    p.v = v;
    particles_.push_back(p);
    n_++;
}

void ParticleContainer::removeParticle(size_t i)
{
    particles_.erase(particles_.begin() + i);
    n_--;
}

size_t ParticleContainer::size() const
{
    return n_;
}
