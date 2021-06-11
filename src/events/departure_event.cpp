/**
    @file src/departure_event.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The implementation of the departure event.
*/
#include "arrival_event.h"
#include "capacity.h"
#include "departure_event.h"
#include "event.h"
#include "simulator.h"
#include "time_point.h"
#include "train.h"
#include "train_dispatcher.h"
#include "train_log.h"
#include <iomanip>
#include <memory>  // make_unique
#include <sstream>
#include <string>
#include <utility>  // move

namespace pabo::train {

class CarLog;

using app::Simulator;
using time::TimeOfDay;

DepartureEvent::DepartureEvent(Simulator& sim, TrainDispatcher& disp,
                               int trainNbr, TimeOfDay time)
    : Event{time}
    , m_sim{sim}
    , m_disp{disp}
    , m_trainNbr{trainNbr}
    , m_currentTrain{&m_disp.viewTrain(m_trainNbr)}
{
}

void DepartureEvent::processEvent_(TrainLog& logger, CarLog&)
{
    updateTrainState();
    if (m_time >= m_sim.startTime()) {
        logDepartedTrain(logger);
    }
    scheduleArrivalEvent();
}

void DepartureEvent::updateTrainState()
{
    m_disp.setOptimalSpeedOfTrain(m_trainNbr);
    m_disp.setDepartureDelay(m_trainNbr);
    m_disp.setStateOfTrain(m_trainNbr, Train::State::running);
}

void DepartureEvent::logDepartedTrain(TrainLog& logger)
{
    prepareLogMessage();
    logger.log({m_time, *m_currentTrain, m_logMsg});
}

void DepartureEvent::prepareLogMessage()
{
    const auto maxSpeed = m_disp.maxSpeed(m_trainNbr);
    auto description = std::ostringstream{};
    description << std::fixed << std::setprecision(0)
                << "has left the platform, travelling at speed "
                << m_currentTrain->currentSpeed() << '(' << maxSpeed << ')';
    m_logMsg = description.str();
}

void DepartureEvent::scheduleArrivalEvent()
{
    const auto arrival = m_disp.estimatedTimeOfArrival(m_trainNbr);
    auto e = std::make_shared<ArrivalEvent>(m_sim, m_disp, m_trainNbr, arrival);
    m_sim.scheduleEvent(std::move(e));
}

}  // namespace pabo::train
