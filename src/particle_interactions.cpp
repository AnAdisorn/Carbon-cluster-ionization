#include "particle_interactions.h"

double calculateLJPotential(const std::string type1, const std::string type2, const Eigen::Vector3d &r1, const Eigen::Vector3d &r2)
{
    Eigen::Vector3d r21 = r2 - r1;
    double r21_norm = r21.norm();

    std::pair<std::string, std::string> pair = createPair(type1, type2);
    PairParameters pair_params = PairParametersMap[pair];

    double r_ratio = pair_params.kR0 / r21_norm;

    return pair_params.kR0 * (pow(r_ratio, 12) - 2 * pow(r_ratio, 6));
}

Eigen::Vector3d calculateLJForce(const std::string type1, const std::string type2, const Eigen::Vector3d &r1, const Eigen::Vector3d &r2)
{
    Eigen::Vector3d r21 = r2 - r1;
    double r21_norm = r21.norm();
    r21.normalized();

    std::pair<std::string, std::string> pair = createPair(type1, type2);
    PairParameters pair_params = PairParametersMap[pair];

    double r_ratio = pair_params.kR0 / r21_norm;

    return 12 * pair_params.kEps / r21_norm * (pow(r_ratio, 12) - pow(r_ratio, 6)) * r21;
}