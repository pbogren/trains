/**
    @file src/electric_locomotive.cpp
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Implementation of the ElectricLocomotive class.
*/

#include "electric_locomotive.h"
#include "vehicle.h"
#include "vehicle_type.h"
#include <string>

namespace pabo::train {

using namespace std::string_literals;

using EL = ElectricLocomotive;
using Speed = EL::Speed;
using Power = EL::Power;

EL::ElectricLocomotive(const int id, const double maxSpd, const double maxPwr)
    : m_id{id}
    , m_maxSpd{maxSpd, "kph"s}
    , m_maxPwr{maxPwr, "kW"s}
{
}

Vehicle::Type EL::type_() const noexcept
{
    return Vehicle::Type::electricLocomotive;
}

int EL::id_() const noexcept
{
    return m_id;
}

Speed EL::maxSpeed_() const noexcept
{
    return m_maxSpd;
}

Power EL::maxPower_() const noexcept
{
    return m_maxPwr;
}

}  // namespace pabo::train
