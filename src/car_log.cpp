/**
    @file src/car_log.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The implementation of the CarLog class.
*/

#include "car_log.h"
#include "vehicle.h"
#include <stdexcept>  // out_of_range
#include <utility>  // move

namespace pabo::train {

CarRecord::CarRecord(time::TimeOfDay tod, const int nbr, std::string stnName)
    : time{tod}, trainNbr{nbr}, destination{std::move(stnName)}
{
}

void CarLog::logArrival(time::TimeOfDay tod, const Train& t, const std::string& stnName)
{
    for (const auto& car: t.attachedCars()) {
        auto& hist = m_history[car->id()];
        hist.emplace_back(std::move(tod), t.number(), stnName);
    }
}

std::vector<CarRecord> CarLog::viewRecordOf(const int carId)
{
    const auto car = m_history.find(carId);
    if (car == m_history.end()) {
        throw std::out_of_range("No such car: " + std::to_string(carId));
    }
    return car->second;
}


}  // namespace pabo::train
