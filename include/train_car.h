/**
    @file include/train_car.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Definition of the abstract class TrainCar.

    This class strives to make it easier to create subclasses by
    providing default implementations for some of the virtual functions
    declared in the base class.
*/
#ifndef INCLUDE_TRAIN_CAR_H
#define INCLUDE_TRAIN_CAR_H

#include "vehicle.h"

namespace pabo::train {

class TrainCar : public Vehicle {
public:
    using Area = Vehicle::Area;
    using Speed = Vehicle::Speed ;
    using Volume = Vehicle::Volume ;
    using Weight = Vehicle::Weight ;

private:
    // Normally not used by TrainCar derived classes.
    [[nodiscard]] bool hasEngine_() const noexcept override
    {
        return false;
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

    // Often used by TrainCars derived classes.
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
