#ifndef VECTOR_H
#define VECTOR_H

#include "Eigen/Dense"

typedef Eigen::Vector3d Vector3d;

inline Vector3d randomUnitVector()
{
    Vector3d vec = {static_cast<double>(rand()) / RAND_MAX, static_cast<double>(rand()) / RAND_MAX, static_cast<double>(rand()) / RAND_MAX};
    return vec.normalized();
}

#endif /* end of include guard: VECTOR_H */