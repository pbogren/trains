/**
    @file src/station.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Implementation of the Station class.
*/

#include "station.h"
#include "string_funcs.h"  // replaceCharWithSpace
#include "vehicle_factory.h"  // makeVehicle
#include <algorithm>  // any_of, find_if
#include <iterator>  // begin, end
#include <sstream>  // istringstream
#include <stdexcept>
#include <string>
#include <utility>  // pair

namespace pabo::train {

// Add cars to the station using car-data in the istream.
void addCarsFromStream(std::istream& is, Station& s);
Station::Car makeCarFromString(std::string);
// Extract and return the parameters from the string.
std::pair<Station::CarType, Params> paramsFromString(const std::string& s);

Station::Station(std::string name)
    : m_name{std::move(name)}
{
    trimWhitespace(m_name);
}

std::string Station::name() const
{
    return m_name;
}

bool Station::hasCar(CarType t) const
{
    const auto isType = IsType(t);
    return std::any_of(m_self.begin(), m_self.end(), isType);
}

bool Station::hasCar(int id) const
{
    const auto hasId = HasId(id);
    return std::any_of(m_self.begin(), m_self.end(), hasId);
}

int Station::carCount() const noexcept
{
    return static_cast<int>(m_self.size());
}

bool Station::isEmpty() const noexcept
{
    return m_self.empty();
}

Station::CarView Station::viewCar(const int id) const
{
    using std::begin;
    using std::end;
    const auto hasId = HasId(id);
     const auto car = std::find_if(begin(m_self), end(m_self), hasId);
     if (car == m_self.end()) {
         throw std::out_of_range("No such vehicle in " + m_name + std::to_string(id));
     }
     return car->get();
}

std::vector<Station::CarView> Station::availableCars() const
{
    auto res = std::vector<CarView>{};
    for (const auto car: m_self) {
        res.push_back(car.get());
    }
    return res;
}

void Station::addCar(Car car)
{
    // The simulation data is static so asserting in debug mode is
    // enough to guarantee the behavior in release mode.
    m_self.emplace_back(std::move(car));
    sortPoolById();
}

auto Station::findCarByType(CarType t)
{
    const auto isType = IsType(t);
    return std::find_if(m_self.begin(), m_self.end(), isType);
}

Station::Car Station::getCar(CarType t)
{
    auto car = findCarByType(t);
    if (car == m_self.end()) {
        throw std::runtime_error("Tried retrieving non existing car!");
    }
    auto res = std::move(*car);
    remove(car);
    return res;
}

void Station::remove(std::vector<Car>::iterator car)
{
    m_self.erase(car);
}

void Station::sortPoolById()
{
    std::sort(m_self.begin(), m_self.end(),
            [](const Car& lhs, const Car& rhs) {
                return lhs->id() < rhs->id();
            });
}

std::istream& operator>>(std::istream& is, Station& station)
{
    auto line = std::string{};
    std::getline(is, line);
    if(line.empty()) {
        is.setstate(std::ios::failbit);
        return is;
    }

    auto iss = std::istringstream{line};
    auto stationName = std::string{};
    std::getline(iss, stationName, '(');

    auto tmp = Station{stationName};
    addCarsFromStream(iss, tmp);

    using std::swap;
    swap(tmp, station);
    return is;
}

void addCarsFromStream(std::istream& is, Station& s)
{
    auto carParams = std::string{};
    while (std::getline(is, carParams, '(')) {
        auto car = makeCarFromString(carParams);
        s.addCar(std::move(car));
    }
}

Station::Car makeCarFromString(std::string s)
{
    pabo::replaceCharWithSpace(s, ')');
    const auto& [type, params] = paramsFromString(s);
    return makeVehicle(type, params);
}

std::pair<Station::CarType, Params> paramsFromString(const std::string& s)
{
    auto iss = std::istringstream{s};
    auto p = pabo::train::Params{};
    iss >> p.id;

    int type;
    iss >> type;

    iss >> p.param0;
    iss >> p.param1;

    return std::make_pair(static_cast<Station::CarType>(type), p);
}


}  // namespace pabo::train
