/**
    @file include/train.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief The definition of the Train class.
*/

#ifndef INCLUDE_TRAIN_H
#define INCLUDE_TRAIN_H

#include "capacity.h"
#include "time_point.h"
#include "train_connection.h"
#include "vehicle.h"
#include "vehicle_type.h"
#include <iosfwd>
#include <memory>  // unique_ptr
#include <string>
#include <vector>

namespace pabo::train {

class Train {
public:
    // Shared ptr allows history to be a copy
    using Car = std::shared_ptr<Vehicle>;
    using CarType = Vehicle::Type;
    using Speed = Capacity<double>;
    using Duration = time::TimeOfDay;

    // A car view is a read only view of a train car.
    using CarView = const Vehicle*;

    struct Slot {
        Slot(CarType, Car);

        CarType type;
        Car car;
    };
    enum class State;

    //
    // Construct
    //

    Train() = default;

    // Construct a train from a corresponding connection.
    Train(const TrainConnection&);

    //
    // Queries
    //
    [[nodiscard]] int number() const noexcept;

    // The time of the departure time of this train.
    [[nodiscard]] time::TimeOfDay departure() const noexcept;

    // Returns true if the vector returned by missingCarTypes
    // is empty.
    [[nodiscard]] bool isAssembled() const noexcept;

    // Returns true of the state of the train is running, arrived
    // of finished.
    [[nodiscard]] bool hasDeparted() const noexcept;

    // Returns the current state of the train.
    [[nodiscard]] State state() const noexcept;

    // Returns all of the missing car types needed to assemble this
    // train.
    [[nodiscard]] std::vector<CarType> missingCarTypes() const;

    // Car queries for this train.
    [[nodiscard]] bool missesCarOfType(CarType) const;
    [[nodiscard]] bool hasCar(int id) const;

    // Returns a read-only view of the car with given id.
    // A runtime_error will be thrown if haCar(id) does not return
    // true for the given id.
    [[nodiscard]] CarView viewCar(int id) const;

    // Returns read only views of all the cars currently attached to
    // the train.
    [[nodiscard]] std::vector<CarView> attachedCars() const;

    // Returns the speed of the slowest of the trains cars.
    [[nodiscard]] Speed maxSpeed() const;

    // Returns the current speed of the train.
    [[nodiscard]] Speed currentSpeed() const;

    // Returns the current delay (at arrival or departure) of the train.
    [[nodiscard]] Duration arrivalDelay() const;
    [[nodiscard]] Duration departureDelay() const;

    //
    // Commands
    //

    // Delay the trains departure time.
    void delayDeparture(int minutes);

    // Attach a car to the train.
    // A runtime_error exception is thrown if the car is not of a
    // type that is returned by missesCarOfType.
    void attachCar(Car);

    // Manually set the state of the train.
    void setState(State) noexcept;

    // Sets the speed of the train.
    // A runtime_error exception is thrown if the speed is
    // greater than the one returned by maxSpeed().
    void setSpeed(const Speed& spd);

    void setArrivalDelay(Duration delay);
    void setDepartureDelay(Duration delay);

    // Disassembles the trains and returns the cars.
    // After this operation the train has no attached cars.
    // The State is set to not_assembled but can be manually
    // set if needed (e.g. to finished).
    [[nodiscard]] std::vector<Car> disassemble();

private:
    // Sets the initial state of the train.
    static State defaultState();

    // Finds an empty slot that matches the type given.
    // Throws a runtime_error exception of the type is not one that
    // is returned by missesCarOfType.
    Slot& findEmptySlotByType(CarType);

    int m_nbr;
    State m_state{defaultState()};
    time::TimeOfDay m_departure;
    Duration m_departureDelay;
    Duration m_arrivalDelay;
    std::vector<Slot> m_self;
    Speed m_currentSpd{0.0, "kph"};
};

enum class Train::State {
    not_assembled,
    assembled,
    incomplete,
    ready,
    running,
    arrived,
    finished,
};

std::ostream& operator<<(std::ostream&, Train::State);

}  // namespace pabo::train

#endif
