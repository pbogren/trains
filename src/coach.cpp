/**
    @file src/coach.cpp
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief The implementation of the Coach class.
*/

#include "coach.h"
#include "vehicle.h"
#include "vehicle_type.h"

namespace pabo::train {

Coach::Coach(int id, int noSeats, bool hasInternet)
    : m_id{id}, m_noSeats{noSeats}, m_hasInternet{hasInternet}
{
}

int Coach::id_() const noexcept
{
    return m_id;
}

Vehicle::Type Coach::type_() const noexcept
{
    return Vehicle::Type::coach;
}

bool Coach::carriesPassengers_() const noexcept
{
    return true;
}

int Coach::numberOfPassengers_() const noexcept
{
    return m_noSeats;
}

bool Coach::hasInternet_() const noexcept
{
    return m_hasInternet;
}

}  // namespace pabo::train
