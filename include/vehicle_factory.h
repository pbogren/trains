/**
    @file include/vehicle_factory.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Functions to create vehicles.
*/

#ifndef INCLUDE_VEHICLE_FACTORY_H
#define INCLUDE_VEHICLE_FACTORY_H

#include "vehicle.h"
#include "vehicle_type.h"
#include <memory>  // unique_ptr

namespace pabo::train {

struct Params {
    int id{0};
    double param0{0};
    double param1{0};
};

// Return a vehicle of type t initialized with the parameters in p.
std::unique_ptr<Vehicle> makeVehicle(Vehicle::Type t, Params p);

}  // namespace pabo::train

#endif
