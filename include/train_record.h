/**
    @file include/train_record.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief A POD to hold a record of a trains state a given point in time.
*/

#ifndef INCLUDE_TRAIN_RECORD_H
#define INCLUDE_TRAIN_RECORD_H

#include <string>
#include "time_point.h"
#include "capacity.h"
#include <vector>
#include "vehicle_type.h"

namespace pabo::train {

struct TrainRecord {
    using Speed = Capacity<double>;

    int trainNo{0};
    // Using names to represent the stations since that is how they
    // are read. Could be pointers but they would add coupling between
    // train and station.
    std::string origin;
    std::string destination;
    time::TimeOfDay scheduledDeparture;
    time::TimeOfDay scheduledArrival;
    Speed maxSpd{0, ""};
    std::vector<Vehicle::Type> vehicleLayout;

    time::TimeOfDay estimatedDeparture;
    time::TimeOfDay estimatedArrival;
    time::TimeOfDay delay{0};
    Speed currentSpeed{0, "kph"};
};

}  // namespace pabo::train


#endif
