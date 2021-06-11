/**
    @file src/vehicle_type.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Implementation of the VehicleType enum functions.
*/

#include "vehicle_type.h"
#include <iostream>
#include <stdexcept>

namespace pabo::train {

std::string typeAsString(Vehicle::Type t)
{
    switch (t) {
    case Vehicle::Type::coach:
        return "Coach";
    case Vehicle::Type::sleepingCar:
        return "Sleeping Car";
    case Vehicle::Type::openFreightCar:
        return "Open Freight Car";
    case Vehicle::Type::coveredFreightCar:
        return "Covered Freight Car";
    case Vehicle::Type::electricLocomotive:
        return "Electric Locomotive";
    case Vehicle::Type::dieselLocomotive:
        return "Diesel Locomotive";
    default:
        const auto type = std::to_string(static_cast<int>(t));
        throw std::invalid_argument("Invalid type: " + type);
    }
}


std::ostream& operator<<(std::ostream& os, Vehicle::Type t)
{
    return os << typeAsString(t);
}

std::istream& operator>>(std::istream& is, Vehicle::Type& t)
{
    int input;
    is >> input;
    t = static_cast<Vehicle::Type>(input);
    return is;
}

}  // namespace pabo::train
