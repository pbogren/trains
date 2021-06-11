#include "sleeping_car.h"
#include "vehicle.h"
#include "vehicle_type.h"

namespace pabo::train {

using SC = SleepingCar;

SC::SleepingCar(int id, int noBeds)
    : m_id{id}, m_noBeds{noBeds}
{
}

Vehicle::Type SC::type_() const noexcept
{
    return Vehicle::Type::sleepingCar;
}

int SC::id_() const noexcept
{
    return m_id;
}

bool SC::carriesPassengers_() const noexcept
{
    return true;
}

int SC::numberOfPassengers_() const noexcept
{
    return m_noBeds;
}

}  // namespace pabo::train
