#include "diesel_locomotive.h"
#include "vehicle.h"
#include "vehicle_type.h"
#include <string>

namespace pabo::train {

using namespace std::string_literals;

using DL = DieselLocomotive;
using Speed = DL::Speed;
using FlowRate = DL::FlowRate;

DL::DieselLocomotive(int id, double maxSpd, double fuelConsumption)
    : m_id{id}
    , m_maxSpd{maxSpd, "kph"s}
    , m_fuelConsumption{fuelConsumption, "lph"s}
{
}

Vehicle::Type DL::type_() const noexcept
{
    return Vehicle::Type::dieselLocomotive;
}

int DL::id_() const noexcept
{
    return m_id;
}

Speed DL::maxSpeed_() const noexcept
{
    return m_maxSpd;
}

FlowRate DL::fuelConsumption_() const noexcept
{
    return m_fuelConsumption;
}

}  // namespace pabo::train
