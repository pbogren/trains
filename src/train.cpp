#include "train.h"
#include "time_point.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>  // move
#include <limits>

namespace pabo::train {

//
// Private helpers
//

bool isEmptyAndType(const Train::Slot& s, Train::CarType match)
{
    const auto& [type, car] = s;
    return type == match && !car;
}

Train::State Train::defaultState()
{
    return Train::State::not_assembled;
}

//
// Construct Slot

Train::Slot::Slot(Train::CarType t, Train::Car c)
    : type{t}, car{std::move(c)}
{
}

//
// Construct Train
//

Train::Train(const TrainConnection& tc)
    : m_nbr{tc.trainNbr()}
    , m_departure{tc.departure()}
{
    for (const auto type: tc.vehicles()) {
        m_self.emplace_back(type, nullptr);
    }
}

//
// Queries
//

int Train::number() const noexcept
{
    return m_nbr;
}

time::TimeOfDay Train::departure() const noexcept
{
    return m_departure;
}

Train::State Train::state() const noexcept
{
    return m_state;
}

bool Train::isAssembled() const noexcept
{
    return std::none_of(m_self.begin(), m_self.end(), [](const Slot& s) {
        const auto& [type, car] = s;
        return !car;
    });
}

bool Train::hasDeparted() const noexcept
{
    return state() >= State::running;
}

std::vector<Train::CarType> Train::missingCarTypes() const
{
    auto res = std::vector<CarType>{};
    for (const auto& [type, car]: m_self) {
        if (!car) { res.push_back(type); }
    }
    return res;
}

bool Train::missesCarOfType(CarType match) const
{
    return std::any_of(m_self.begin(), m_self.end(),
                       [match](const Slot& s) {
                           return isEmptyAndType(s, match);
                       });
}

bool Train::hasCar(int id) const
{
    return std::any_of(m_self.begin(), m_self.end(), [id](const Slot& s) {
        const auto& [type, car] = s;
        if (!car) { return false; }
        return car->id() == id;
    });
}

//
// Commands
//
void Train::delayDeparture(const int minutes)
{
    m_departure.addMinutes(minutes);
}

void Train::attachCar(Car c)
{
    auto& [type, car] = findEmptySlotByType(c->type());
    car = std::move(c);
}

Train::Slot& Train::findEmptySlotByType(CarType match)
{
    auto slot = std::find_if(m_self.begin(), m_self.end(),
                             [match](const Slot& s) {
                                 return isEmptyAndType(s, match);
                             });

    if (slot == m_self.end()) {
        throw std::runtime_error("Tried finding type that was not missing");
    }
    return *slot;
}

void Train::setState(const State s) noexcept
{
    m_state = s;
}

Train::CarView Train::viewCar(int id) const
{
    auto slot = std::find_if(m_self.begin(), m_self.end(), [id](const Slot& s) {
        const auto& [type, car] = s;
        if (!car) { return false; }
        return car->id() == id;
    });
    if (slot == m_self.end()) {
        throw std::runtime_error("Tried retrieving non-existing car!");
    }
    const auto& [type, car] = *slot;
    return car.get();
}

std::vector<Train::CarView> Train::attachedCars() const
{
    auto res = std::vector<CarView>{};
    std::for_each(m_self.begin(), m_self.end(), [&res](const Slot& s) {
        const auto& [type, car] = s;
        if (car) {
            res.push_back(car.get());
        }
    });
    return res;
}

Train::Speed Train::maxSpeed() const
{
    const auto sentry = std::numeric_limits<double>::max();
    auto res = Speed{sentry, "kph"};
    for (const auto& [type, car] : m_self) {
        if (car && car->hasEngine()) {
            res = std::min(res, car->maxSpeed());
        }
    }
    if (res == sentry) { res = {0, "kph"}; }
    return res;
}

Train::Speed Train::currentSpeed() const
{
    return m_currentSpd;
}


void Train::setSpeed(const Speed& spd)
{
    if (maxSpeed() < spd) {
        throw std::runtime_error("Attempted to set speed above speed limit!");
    }
    m_currentSpd = Speed{spd.value, "kph"};
}

void Train::setDepartureDelay(Duration delay)
{
    m_departureDelay = std::move(delay);
}

void Train::setArrivalDelay(Duration delay)
{
    m_arrivalDelay = std::move(delay);
}

Train::Duration Train::departureDelay() const
{
    return m_departureDelay;
}

Train::Duration Train::arrivalDelay() const
{
    return m_arrivalDelay;
}

std::vector<Train::Car> Train::disassemble()
{
    auto res = std::vector<Car>{};
    for (auto& slot : m_self) {
        auto& [type, car] = slot;
        if (car) {
            res.emplace_back(std::move(car));
        }
    }
    setState(State::not_assembled);
    return std::move(res);
}

//
// Non-members
//

std::string toString(const Train::State s)
{
    switch (s) {
    case Train::State::not_assembled:
        return "NOT ASSEMBLED";
    case Train::State::assembled:
        return "ASSEMBLED";
    case Train::State::incomplete:
        return "INCOMPLETE";
    case Train::State::ready:
        return "READY";
    case Train::State::running:
        return "RUNNING";
    case Train::State::arrived:
        return "ARRIVED";
    case Train::State::finished:
        return "FINISHED";
    default:
        throw std::runtime_error("Invalid state!");
    }
}

std::ostream& operator<<(std::ostream& os, Train::State state)
{
    const auto string = toString(state);
    return os << string;
}

}  // namespace pabo::train
