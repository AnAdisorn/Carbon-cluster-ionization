#include "interactions.h"

PaticleParameters::PaticleParameters(int kM, int kQ) : kM(kM), kQ(kQ) {}

PairParameters::PairParameters(double kEps, double kR0) : kEps(kEps), kR0(kR0) {}

IonisationParameters::IonisationParameters(int kN, int kL, int kM, double kEi) : kN(kN), kL(kL), kM(kM), kEi(kEi) {}

std::unordered_map<std::string, PaticleParameters>
    ParticleParametersMap{
        {"e-", PaticleParameters(1, -1)},
        {"C0", PaticleParameters(21883, 0)},
        {"C1", PaticleParameters(21882, 1)},
        {"C2", PaticleParameters(21881, 2)},
        {"C3", PaticleParameters(21880, 3)},
        {"C4", PaticleParameters(21879, 4)},
        {"C5", PaticleParameters(21878, 5)},
        {"C6", PaticleParameters(21877, 6)},
    };

std::unordered_map<std::pair<std::string, std::string>, PairParameters>
    PairParametersMap{
        {{"C0", "C0"}, PairParameters(1234., 2.91)}};

std::unordered_map<std::string, IonisationParameters>
    IonisationParametersMap{
        {"C0", IonisationParameters(2, 1, 0, 0.415)},
        {"C1", IonisationParameters(2, 1, 1, 0.897)},
        {"C2", IonisationParameters(2, 0, 0, 1.76)},
        {"C3", IonisationParameters(2, 0, 0, 2.37)},
        {"C4", IonisationParameters(1, 0, 0, 14.4)},
        {"C5", IonisationParameters(1, 0, 0, 18.0)},
    };

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