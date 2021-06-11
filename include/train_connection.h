/**
    @file include/train_connection.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The definition of the TrainConnection class.

    A train connection deals with static data for a connection
    between two stations on a given time.
*/
#ifndef INCLUDE_TRAIN_CONNECTION_H
#define INCLUDE_TRAIN_CONNECTION_H

#include "capacity.h"
#include "time_point.h"
#include "vehicle.h"
#include "vehicle_type.h"
#include <iosfwd>
#include <string>
#include <vector>

namespace pabo::train {

class TrainConnection {
public:
    using Speed = Capacity<double>;

    TrainConnection() = default;
    TrainConnection(std::string);

    [[nodiscard]] int trainNbr() const;
    [[nodiscard]] std::string origin() const;
    [[nodiscard]] std::string destination() const;
    [[nodiscard]] time::TimeOfDay departure() const;
    [[nodiscard]] time::TimeOfDay arrival() const;
    [[nodiscard]] Capacity<double> maxSpeed() const;
    [[nodiscard]] std::vector<Vehicle::Type> vehicles() const;

private:
    int m_nbr{0};
    std::string m_origin;
    std::string m_destination;
    time::TimeOfDay m_departure;
    time::TimeOfDay m_arrival;
    Speed m_maxSpeed;
    std::vector<Vehicle::Type> m_layout;
};

std::istream& operator>>(std::istream&, TrainConnection&);


}  // namespace pabo::train

#endif
