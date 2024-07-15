#include "parameters.h"

std::map<std::string, PaticleParameters>
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

std::map<std::pair<std::string, std::string>, PairParameters>
    PairParametersMap{
        {{"C0", "C0"}, PairParameters(1234., 2.91)}};

std::map<std::string, IonisationParameters>
    IonisationParametersMap{
        {"C0", IonisationParameters(2, 1, 0, 0.415)},
        {"C1", IonisationParameters(2, 1, 1, 0.897)},
        {"C2", IonisationParameters(2, 0, 0, 1.76)},
        {"C3", IonisationParameters(2, 0, 0, 2.37)},
        {"C4", IonisationParameters(1, 0, 0, 14.4)},
        {"C5", IonisationParameters(1, 0, 0, 18.0)},
    };