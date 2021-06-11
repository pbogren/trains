/**
    @file include/train_dispatch.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The definition of the TrainDispatcher class.

*/

#ifndef INCLUDE_TRAIN_DISPATCH_H
#define INCLUDE_TRAIN_DISPATCH_H

#include "capacity.h"
#include "path.h"
#include "station.h"
#include "time_point.h"
#include "train.h"
#include "train_connection.h"
#include <string>
#include <vector>

namespace pabo::train {

class TrainDispatcher {
public:
    using ConnObj = TrainConnection;
    using TrainObj = Train;
    using StationObj = Station;
    using PathObj = Path;
    using Distance = Capacity<double>;
    using Speed = Train::Speed;
    using Duration = time::TimeOfDay;

    using TrainView = const Train&;
    using StationView = const Station&;
    using CarView = Train::CarView;

    TrainDispatcher() = default;
    TrainDispatcher(std::vector<ConnObj>,
                    std::vector<StationObj>,
                    std::vector<PathObj>);

    // Time queries
    [[nodiscard]] time::TimeOfDay estimatedTimeOfDeparture(int nbr) const;
    [[nodiscard]] time::TimeOfDay estimatedTimeOfArrival(int nbr) const;
    [[nodiscard]] time::TimeOfDay estimatedTimeOfArrival(const Train& train) const;

    [[nodiscard]] time::TimeOfDay scheduledTimeOfDeparture(int nbr) const;
    [[nodiscard]] time::TimeOfDay scheduledTimeOfArrival(int nbr) const;

    [[nodiscard]] time::TimeOfDay currentDelay(int nbr) const;
    [[nodiscard]] time::TimeOfDay currentDelay(const Train& t) const;

    [[nodiscard]] time::TimeOfDay departureDelay(const Train& t) const;
    [[nodiscard]] time::TimeOfDay arrivalDelay(const Train& t) const;

    // Train queries
    [[nodiscard]] std::vector<int> trainNumbers() const;
    [[nodiscard]] std::vector<int> delayedTrainNumbers() const;
    [[nodiscard]] std::vector<int> nonDepartedTrainNumbers() const;
    [[nodiscard]] TrainView viewTrain(int nbr) const;
    [[nodiscard]] bool trainIsAssembled(int nbr) const;
    [[nodiscard]] bool allTrainsFinished() const;
    [[nodiscard]] std::string trainLocation(const Train& t) const;
    [[nodiscard]] TrainView viewTrainByVehicleId(int id) const;

    [[nodiscard]] Duration totalArrivalDelay() const;
    [[nodiscard]] Duration totalDepartureDelay() const;

    // Returns the lowest speed of the maximum allowed track speed
    // and the highest possible speed for the train.
    [[nodiscard]] Train::Speed maxSpeed(int nbr) const;

    // Connection queries
    [[nodiscard]] Distance distance(int nbr) const;
    [[nodiscard]] std::string origin(int nbr) const;
    [[nodiscard]] std::string destination(int nbr) const;

    // Station queries
    [[nodiscard]] std::vector<std::string> stationNames() const;
    [[nodiscard]] StationView viewStation(const std::string& name) const;
    [[nodiscard]] std::vector<const Train*> trainsAtStation(const std::string& name);

    // Vehicle queries
    [[nodiscard]] std::vector<CarView> viewAllCars() const;
    [[nodiscard]] CarView viewCar(int id) const;
    [[nodiscard]] std::string carLocation(int id) const;

    // Commands
    void tryAssembleTrain(int nbr);
    void disassembleTrain(int nbr);
    void setStateOfTrain(int nbr, Train::State s);
    void delayDeparture(int nbr, time::TimeOfDay delay);
    void setDepartureDelay(int nbr);
    void setArrivalDelay(int nbr);
    void setOptimalSpeedOfTrain(int nbr);

private:
    // Find operations
    [[nodiscard]] auto findStationByName(const std::string& name) -> std::vector<StationObj>::iterator;
    [[nodiscard]] auto findStationByName(const std::string& name) const -> std::vector<StationObj>::const_iterator;

    [[nodiscard]] auto findTrainByNbr(int nbr) const -> std::vector<TrainObj>::const_iterator;
    [[nodiscard]] auto findTrainByNbr(int nbr) -> std::vector<TrainObj>::iterator;

    [[nodiscard]] auto findConnectionByNbr(int nbr) const -> std::vector<ConnObj>::const_iterator;
    [[nodiscard]] auto findConnectionByNbr(int nbr) -> std::vector<ConnObj>::iterator;

    [[nodiscard]] Distance findDistance(int nbr) const;
    [[nodiscard]] Distance findDistance(std::string station1,
                                        std::string station2) const;

    [[nodiscard]] auto findCarById(int id) const;
    [[nodiscard]] auto findCarById(int id);

    // Calculations
    [[nodiscard]] time::TimeOfDay calculateDelayOfStaticTrain(const Train& t) const;
    [[nodiscard]] time::TimeOfDay calculateDelayOfRunningTrain(const Train& t) const;

    // TODO Keep a cache for each vector to avoid unecessary find
    // operations.

    std::vector<TrainObj> m_trains;
    std::vector<ConnObj> m_connections;
    std::vector<StationObj> m_stations;
    std::vector<PathObj> m_map;
};

//
// Non-members
//

TrainDispatcher::Duration calcTravelTime(const TrainDispatcher::Distance&,
                                         const Train::Speed&);

TrainDispatcher::Speed calcSpeed(const TrainDispatcher::Distance& d,
                                 const TrainDispatcher::Duration& travelTime);

}  // namespace pabo::train

#endif
