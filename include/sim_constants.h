/**
    @file include/sim_constants.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Constants used by the simulator
*/
#ifndef INCLUDE_SIM_CONSTANTS_H
#define INCLUDE_SIM_CONSTANTS_H

#include <string>
#include "time_point.h"

namespace pabo::train::constants {
using Duration = time::TimeOfDay;

const auto timeBetweenDepartureAndAssembly = Duration{"00:30"};
const auto timeBetweenAssembledAndReady = Duration{"00:10"};
const auto timeBetweenAssemblyAttempts = Duration{"00:10"};
const auto timeBetweenReadyAndDeparture = Duration{"00:10"};
const auto timeBetweenArrivalAndDisassembly = Duration{"00:20"};

}  // namespace pabo::train::constants


#endif
