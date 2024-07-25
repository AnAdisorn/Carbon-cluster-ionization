#ifndef LASER_FIELD_H
#define LASER_FIELD_H

#include "vector.h"
#include "parameters.h"

Vector3cd vectorPotential(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3cd &r, const double t);

Vector3d electricField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t);

Vector3d magneticField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t);

#endif /* end of include guard: LASER_FIELD_H */