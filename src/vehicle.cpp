/**
    @file src/vehicle.cpp
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Implementation of the Vehicle abstract base class.
*/

#include "vehicle.h"

namespace pabo::train {

Vehicle::Type Vehicle::type() const noexcept
{
    return type_();
}

int Vehicle::id() const noexcept
{
    return id_();
}

bool Vehicle::hasEngine() const noexcept
{
    return hasEngine_();
}

Vehicle::Speed Vehicle::maxSpeed() const noexcept
{
    return maxSpeed_();
}

Vehicle::Power Vehicle::maxPower() const noexcept
{
    return maxPower_();
}

Vehicle::FlowRate Vehicle::fuelConsumption() const noexcept
{
    return fuelConsumption_();
}

bool Vehicle::carriesPassengers() const noexcept
{
    return carriesPassengers_();
}

int Vehicle::numberOfPassengers() const noexcept
{
    return numberOfPassengers_();
}

bool Vehicle::hasInternet() const noexcept
{
    return hasInternet_();
}

bool Vehicle::carriesCargo() const noexcept
{
    return carriesCargo_();
}

Capacity<double> Vehicle::cargoCapacity() const noexcept
{
    return cargoCapacity_();
}

Vehicle::Area Vehicle::floorArea() const noexcept
{
    return floorArea_();
}

bool operator==(const Vehicle& lhs, const Vehicle& rhs)
{
    return lhs.id() == rhs.id();
}


//
// Comparison objects
//

IsType::IsType(Vehicle::Type t)
    : m_type{t}
{
}

bool IsType::operator()(const std::shared_ptr<Vehicle>& candidate) const
{
    return candidate->type() == m_type;
}

HasId::HasId(int id)
    : m_id{id}
{
}

bool HasId::operator()(const std::shared_ptr<Vehicle>& candidate) const
{
    return candidate->id() == m_id;
}

}  // namespace pabo::train
