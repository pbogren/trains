/**
    @file src/disassembly_event.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The implementation of the disassembly event.
*/

#include "disassembly_event.h"
#include "simulator.h"
#include "time_point.h"
#include "train.h"
#include "train_dispatcher.h"
#include "train_log.h"
#include <cassert>
#include <string>
#include <vector>

namespace pabo::train {

using app::Simulator;
using time::TimeOfDay;

DisassemblyEvent::DisassemblyEvent(Simulator& sim, TrainDispatcher& disp,
                                   int trainNbr, TimeOfDay time)
    : Event{time}
    , m_sim{sim}
    , m_disp{disp}
    , m_trainNbr{trainNbr}
    , m_currentTrain{&m_disp.viewTrain(m_trainNbr)}
{
}

void DisassemblyEvent::processEvent_(TrainLog& logger, CarLog&)
{
    updateStateOfTrain();
    if (m_time >= m_sim.startTime()) {
        logDisassembledTrain(logger);
    }
}

void DisassemblyEvent::updateStateOfTrain()
{
    m_disp.disassembleTrain(m_trainNbr);
    assert(m_currentTrain->attachedCars().empty());
    m_disp.setStateOfTrain(m_trainNbr, Train::State::finished);
}

void DisassemblyEvent::logDisassembledTrain(TrainLog& logger)
{
    logger.log({m_time, *m_currentTrain, "is now disassembled."});
}

}  // namespace pabo::train
