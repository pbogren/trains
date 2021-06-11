/**
    @file include/locomotive.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief The definition of the abstract class Locomotive

    This class strives to make it easier to create subclasses by
    providing default implementations for some of the virtual functions
    declared in the base class.
*/

#ifndef INCLUDE_LOCOMOTIVE_H
#define INCLUDE_LOCOMOTIVE_H

#include "vehicle.h"

namespace pabo::train {

class Locomotive : public Vehicle {
public:
    using Speed = Vehicle::Speed;
    using Power = Vehicle::Power;
    using FlowRate = Vehicle::FlowRate;

private:
    // Often used by locomotive derived classes.
    [[nodiscard]] bool hasEngine_() const noexcept override
    {
        return true;
    }
    [[nodiscard]] Speed maxSpeed_() const noexcept override
    {
        return {0.0, ""};
    }
    [[nodiscard]] Power maxPower_() const noexcept override
    {
        return {0.0, ""};
    }
    [[nodiscard]] FlowRate fuelConsumption_() const noexcept override
    {
        return {0.0, ""};
    }

    // Normally not used by locomotive derived classes.
    [[nodiscard]] bool carriesPassengers_() const noexcept override
    {
        return false;
    }
    [[nodiscard]] int numberOfPassengers_() const noexcept override
    {
        return 0;
    }
    [[nodiscard]] bool hasInternet_() const noexcept override
    {
        return false;
    }
    [[nodiscard]] bool carriesCargo_() const noexcept override
    {
        return false;
    }
    [[nodiscard]] Capacity<double> cargoCapacity_() const noexcept override
    {
        return {0.0, ""};
    }
    [[nodiscard]] Area floorArea_() const noexcept override
    {
        return {0.0, ""};
    }
};

}  // namespace pabo::train

#endif
