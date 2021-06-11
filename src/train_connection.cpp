/**
    @file src/train_connection.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The implementation of the TrainConnection class
*/


#include "train_connection.h"
#include <string>
#include <sstream>
#include <utility>  // swap

namespace pabo::train {

using TC = TrainConnection;

TC::TrainConnection(std::string str)
{
    auto iss = std::istringstream{str};
    iss >> m_nbr >> m_origin >> m_destination >> m_departure >> m_arrival;

    double spd;
    iss >> spd;
    m_maxSpeed = Speed(spd, "kph");

    Vehicle::Type type;
    while (iss >> type) {
        m_layout.push_back(type);
    }
}

int TC::trainNbr() const
{
    return m_nbr;
}

std::string TC::origin() const
{
    return m_origin;
}

std::string TC::destination() const
{
    return m_destination;
}

time::TimeOfDay TC::departure() const
{
    return m_departure;
}

time::TimeOfDay TC::arrival() const
{
    return m_arrival;
}

TC::Speed TC::maxSpeed() const
{
    return m_maxSpeed;
}

std::vector<Vehicle::Type> TC::vehicles() const
{
    return m_layout;
}


std::istream& operator>>(std::istream& is, TrainConnection& conn)
{
    auto line = std::string{};
    std::getline(is, line);
    if (line.empty()) {
        is.setstate(std::ios::failbit);
        return is;
    }

    auto tmp = TrainConnection{line};
    using std::swap;
    swap(tmp, conn);
    return is;
}

}  // namespace pabo::train
