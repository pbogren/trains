/**
    @file src/start_event.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The implementation of the start event
*/
#ifndef SRC_START_EVENT_CPP
#define SRC_START_EVENT_CPP

#include "assembly_event.h"
#include "event.h"
#include "sim_constants.h"
#include "simulator.h"
#include "start_event.h"
#include "time_point.h"
#include "train_dispatcher.h"
#include <memory>  // unique_ptr, make_unique
#include <utility>  // move
#include <vector>

namespace pabo::train {

class TrainLog;
class CarLog;

using app::Simulator;
using time::TimeOfDay;

StartEvent::StartEvent(Simulator& sim, TrainDispatcher& disp)
    : Event{TimeOfDay{"00:00"}}, m_sim{sim}, m_disp{disp}
{
}

void StartEvent::processEvent_(TrainLog&, CarLog&)
{
    for (const auto& trainNbr: m_disp.trainNumbers()) {
        const auto time = calculateAssemblyTime(trainNbr);
        scheduleAssemblyEvent(trainNbr, time);
    }
}

time::TimeOfDay StartEvent::calculateAssemblyTime(const int trainNbr) const
{
    return m_disp.scheduledTimeOfDeparture(trainNbr) -
           constants::timeBetweenDepartureAndAssembly;
}

void StartEvent::scheduleAssemblyEvent(int trainNbr, time::TimeOfDay time)
{
    auto e = std::make_unique<AssemblyEvent>(m_sim, m_disp, trainNbr, time);
    m_sim.scheduleEvent(std::move(e));
}

}  // namespace pabo::train

#endif
