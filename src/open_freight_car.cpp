/**
    @file src/open_freight_car.cpp
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief The implementation of the OpenFreightCar class.
*/

#include "open_freight_car.h"
#include "vehicle.h"
#include "vehicle_type.h"
#include <string>

namespace pabo::train {

using OFC = OpenFreightCar;
using namespace std::string_literals;
using Area = OFC::Area;
using Weight = OFC::Weight;

OFC::OpenFreightCar(int id, double cargoCapacity, double floorArea)
    : m_id{id}
    , m_cargoCapacity{cargoCapacity, "ton(s)"s}
    , m_floorArea{floorArea, "m2"s}
{
}

Vehicle::Type OFC::type_() const noexcept
{
    return Vehicle::Type::openFreightCar;
}

int OFC::id_() const noexcept
{
    return m_id;
}

bool OFC::carriesCargo_() const noexcept
{
    return true;
}

Weight OFC::cargoCapacity_() const noexcept
{
    return m_cargoCapacity;
}

Area OFC::floorArea_() const noexcept
{
    return m_floorArea;
}

}  // namespace pabo::train
