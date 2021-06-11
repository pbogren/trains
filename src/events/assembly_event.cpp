/**
    @file src/assembly_event.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The implementation of the assembly event.
*/

#include "assembly_event.h"
#include "event.h"
#include "ready_event.h"
#include "sim_constants.h"
#include "simulator.h"
#include "time_point.h"
#include "train.h"
#include "train_dispatcher.h"
#include "train_log.h"
#include <memory>  // make_unique
#include <string>
#include <utility>  // move

namespace pabo::train {

class CarLog;

using app::Simulator;
using time::TimeOfDay;


AssemblyEvent::AssemblyEvent(Simulator& sim, TrainDispatcher& disp,
                             int trainNbr, TimeOfDay time)
    : Event{time}
    , m_sim{sim}
    , m_disp{disp}
    , m_trainNbr{trainNbr}
    , m_currentTrain{&m_disp.viewTrain(m_trainNbr)}
{
}

void AssemblyEvent::processEvent_(TrainLog& log, CarLog&)
{
    m_disp.tryAssembleTrain(m_trainNbr);
    if (m_disp.trainIsAssembled(m_trainNbr)) {
        processAssembledTrain(log);
    }
    else {
        processIncompleteTrain(log);
    }
}

void AssemblyEvent::processAssembledTrain(TrainLog& log)
{
    m_disp.setStateOfTrain(m_trainNbr, Train::State::assembled);
    calculateTimeForReadyEvent();

    if (m_time >= m_sim.startTime()) {
        logAssembledTrain(log);
    }
    scheduleNewReadyEvent();
}

void AssemblyEvent::calculateTimeForReadyEvent()
{
    m_timeOfNext = m_disp.estimatedTimeOfDeparture(m_trainNbr) -
                   constants::timeBetweenAssembledAndReady;
}

void AssemblyEvent::logAssembledTrain(TrainLog& log)
{
    log.log({m_time, *m_currentTrain,
             "is now assembled, arriving at the platform at " +
                     m_timeOfNext.asString()});
}

void AssemblyEvent::scheduleNewReadyEvent()
{
    auto e = std::make_shared<ReadyEvent>(
            m_sim, m_disp, m_trainNbr, m_timeOfNext);
    m_sim.scheduleEvent(std::move(e));
}

void AssemblyEvent::processIncompleteTrain(TrainLog& log)
{
    if (m_currentTrain->state() != Train::State::incomplete) {
        m_disp.setStateOfTrain(m_trainNbr, Train::State::incomplete);
    }
    calculateNextAttempt();
    delayDeparture();
    if (m_time >= m_sim.startTime()) {
        logIncompleteTrain(log);
    }
    scheduleNewAssemblyEvent();
}

void AssemblyEvent::calculateNextAttempt()
{
    m_timeOfNext = m_time + constants::timeBetweenAssemblyAttempts;
}

void AssemblyEvent::delayDeparture()
{
    m_disp.delayDeparture(m_trainNbr, constants::timeBetweenAssemblyAttempts);
}

void AssemblyEvent::logIncompleteTrain(TrainLog& log)
{
    log.log({m_time, *m_currentTrain,
             "is now incomplete, next try " + m_timeOfNext.asString()});
}

void AssemblyEvent::scheduleNewAssemblyEvent()
{
    auto e = std::make_shared<AssemblyEvent>(
            m_sim, m_disp, m_trainNbr, m_timeOfNext);
    m_sim.scheduleEvent(std::move(e));
}

}  // namespace pabo::train
