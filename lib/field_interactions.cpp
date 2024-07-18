#include "field_interactions.h"

Vector3d updateVelocityBoris(const std::string type, const Vector3d &v, const Vector3d &e, const Vector3d &b, double dt)
{
    ParticleParameters &params = ParticleParametersMap.at(type);
    const double m = params.kM;
    const double q = params.kQ;
    Vector3d u = convertV2U(v);

    // First half electric field acceleration
    Vector3d u_minus = u + q * e * dt / (2 * m);
    // Rotation step
    double g = gammaU(u_minus);
    Vector3d t = b * q * dt / (2 * m * g);
    Vector3d s = 2 * t / (1 + pow(t.norm(), 2));
    Vector3d u_plus = u_minus + (u_minus + (u_minus.cross(t))).cross(s);

    return convertU2V(u_plus + q * e * dt / (2 * m));
}

Vector3d updateVelocityVay(const std::string type, const Vector3d &v, const Vector3d &E, const Vector3d &B, double dt)
{
    ParticleParameters &params = ParticleParametersMap.at(type);
    const double m = params.kM;
    const double q = params.kQ;
    Vector3d u = convertV2U(v);

    // Field contribution
    Vector3d u_half = u + q * dt * (E + v.cross(B)) / (2 * m);
    // Rotation step
    Vector3d u_prime = u_half + E * q * dt / (2 * m);
    Vector3d tau = B * q * dt / (2 * m);
    double tau_norm = tau.norm();
    double u_star = u_prime.dot(tau) / c;
    double g_prime = gammaU(u_prime);
    double sigma = pow(g_prime, 2) - pow(tau_norm, 2);
    double g_next = sqrt((sigma + sqrt(pow(sigma, 2) + 4 * (pow(tau_norm, 2) + pow(u_star, 2)))) / 2);
    Vector3d t = tau / g_next;
    double s = 1 / (1 + pow(t.norm(), 2));

    return convertU2V(s * (u_prime + (u_prime.dot(t)) * t + u_prime.cross(t)));
}

Vector3d updateVelocityHC(const std::string type, const Vector3d &v, const Vector3d &E, const Vector3d &B, double dt)
{
    ParticleParameters &params = ParticleParametersMap.at(type);
    const double m = params.kM;
    const double q = params.kQ;
    Vector3d u = convertV2U(v);

    // First half electric field acceleration
    Vector3d u_minus = u + q * E * dt / (2 * m);
    // Rotation step
    double g_minus = gammaU(u_minus);
    Vector3d tau = B * q * dt / (2 * m);
    double tau_norm = tau.norm();
    double u_star = u_minus.dot(tau) / c;
    double sigma = pow(g_minus, 2) - pow(tau_norm, 2);
    double g_plus = sqrt((sigma + sqrt(pow(sigma, 2) + 4 * (pow(tau_norm, 2) + pow(u_star, 2)))) / 2);
    Vector3d t = tau / g_plus;
    double s = 1 / (1 + pow(t.norm(), 2));
    Vector3d u_plus = s * (u_minus + (u_minus.dot(t)) * t + u_minus.cross(t));
    // Second half electric field acceleration
    return convertU2V(u_plus + q * E * dt / (2 * m) + u_minus.cross(t));
}

double ionisationRate(const std::string type, double f)
{
    IonisationParameters &params = IonisationParametersMap.at(type);
    const double n_star = params.kN_star;
    const double cnl_sqr = params.kCnl_sqr;
    const double flm = params.kFlm;
    const double ei = params.kEi;
    const int m = params.kM;

    return cnl_sqr * sqrt(6 / M_PI) * flm * ei * pow(2 * pow(2 * ei, 1.5) / f, 2 * n_star - abs(m) - 1.5) * exp(-2 * pow(2 * ei, 1.5) / (3 * f));
}
