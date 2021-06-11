/**
    @file include/ready_event.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Definition of the ready event;
*/
#ifndef INCLUDE_READY_EVENT_H
#define INCLUDE_READY_EVENT_H

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

class ReadyEvent : public Event {
public:
    ReadyEvent(app::Simulator& sim, TrainDispatcher& disp,
               int trainNbr, time::TimeOfDay time);

private:
    [[nodiscard]] bool isHighPriority_() const override { return false; }
    [[nodiscard]] std::string type_() const override { return "ready"; }
    void processEvent_(TrainLog&, CarLog&) override;
    void updateStateOfTrain();
    void calculateTimeOfDeparture();
    void logReadyTrain(TrainLog&);
    void scheduleDepartureEvent();

    app::Simulator& m_sim;
    TrainDispatcher& m_disp;
    int m_trainNbr;
    const Train* m_currentTrain{nullptr};
    time::TimeOfDay m_timeOfDeparture;
};

}  // namespace pabo::train

#endif
