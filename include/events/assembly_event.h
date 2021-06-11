/**
    @file include/assembly_event.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The definition of the assembly event.
*/
#ifndef INCLUDE_ASSEMBLY_EVENT_H
#define INCLUDE_ASSEMBLY_EVENT_H

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

class AssemblyEvent : public Event {
public:
    AssemblyEvent(app::Simulator& sim, TrainDispatcher& m_dis,
                  int trainNbr, time::TimeOfDay time);

private:
    [[nodiscard]] std::string type_() const override { return "assembly"; }
    [[nodiscard]] bool isHighPriority_() const override { return false; }
    void processEvent_(TrainLog&, CarLog&) override;

    void processAssembledTrain(TrainLog&);
    void calculateTimeForReadyEvent();
    void logAssembledTrain(TrainLog&);
    void scheduleNewReadyEvent();

    void processIncompleteTrain(TrainLog&);
    void calculateNextAttempt();
    void delayDeparture();
    void logIncompleteTrain(TrainLog&);
    void scheduleNewAssemblyEvent();

    app::Simulator& m_sim;
    TrainDispatcher& m_disp;
    int m_trainNbr;
    const Train* m_currentTrain{nullptr};
    time::TimeOfDay m_timeOfNext;
};

}  // namespace pabo::train

#endif
