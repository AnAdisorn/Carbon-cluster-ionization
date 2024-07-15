#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <map>

// Particle parameters that are independent of other particles
struct PaticleParameters
{
    const int kM; // mass
    const int kQ; // charge

    // Constructor
    PaticleParameters() : kM(), kQ() {}
    PaticleParameters(int kM, int kQ) : kM(kM), kQ(kQ) {}
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
    const int kN, kL, kM;
    const double kEi;

    // Constructor
    IonisationParameters() : kN(), kL(), kM(), kEi() {}
    IonisationParameters(int kN, int kL, int kM, double kEi) : kN(kN), kL(kL), kM(kM), kEi(kEi) {}
};

// Map particle name to ParticleParameters
extern std::map<std::string, PaticleParameters>
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

#endif /* end of include guard: PARAMETERS_H */