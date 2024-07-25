#include "laser_field.h"

using namespace std::complex_literals;

Vector3cd vectorPotential(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3cd &r, const double t)
{
    const std::complex<double> kR_dot_n = r.dot(n);        // r . n
    const std::complex<double> kB_r_n = b + 1i * kR_dot_n; // b + i r . n
    const Vector3cd kR_cross_n = r.cross(n);               // r x n

    std::complex<double> a0 = e0 * pow(b, 2) / (w * pow(abs(kB_r_n), 2));                           // Magnitude of vector potential pulse
    Vector3cd vec = eps + w / c * r.dot(eps.cross(n)) * kR_cross_n / kB_r_n;                        // Vector term
    std::complex<double> exp1 = exp(-1i * w * (t - kR_dot_n / c));                                  // First exponential term
    std::complex<double> exp2 = exp(-w / (2 * c * (b + 1i * kR_dot_n)) * kR_cross_n.squaredNorm()); // Second exponential term

    return a0 * vec * exp1 * exp2;
}

Vector3d electricField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t)
{
    return -w * vectorPotential(e0, w, b, n, eps, r, t).imag();
}

Vector3d magneticField(const double e0, const double w, const double b, const Vector3d &n, const Vector3d &eps, const Vector3d &r, const double t)
{
    const double h = 1e-200;

    const Vector3cd kA = vectorPotential(e0, w, b, n, eps, r, t);

    Vector3cd dA_dx = (vectorPotential(e0, w, b, n, eps, r + Vector3d{h, 0, 0}, t) - kA) / h; // dA/dx
    Vector3cd dA_dy = (vectorPotential(e0, w, b, n, eps, r + Vector3d{0, h, 0}, t) - kA) / h; // dA/dy
    Vector3cd dA_dz = (vectorPotential(e0, w, b, n, eps, r + Vector3d{0, 0, h}, t) - kA) / h; // dA/dz

    double x, y, z; // Magnitic field in respective axis
    x = (dA_dy[2] - dA_dz[1]).real();
    y = (dA_dz[0] - dA_dx[2]).real();
    z = (dA_dx[1] - dA_dy[0]).real();

    return {x, y, z};
}