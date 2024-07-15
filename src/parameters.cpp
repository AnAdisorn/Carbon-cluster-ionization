#include "parameters.h"

std::map<std::string, ParticleParameters>
    ParticleParametersMap{
        {"e-", ParticleParameters(1, -1)},
        {"C0", ParticleParameters(21883, 0)},
        {"C1", ParticleParameters(21882, 1)},
        {"C2", ParticleParameters(21881, 2)},
        {"C3", ParticleParameters(21880, 3)},
        {"C4", ParticleParameters(21879, 4)},
        {"C5", ParticleParameters(21878, 5)},
        {"C6", ParticleParameters(21877, 6)},
    };

std::map<std::pair<std::string, std::string>, PairParameters>
    PairParametersMap{
        {{"C0", "C0"}, PairParameters(1234., 2.91)}};

std::map<std::string, IonisationParameters>
    IonisationParametersMap{
        {"C0", IonisationParameters(0, 2, 1, 0, .415)},
        {"C1", IonisationParameters(1, 2, 1, 1, .897)},
        {"C2", IonisationParameters(2, 2, 0, 0, 1.76)},
        {"C3", IonisationParameters(3, 2, 0, 0, 2.37)},
        {"C4", IonisationParameters(4, 1, 0, 0, 14.4)},
        {"C5", IonisationParameters(5, 1, 0, 0, 18.0)},
    };