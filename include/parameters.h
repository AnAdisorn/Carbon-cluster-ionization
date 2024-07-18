#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <map>

const double c = 137; // Light speed constant in atomic unit

// Particle parameters that are independent of other particles
struct ParticleParameters
{
    const double kM; // mass
    const double kQ; // charge

    // Constructor
    ParticleParameters() : kM(), kQ() {}
    ParticleParameters(double kM, double kQ) : kM(kM), kQ(kQ) {}
};

// Particle parameters that are dependent of other particles, i.e. Lennard-Jones Potential
struct PairParameters
{
    const double kEps;
    const double kR0;

    // Constructor
    PairParameters() : kEps(), kR0() {}
    PairParameters(double kEps, double kR0) : kEps(kEps), kR0(kR0) {}
};

// Particle parameters for ionisation
struct IonisationParameters
{
    const int kZ, kN, kL, kM;
    const double kEi;
    double kN_star, kCnl_sqr, kFlm;

    // Constructor
    IonisationParameters() : kZ(), kN(), kL(), kM(), kEi(), kN_star(), kCnl_sqr(), kFlm() {}
    IonisationParameters(int kZ, int kN, int kL, int kM, double kEi) : kZ(kZ), kN(kN), kL(kL), kM(kM), kEi(kEi)
    {
        kN_star = kZ / sqrt(2 * kEi);
        double l_star = kN_star - 1;
        kCnl_sqr = pow(2, 2 * kN_star) / (kN_star * tgamma(kN_star + l_star + 1) * tgamma(kN_star - l_star));
        kFlm = (2 * kL + 1) * tgamma(kL + abs(kM) + 1) / (pow(2, abs(kM)) * tgamma(abs(kM) + 1) * tgamma(kL - abs(kM) + 1));
    }
};

// Map particle name to ParticleParameters
extern std::map<std::string, ParticleParameters>
    ParticleParametersMap;

// Map pair of particle names to PairParameters
extern std::map<std::pair<std::string, std::string>, PairParameters>
    PairParametersMap;

// Map particle name to IonisationParameters
extern std::map<std::string, IonisationParameters>
    IonisationParametersMap;

// Function to create sorted pair of strings
inline std::pair<std::string, std::string> createPair(std::string str1, std::string str2)
{
    if (str1.compare(str2) < 0)
    {
        return {str1, str2};
    }
    else
    {
        return {str2, str1};
    }
}

inline std::string lowerType(std::string type)
{
    std::string type_prefix = type.substr(0, 1);
    int type_suffix = std::stoi(type.substr(1, 2));

    return type_prefix + std::to_string(type_suffix - 1);
}
#endif /* end of include guard: PARAMETERS_H */