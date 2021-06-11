/**
    @file src/ready_event.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The implementation of the ready event.
*/

#include "departure_event.h"
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

ReadyEvent::ReadyEvent(Simulator& sim, TrainDispatcher& disp,
                       int trainNbr, TimeOfDay time)
    : Event{time}
    , m_sim{sim}
    , m_disp{disp}
    , m_trainNbr{trainNbr}
    , m_currentTrain{&m_disp.viewTrain(m_trainNbr)}
{
}

void ReadyEvent::processEvent_(TrainLog& logger, CarLog&)
{
    updateStateOfTrain();
    calculateTimeOfDeparture();
    if (m_time >= m_sim.startTime()) {
        logReadyTrain(logger);
    }
    scheduleDepartureEvent();
}

void ReadyEvent::updateStateOfTrain()
{
    m_disp.setStateOfTrain(m_trainNbr, Train::State::ready);
}

void ReadyEvent::calculateTimeOfDeparture()
{
    m_timeOfDeparture = m_time + constants::timeBetweenReadyAndDeparture;
}

void ReadyEvent::logReadyTrain(TrainLog& logger)
{
    logger.log({m_time, *m_currentTrain, "is now at the platform, departing at " + m_timeOfDeparture.asString()});
}

void ReadyEvent::scheduleDepartureEvent()
{
    auto e = std::make_shared<DepartureEvent>(
            m_sim, m_disp, m_trainNbr, m_timeOfDeparture);
    m_sim.scheduleEvent(std::move(e));
}

}  // namespace pabo::train
