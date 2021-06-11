/**
    @file include/station.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief The definition of the Station class.
*/
#ifndef INCLUDE_STATION_H
#define INCLUDE_STATION_H

#include "vehicle.h"
#include "vehicle_type.h"
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace pabo::train {

class Station {
public:
    using Car = std::shared_ptr<Vehicle>;
    using CarType = Vehicle::Type;
    // A CarView is a read-only view of a Car;
    using CarView = const Vehicle*;

    //
    // Construct
    //
    Station() = default;
    explicit Station(std::string name);

    //
    // Queries
    //
    [[nodiscard]] std::string name() const;
    [[nodiscard]] bool hasCar(CarType) const;
    [[nodiscard]] bool hasCar(int id) const;
    [[nodiscard]] int carCount() const noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    // Returns a read only view of the car with given id.
    // Throws std::out_of_range if no such car exists in the pool
    [[nodiscard]] CarView viewCar(int id) const;
    // Returns a vector of the cars in the pool.
    [[nodiscard]] std::vector<CarView> availableCars() const;

    //
    // Commands
    //

    // Add a car to the pool.
    void addCar(Car);
    // Returns a Car of type CarType.
    // Throws std::out_of_range if no such car exists in the pool
    [[nodiscard]] Car getCar(CarType);

private:
    [[nodiscard]] auto findCarByType(CarType);
    void remove(std::vector<Car>::iterator);
    void sortPoolById();

    std::string m_name;
    std::vector<Car> m_self;
};

std::istream& operator>>(std::istream&, Station&);

}  // namespace pabo::train

#endif
