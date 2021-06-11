/**
    @file include/start_event.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Definition of the start event;
*/
#ifndef INCLUDE_START_EVENT_H
#define INCLUDE_START_EVENT_H

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

class StartEvent : public Event {
public:
    StartEvent(app::Simulator& sim, TrainDispatcher& disp);

private:
    [[nodiscard]] std::string type_() const override { return "start"; }
    [[nodiscard]] bool isHighPriority_() const override { return false; }

    void processEvent_(TrainLog&, CarLog&) override;
    [[nodiscard]] time::TimeOfDay calculateAssemblyTime(int trainNbr) const;
    void scheduleAssemblyEvent(int trainNbr, time::TimeOfDay time);

    app::Simulator& m_sim;
    TrainDispatcher& m_disp;
};

}  // namespace pabo::train

#endif
