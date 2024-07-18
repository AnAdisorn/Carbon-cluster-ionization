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

Vector3d updateVelocityVay(const std::string type, const Vector3d &v, const Vector3d &e, const Vector3d &b, double dt)
{
    ParticleParameters &params = ParticleParametersMap.at(type);
    const double m = params.kM;
    const double q = params.kQ;
    Vector3d u = convertV2U(v);

    // Field contribution
    Vector3d u_half = u + q * dt * (e + v.cross(b)) / (2 * m);
    // Rotation step
    Vector3d u_prime = u_half + e * q * dt / (2 * m);
    Vector3d tau = b * q * dt / (2 * m);
    double tau_norm = tau.norm();
    double u_star = u_prime.dot(tau) / c;
    double g_prime = gammaU(u_prime);
    double sigma = pow(g_prime, 2) - pow(tau_norm, 2);
    double g_next = sqrt((sigma + sqrt(pow(sigma, 2) + 4 * (pow(tau_norm, 2) + pow(u_star, 2)))) / 2);
    Vector3d t = tau / g_next;
    double s = 1 / (1 + pow(t.norm(), 2));

    return convertU2V(s * (u_prime + (u_prime.dot(t)) * t + u_prime.cross(t)));
}

Vector3d updateVelocityHC(const std::string type, const Vector3d &v, const Vector3d &e, const Vector3d &b, double dt)
{
    ParticleParameters &params = ParticleParametersMap.at(type);
    const double m = params.kM;
    const double q = params.kQ;
    Vector3d u = convertV2U(v);

    // First half electric field acceleration
    Vector3d u_minus = u + q * e * dt / (2 * m);
    // Rotation step
    double g_minus = gammaU(u_minus);
    Vector3d tau = b * q * dt / (2 * m);
    double tau_norm = tau.norm();
    double u_star = u_minus.dot(tau) / c;
    double sigma = pow(g_minus, 2) - pow(tau_norm, 2);
    double g_plus = sqrt((sigma + sqrt(pow(sigma, 2) + 4 * (pow(tau_norm, 2) + pow(u_star, 2)))) / 2);
    Vector3d t = tau / g_plus;
    double s = 1 / (1 + pow(t.norm(), 2));
    Vector3d u_plus = s * (u_minus + (u_minus.dot(t)) * t + u_minus.cross(t));
    // Second half electric field acceleration
    return convertU2V(u_plus + q * e * dt / (2 * m) + u_minus.cross(t));
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

std::array<Vector3d, 4> calculatePairFields(const std::string type1, const std::string type2, const Vector3d &r1, const Vector3d &r2, const Vector3d &v1, const Vector3d &v2)
{
    Vector3d e1, e2, b1, b2;

    const double q1 = ParticleParametersMap.at(type1).kQ;
    const double q2 = ParticleParametersMap.at(type2).kQ;
    Vector3d r21 = r2 - r1;
    Vector3d r_inverse_2 = r21.normalized() / pow(r21.norm(), 2);

    // Coulomb law
    // if (q2 > 0 && !(IonisationParametersMap.find(lowerType(type2)) == IonisationParametersMap.end()))
    // {
    //     std::string lower_type = lowerType(type2);
    //     double r_soft = pow(q1 * q2, 2) / IonisationParametersMap[lower_type].kEi;
    //     e1 = q2 * pow(pow(r21.norm(), 2) + r_soft, 1.5) * (-r21);
    // }
    // else
    {
        e1 = q2 * (-r_inverse_2);
    }

    // if (q1 > 0 && !(IonisationParametersMap.find(lowerType(type1)) == IonisationParametersMap.end()))
    // {
    //     double r_soft = pow(q1 * q2, 2) / IonisationParametersMap[type1].kEi;
    //     e2 = q1 * pow(pow(r21.norm(), 2) + r_soft, 1.5) * r21;
    // }
    // else
    {
        e2 = q1 * r_inverse_2;
    }

    // Biot-Savart law
    b1 = 1 / (c * c) * q2 * v2.cross(-r_inverse_2);
    b2 = 1 / (c * c) * q1 * v1.cross(r_inverse_2);

    return {e1, e2, b1, b2};
}
