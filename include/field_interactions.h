#ifndef FIELD_INTERACTIONS_H
#define FIELD_INTERACTIONS_H

#include "parameters.h"
#include "vector.h"
#include <string>

static int c = 137;

// Convert v (classical velocity) to u (relativistic)
inline Vector3d convertV2U(const Vector3d &v)
{
    return v / sqrt(1 - pow(v.norm() / c, 2));
}

// Convert u (relativistic) to v (classical velocity)
inline Vector3d convertU2V(const Vector3d &u)
{
    return u / sqrt(1 + pow(u.norm() / c, 2));
}

// Calculate gamma factor from classical velocity
inline double gammaV(const Vector3d &v)
{
    return 1 / sqrt(1 - pow(v.norm() / c, 2));
}

// Calculate gamma factor from relativistic velocity
inline double gammaU(const Vector3d &u)
{
    return sqrt(1 + pow(u.norm() / c, 2));
}

inline Vector3d updateHalfPosition(const Vector3d &x, const Vector3d &v, double dt)
{
    return x + v * dt / 2;
}

// Calculate new velocity from Electromagnetic field with Boris method
Vector3d updateVelocityBoris(const std::string type, const Vector3d &v, const Vector3d &E, const Vector3d &B, double dt);

// Calculate new velocity from Electromagnetic field with Vay method
Vector3d updateVelocityVay(const std::string type, const Vector3d &v, const Vector3d &E, const Vector3d &B, double dt);

// Calculate new velocity from Electromagnetic field with Higuera-Cary method
Vector3d updateVelocityHC(const std::string type, const Vector3d &v, const Vector3d &E, const Vector3d &B, double dt);

// Calculate ionisation rate
double ionisationRate(const std::string type, double f);

#endif /* end of include guard: FIELD_INTERACTIONS_H */