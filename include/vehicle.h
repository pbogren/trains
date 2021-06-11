/**
    @file include/vehicle.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Definition of the abstract base class Vehicle
*/

#ifndef INCLUDE_VEHICLE_H
#define INCLUDE_VEHICLE_H

#include "capacity.h"
#include <memory>  // shared_ptr

namespace pabo::train {

class Vehicle {
public:
    using Speed = Capacity<double>;
    using Power = Capacity<double>;
    using FlowRate = Capacity<double>;
    using Volume = Capacity<double>;
    using Area = Capacity<double>;
    using Weight = Capacity<double>;

    enum class Type;

    virtual ~Vehicle() = default;

    // Uses the NVI idiom
    [[nodiscard]] Type type() const noexcept;
    [[nodiscard]] int id() const noexcept;

    //
    // Queries
    //
    [[nodiscard]] bool hasEngine() const noexcept;
    [[nodiscard]] Speed maxSpeed() const noexcept;
    [[nodiscard]] Power maxPower() const noexcept;
    [[nodiscard]] FlowRate fuelConsumption() const noexcept;
    [[nodiscard]] bool carriesPassengers() const noexcept;
    [[nodiscard]] int numberOfPassengers() const noexcept;
    [[nodiscard]] bool hasInternet() const noexcept;
    [[nodiscard]] bool carriesCargo() const noexcept;
    [[nodiscard]] Capacity<double> cargoCapacity() const noexcept;
    [[nodiscard]] Area floorArea() const noexcept;

private:
    [[nodiscard]] virtual Type type_() const noexcept = 0;
    [[nodiscard]] virtual int id_() const noexcept = 0;

    [[nodiscard]] virtual bool hasEngine_() const noexcept = 0;
    [[nodiscard]] virtual Speed maxSpeed_() const noexcept = 0;
    [[nodiscard]] virtual Power maxPower_() const noexcept = 0;
    [[nodiscard]] virtual Power fuelConsumption_() const noexcept = 0;

    [[nodiscard]] virtual bool carriesPassengers_() const noexcept = 0;
    [[nodiscard]] virtual int numberOfPassengers_() const noexcept = 0;
    [[nodiscard]] virtual bool hasInternet_() const noexcept = 0;

    [[nodiscard]] virtual bool carriesCargo_() const noexcept = 0;
    [[nodiscard]] virtual Capacity<double> cargoCapacity_() const noexcept = 0;
    [[nodiscard]] virtual Area floorArea_() const noexcept = 0;
};

// Function object to check if a vehicle matches a type.
class IsType {
public:
    IsType(Vehicle::Type t);
    bool operator()(const std::shared_ptr<Vehicle>& candidate) const;
private:
    Vehicle::Type m_type;
};

// Function object to check if a vehicle matches an id.
class HasId {
public:
    HasId(int id);
    bool operator()(const std::shared_ptr<Vehicle>& candidate) const;

private:
    int m_id;
};

bool operator==(const Vehicle& lhs, const Vehicle& rhs);

}  // namespace pabo::train

#endif
