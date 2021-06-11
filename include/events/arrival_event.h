/**
    @file include/arrival_event.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Definition of the arrival event;
*/
#ifndef INCLUDE_ARRIVAL_EVENT_H
#define INCLUDE_ARRIVAL_EVENT_H

#include "event.h"
#include "time_point.h"
#include <string>

namespace pabo::app {
class Simulator;
}

namespace pabo::train {

class TrainDispatcher;
class TrainLog;
class CarLog;
class Train;

class ArrivalEvent : public Event {
public:
    ArrivalEvent(app::Simulator& sim, TrainDispatcher& disp,
                 int trainNbr, time::TimeOfDay time);

private:
    [[nodiscard]] bool isHighPriority_() const override { return true; }
    [[nodiscard]] std::string type_() const override { return "arrival"; }
    void processEvent_(TrainLog&, CarLog&) override;
    void updateStateOfTrain();
    void calculateDisassemblyTime();
    void logArrivingTrain(TrainLog& logger);
    void logArrivingCars(CarLog& carLog);
    void scheduleDisassemblyEvent();

    app::Simulator& m_sim;
    TrainDispatcher& m_disp;
    int m_trainNbr;
    const Train* m_currentTrain{nullptr};
    time::TimeOfDay m_disassemblyTime;
};

}  // namespace pabo::train

#endif
