/**
    @file include/departure_event.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Definition of the departure event;
*/
#ifndef INCLUDE_DEPARTURE_EVENT_H
#define INCLUDE_DEPARTURE_EVENT_H


#include "event.h"
#include <string>

namespace pabo::app {
class Simulator;
}

namespace pabo::time {
class TimeOfDay;
}

namespace pabo::train {

class TrainDispatcher;
class TrainLog;
class CarLog;
class Train;

class DepartureEvent : public Event {
public:
    DepartureEvent(app::Simulator& sim, TrainDispatcher& disp,
                   int trainNbr, time::TimeOfDay time);

private:
    [[nodiscard]] bool isHighPriority_() const override { return false; }
    [[nodiscard]] std::string type_() const override { return "departure"; }
    void processEvent_(TrainLog&, CarLog&) override;
    void updateTrainState();
    void prepareLogMessage();
    void logDepartedTrain(TrainLog& logger);
    void scheduleArrivalEvent();


    app::Simulator& m_sim;
    TrainDispatcher& m_disp;
    int m_trainNbr;
    const Train* m_currentTrain{nullptr};
    std::string m_logMsg;
};

}  // namespace pabo::train

#endif
