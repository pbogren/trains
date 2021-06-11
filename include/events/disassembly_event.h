/**
    @file include/disassembly_event.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Definition of the disassembly event;
*/
#ifndef INCLUDE_DISASSEMBLY_EVENT_H
#define INCLUDE_DISASSEMBLY_EVENT_H

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

class DisassemblyEvent : public Event {
public:
    DisassemblyEvent(app::Simulator& sim, TrainDispatcher& disp,
                     int trainNbr, time::TimeOfDay time);

private:
    [[nodiscard]] bool isHighPriority_() const override { return true; }
    [[nodiscard]] std::string type_() const override { return "disassembly"; }
    void processEvent_(TrainLog&, CarLog&) override;
    void updateStateOfTrain();
    void logDisassembledTrain(TrainLog& logger);

    app::Simulator& m_sim;
    TrainDispatcher& m_disp;
    int m_trainNbr;
    const Train* m_currentTrain{nullptr};
};

}  // namespace pabo::train

#endif
