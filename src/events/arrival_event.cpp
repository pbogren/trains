/**
    @file src/arrival_event.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The implementatoin of the arrival event.
*/
#include "arrival_event.h"
#include "car_log.h"
#include "disassembly_event.h"
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

using app::Simulator;
using time::TimeOfDay;

ArrivalEvent::ArrivalEvent(Simulator& sim, TrainDispatcher& disp,
                           int trainNbr, TimeOfDay time)
    : Event{time}
    , m_sim{sim}
    , m_disp{disp}
    , m_trainNbr{trainNbr}
    , m_currentTrain{&m_disp.viewTrain(m_trainNbr)}
{
}

void ArrivalEvent::processEvent_(TrainLog& logger, CarLog& carLog)
{
    updateStateOfTrain();
    calculateDisassemblyTime();
    if (m_time >= m_sim.startTime()) {
        logArrivingTrain(logger);
        logArrivingCars(carLog);
    }
    scheduleDisassemblyEvent();
}

void ArrivalEvent::updateStateOfTrain()
{
    m_disp.setStateOfTrain(m_trainNbr, Train::State::arrived);
    m_disp.setArrivalDelay(m_trainNbr);
}

void ArrivalEvent::calculateDisassemblyTime()
{
    m_disassemblyTime = m_time + constants::timeBetweenArrivalAndDisassembly;
}

void ArrivalEvent::logArrivingTrain(TrainLog& logger)
{
    logger.log({m_time, *m_currentTrain,
                "Has Arrived at the platform disassembly at " +
                        m_disassemblyTime.asString()});
}

void ArrivalEvent::logArrivingCars(CarLog& carLog)
{
    const auto destination = m_disp.destination(m_trainNbr);
    carLog.logArrival(m_time, *m_currentTrain, destination);
}

void ArrivalEvent::scheduleDisassemblyEvent()
{
    const auto e = std::make_shared<DisassemblyEvent>(
            m_sim, m_disp, m_trainNbr, m_disassemblyTime);
    m_sim.scheduleEvent(std::move(e));
}


}  // namespace pabo::train
