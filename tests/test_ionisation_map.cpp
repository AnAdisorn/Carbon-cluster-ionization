#include "parameters.h"
#include <iostream>

// Test if the reference of the parameters has the same parameters as by copying the parameters
int main()
{
    std::vector<std::string> type_list = {"C0", "C1", "C2", "C3", "C4", "C5"};
    for (std::string type : type_list)
    {
        IonisationParameters &params_copy = IonisationParametersMap[type];
        IonisationParameters &params_ref = IonisationParametersMap[type];
        if (!(params_copy.kCnl_sqr == params_ref.kCnl_sqr && params_copy.kFlm == params_ref.kFlm && params_copy.kN_star == params_ref.kN_star))
        {
            return -1;
        }
    }
    return 0;
}