/**
    @file include/vehicle_type.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Definition of the VechicleType enum.

    The enum definition is lifted outside of the Vehicle definition
    so that users can create and modify types without modifying the
    base class.
*/

#ifndef INCLUDE_VEHICLE_TYPE_H
#define INCLUDE_VEHICLE_TYPE_H

#include "vehicle.h"
#include <iosfwd>
#include <string>

namespace pabo::train {

enum class Vehicle::Type { coach,
                           sleepingCar,
                           openFreightCar,
                           coveredFreightCar,
                           electricLocomotive,
                           dieselLocomotive };

std::string typeAsString(Vehicle::Type t);

std::ostream& operator<<(std::ostream&, Vehicle::Type);
std::istream& operator>>(std::istream&, Vehicle::Type&);


}  // namespace pabo::train

#endif
