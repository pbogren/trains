/**
    @file src/covered_freight_car.cpp
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Implementation of the CoveredFreightCar class.
*/
#include "covered_freight_car.h"
#include "vehicle_type.h"
#include "vehicle.h"
#include <string>

namespace pabo::train {

using namespace std::string_literals;

using CFC = CoveredFreightCar;
using Volume = CFC::Volume;

CFC::CoveredFreightCar(int id, double cargoVolume)
    : m_id{id}
    , m_capacity{cargoVolume, "m3"s}
{
}

Vehicle::Type CFC::type_() const noexcept
{
    return Vehicle::Type::coveredFreightCar;
}

int CFC::id_() const noexcept
{
    return m_id;
}

bool CFC::carriesCargo_() const noexcept
{
    return true;
}

Volume CFC::cargoCapacity_() const noexcept
{
    return m_capacity;
}

}  // namespace pabo::train
