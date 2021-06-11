/**
    @file src/train_dispatcher.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Implementation of the TrainDispatcher class.
*/

#include "train_dispatcher.h"
#include <algorithm>  // find_if
#include <cassert>
#include <iterator>  // begin, end
#include <numeric>  // accumulate
#include <stdexcept>  // out_of_range
#include <string>
#include <utility>  // move
#include <vector>

namespace pabo::train {

using namespace std::string_literals;

using TD = TrainDispatcher;
using TrainView = TrainDispatcher::TrainView;
using TrainObj = TrainDispatcher::TrainObj;
using Duration = TrainDispatcher::Duration;

TD::TrainDispatcher(std::vector<ConnObj> connections,
                    std::vector<StationObj> stns,
                    std::vector<PathObj> map)
    : m_connections{std::move(connections)}
    , m_stations{std::move(stns)}
    , m_map{std::move(map)}
{
    for (const auto& c: m_connections) {
        m_trains.emplace_back(c);
    }
}


//
// Time queries
//

time::TimeOfDay TD::estimatedTimeOfDeparture(const int nbr) const
{
    const auto train = findTrainByNbr(nbr);
    return train->departure();
}

time::TimeOfDay TD::estimatedTimeOfArrival(const int nbr) const
{
    auto train = findTrainByNbr(nbr);
    const auto conn = findConnectionByNbr(nbr);
    return conn->arrival() + currentDelay(*train);
}

time::TimeOfDay TD::estimatedTimeOfArrival(const Train& train) const
{
    const auto conn = findConnectionByNbr(train.number());
    return conn->arrival() + currentDelay(train);
}

time::TimeOfDay TD::scheduledTimeOfDeparture(const int nbr) const
{
    const auto conn = findConnectionByNbr(nbr);
    return conn->departure();
}

time::TimeOfDay TD::scheduledTimeOfArrival(const int nbr) const
{
    const auto conn = findConnectionByNbr(nbr);
    return conn->arrival();
}

time::TimeOfDay TD::currentDelay(const int nbr) const
{
    const auto train = findTrainByNbr(nbr);
    return currentDelay(*train);
}

time::TimeOfDay TD::currentDelay(const Train& t) const
{
    if (t.hasDeparted()) {
        return calculateDelayOfRunningTrain(t);
    }
    return calculateDelayOfStaticTrain(t);
}

time::TimeOfDay TD::departureDelay(const Train& t) const
{
    return calculateDelayOfStaticTrain(t);
}

time::TimeOfDay TD::arrivalDelay(const Train& t) const
{
    if (t.state() < Train::State::running) {
        return departureDelay(t);
    }
    assert(t.currentSpeed() > 0.0);
    return calculateDelayOfRunningTrain(t);
}

//
// Train queries
//

std::vector<int> TD::trainNumbers() const
{
    auto res = std::vector<int>{};
    for (const auto& train: m_trains) {
        res.push_back(train.number());
    }
    return res;
}

std::vector<int> TD::delayedTrainNumbers() const
{
    auto res = std::vector<int>{};
    for (const auto& train: m_trains) {
        if (train.arrivalDelay() > Duration{0}) {
            res.push_back(train.number());
        }
    }
    return res;
}

std::vector<int> TD::nonDepartedTrainNumbers() const
{
    auto res = std::vector<int>{};
    for (const auto& train: m_trains) {
        if (train.state() <= Train::State::ready) {
            res.push_back(train.number());
        }
    }
    return res;
}

TrainView TD::viewTrain(const int nbr) const
{
    const auto train = findTrainByNbr(nbr);
    return *train;
}

bool TD::trainIsAssembled(const int nbr) const
{
    const auto train = findTrainByNbr(nbr);
    return train->isAssembled();
}

bool TD::allTrainsFinished() const
{
    using std::begin;
    using std::end;
    return std::all_of(begin(m_trains), end(m_trains),
                       [](const TrainObj& t) {
                           return t.state() == Train::State::finished;
                       });
}

TD::Duration TD::totalDepartureDelay() const
{
    using std::begin;
    using std::end;
    const auto delay = std::accumulate(begin(m_trains), end(m_trains), Duration{},
                                       [this](Duration current, const TrainObj& t) {
                                           auto delayToAdd = t.departureDelay();
                                           return std::move(current) + std::move(delayToAdd);
                                       });
    return delay;
}

TD::Duration TD::totalArrivalDelay() const
{
    using std::begin;
    using std::end;
    const auto delay = std::accumulate(begin(m_trains), end(m_trains), Duration{},
                                       [this](Duration current, const TrainObj& t) {
                                           auto delayToAdd = t.arrivalDelay();
                                           return std::move(current) + std::move(delayToAdd);
                                       });
    return delay;
}

Train::Speed TD::maxSpeed(const int nbr) const
{
    const auto conn = findConnectionByNbr(nbr);
    const auto train = findTrainByNbr(nbr);
    return std::min(conn->maxSpeed(), train->maxSpeed());
}

std::string TD::trainLocation(const Train& t) const
{
    if (t.state() < Train::State::running) {
        return origin(t.number());
    }
    if (t.state() > Train::State::running) {
        return destination(t.number());
    }
    return "In transit";
}

TrainView TD::viewTrainByVehicleId(const int id) const
{
    using std::begin;
    using std::end;
    const auto train = std::find_if(begin(m_trains), end(m_trains),
                                    [id](const TrainObj& t) {
                                        return t.hasCar(id);
                                    });
    if (train == m_trains.end()) {
        const auto idStr = std::to_string(id);
        throw std::out_of_range("Vehicle is not in any train:"s + idStr);
    }
    return *train;
}

//
// Connection queries
//

TD::Distance TD::distance(int nbr) const
{
    return findDistance(nbr);
}

std::string TD::origin(int nbr) const
{
    const auto conn = findConnectionByNbr(nbr);
    return conn->origin();
}

std::string TD::destination(int nbr) const
{
    const auto conn = findConnectionByNbr(nbr);
    return conn->destination();
}

//
// Station queries
//
std::vector<std::string> TD::stationNames() const
{
    auto res = std::vector<std::string>{};
    res.reserve(m_stations.size());
    for (const auto stn: m_stations) {
        res.emplace_back(stn.name());
    }
    return res;
}

TD::StationView TD::viewStation(const std::string& name) const
{
    const auto station = findStationByName(name);
    if (station == m_stations.end()) {
        throw std::out_of_range("Station does not exist: " + name);
    }
    return *station;
}

std::vector<const Train*> TD::trainsAtStation(const std::string& name)
{
    auto res = std::vector<const Train*>{};
    for (const auto& train: m_trains) {
        if (trainLocation(train) == name) {
            res.push_back(&train);
        }
    }
    return res;
}

//
// Car queries
//

auto TD::findCarById(const int id) const
{
    using std::begin;
    using std::end;
    const auto train = std::find_if(begin(m_trains), end(m_trains),
                                    [id](const TrainObj& t) { return t.hasCar(id); });
    if (train != m_trains.end()) {
        return train->viewCar(id);
    }

    const auto station = std::find_if(begin(m_stations), end(m_stations),
                                      [id](const StationObj& s) { return s.hasCar(id); });
    if (station != m_stations.end()) {
        return station->viewCar(id);
    }
    const auto idStr = std::to_string(id);
    throw std::out_of_range("Can not find car with id: " + idStr);
}

TD::CarView TD::viewCar(const int id) const
{
    const auto car = findCarById(id);
    return car;
}

std::string TD::carLocation(const int id) const
{
    for (const auto& train: m_trains) {
        if (train.hasCar(id)) {
            const auto nbr = std::to_string(train.number());
            return "Train " + nbr;
        }
    }

    for (const auto& stn: m_stations) {
        if (stn.hasCar(id)) {
            return stn.name();
        }
    }

    throw std::out_of_range("No vehicle exists with id: " + std::to_string(id));
}

std::vector<TD::CarView> TD::viewAllCars() const
{
    auto res = std::vector<CarView>{};
    const auto estSize = m_trains.size() * 5 + m_stations.size() * 120;
    res.reserve(estSize);

    using std::begin;
    using std::end;
    for (const auto train: m_trains) {
        const auto cars = train.attachedCars();
        res.insert(end(res), begin(cars), end(cars));
    }

    for (const auto stn: m_stations) {
        const auto cars = stn.availableCars();
        res.insert(end(res), begin(cars), end(cars));
    }

    return res;
}

//
// Commands
//

void TD::tryAssembleTrain(const int nbr)
{
    auto conn = findConnectionByNbr(nbr);
    auto station = findStationByName(conn->origin());
    auto train = findTrainByNbr(nbr);
    for (const auto type: train->missingCarTypes()) {
        if (station->hasCar(type)) {
            auto car = station->getCar(type);
            train->attachCar(std::move(car));
        }
    }
}

void TD::disassembleTrain(const int nbr)
{
    auto conn = findConnectionByNbr(nbr);
    auto station = findStationByName(conn->destination());
    auto train = findTrainByNbr(nbr);
    for (auto car: train->disassemble()) {
        station->addCar(std::move(car));
    }
}

void TD::setStateOfTrain(const int nbr, Train::State s)
{
    const auto train = findTrainByNbr(nbr);
    train->setState(s);
}

void TD::delayDeparture(int nbr, time::TimeOfDay delay)
{
    auto train = findTrainByNbr(nbr);
    assert(train != m_trains.end());
    train->delayDeparture(delay.inMinutes());
}

void TD::setDepartureDelay(const int nbr)
{
    auto train = findTrainByNbr(nbr);
    assert(train != m_trains.end());
    const auto delay = calculateDelayOfStaticTrain(*train);
    train->setDepartureDelay(delay);
}

void TD::setArrivalDelay(const int nbr)
{
    auto train = findTrainByNbr(nbr);
    assert(train != m_trains.end());
    const auto delay = calculateDelayOfRunningTrain(*train);
    train->setArrivalDelay(delay);
}

void TD::setOptimalSpeedOfTrain(const int nbr)
{
    const auto train = findTrainByNbr(nbr);
    const auto dist = findDistance(nbr);
    const auto maxSpd = maxSpeed(nbr);

    const auto minTravelTime = calcTravelTime(dist, maxSpd);

    const auto schedArrival = scheduledTimeOfArrival(nbr);
    const auto departure = train->departure();

    const auto optimalTravelTime = (departure > schedArrival)
                                           ? time::TimeOfDay{0}
                                           : schedArrival - departure;

    if (minTravelTime >= optimalTravelTime) {
        train->setSpeed(maxSpd);
        return;
    }
    const auto optimalSpeed = calcSpeed(dist, optimalTravelTime);
    train->setSpeed(optimalSpeed);
}

//
// Calculations
//

time::TimeOfDay TD::calculateDelayOfStaticTrain(const Train& t) const
{
    const auto nbr = t.number();
    return t.departure() - scheduledTimeOfDeparture(nbr);
}

time::TimeOfDay TD::calculateDelayOfRunningTrain(const Train& t) const
{
    const auto nbr = t.number();
    const auto travelTime = calcTravelTime(distance(nbr), t.currentSpeed());
    const auto actualArrival = t.departure() + travelTime;
    return actualArrival - scheduledTimeOfArrival(nbr);
}

//
// Find operations
//

auto TD::findTrainByNbr(const int nbr) const
        -> std::vector<TrainObj>::const_iterator
{
    using std::begin;
    using std::end;
    const auto train = std::find_if(begin(m_trains), end(m_trains),
                                    [nbr](const TrainObj& t) {
                                        return t.number() == nbr;
                                    });
    if (train == m_trains.end()) {
        throw std::out_of_range("Train does not exist: " + std::to_string(nbr));
    }
    return train;
}

auto TD::findTrainByNbr(const int nbr) -> std::vector<TrainObj>::iterator
{
    using std::begin;
    using std::end;
    const auto train = std::find_if(begin(m_trains), end(m_trains),
                                    [nbr](const TrainObj& t) {
                                        return t.number() == nbr;
                                    });
    if (train == m_trains.end()) {
        throw std::out_of_range("Train does not exist: " + std::to_string(nbr));
    }
    return train;
}

auto TD::findConnectionByNbr(int nbr) const -> std::vector<ConnObj>::const_iterator
{
    using std::begin;
    using std::end;
    const auto conn = std::find_if(begin(m_connections), end(m_connections),
                                   [nbr](const ConnObj& c) {
                                       return c.trainNbr() == nbr;
                                   });
    if (conn == m_connections.end()) {
        throw std::out_of_range("Connection does not exist: " + std::to_string(nbr));
    }
    return conn;
}

auto TD::findConnectionByNbr(int nbr) -> std::vector<ConnObj>::iterator
{
    using std::begin;
    using std::end;
    const auto conn = std::find_if(begin(m_connections), end(m_connections),
                                   [nbr](const ConnObj& c) {
                                       return c.trainNbr() == nbr;
                                   });
    if (conn == m_connections.end()) {
        throw std::out_of_range("Connection does not exist: " + std::to_string(nbr));
    }
    return conn;
}

auto TD::findStationByName(const std::string& name) -> std::vector<StationObj>::iterator
{
    using std::begin;
    using std::end;
    const auto station = std::find_if(begin(m_stations), end(m_stations),
                                      [&name](const StationObj& s) {
                                          return s.name() == name;
                                      });
    if (station == m_stations.end()) {
        throw std::out_of_range("Station does not exist: " + name);
    }
    return station;
}

auto TD::findStationByName(const std::string& name) const -> std::vector<StationObj>::const_iterator
{
    using std::begin;
    using std::end;
    const auto station = std::find_if(begin(m_stations), end(m_stations),
                                      [&name](const StationObj& s) {
                                          return s.name() == name;
                                      });
    if (station == m_stations.end()) {
        throw std::out_of_range("Station does not exist: " + name);
    }
    return station;
}

TD::Distance TD::findDistance(int nbr) const
{
    const auto& conn = findConnectionByNbr(nbr);
    return findDistance(conn->origin(), conn->destination());
}

Capacity<double> TD::findDistance(std::string station1,
                                  std::string station2) const
{
    using std::begin;
    using std::end;
    const auto match = Path{std::move(station1), std::move(station2)};
    auto path = std::find_if(begin(m_map), end(m_map),
                             [&match](const PathObj& p) { return p == match; });
    return path->distance();
}

//
// Non-members
//

Duration calcTravelTime(const TrainDispatcher::Distance& dist,
                        const Train::Speed& spd)
{
    const auto travelTime = (dist.value / spd.value * 60.0);
    return Duration{travelTime};
}

TD::Speed calcSpeed(const TD::Distance& d, const Duration& travelTime)
{
    return TD::Speed{d.value / travelTime.inHours(), "kph"};
}


}  // namespace pabo::train
