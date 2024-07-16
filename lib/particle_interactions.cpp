#include "particle_interactions.h"

double calculateLJPotential(const std::string type1, const std::string type2, const Eigen::Vector3d &r1, const Eigen::Vector3d &r2)
{
    Eigen::Vector3d r21 = r2 - r1;
    double r21_norm = r21.norm();

    std::pair<std::string, std::string> pair = createPair(type1, type2);
    PairParameters *pair_params = &PairParametersMap[pair];

    double r_ratio = pair_params->kR0 / r21_norm;

    return pair_params->kEps * (pow(r_ratio, 12) - 2 * pow(r_ratio, 6));
}

Vector3d calculateLJForce(const std::string type1, const std::string type2, const Vector3d &r1, const Vector3d &r2)
{
    Eigen::Vector3d r21 = r2 - r1;
    double r21_norm = r21.norm();
    r21.normalized();

    std::pair<std::string, std::string> pair = createPair(type1, type2);
    PairParameters *pair_params = &PairParametersMap[pair];

    double r_ratio = pair_params->kR0 / r21_norm;

    return 12 * pair_params->kEps / r21_norm * (pow(r_ratio, 12) - pow(r_ratio, 6)) * r21;
}

std::array<Vector3d, 2> movePositionLJ(const std::string type1, const std::string type2, const Vector3d &r1, const Vector3d &r2, double dt)
{
    const double m1 = ParticleParametersMap[type1].kM;
    const double m2 = ParticleParametersMap[type2].kM;
    Vector3d f_dt = calculateLJForce(type1, type2, r1, r2) * dt;
    return {-f_dt / m1, f_dt / m2};
}