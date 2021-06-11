#include "event.h"
#include "simulator.h"
#include "time_point.h"
#include "train_dispatcher.h"
#include <memory>
#include <string>
#include <utility>  // move

namespace pabo::app {

using Sim = Simulator;

Sim::Simulator(train::TrainDispatcher& disp, TrainLog& log, CarLog& cl)
    : m_dispatch{disp}, m_log{log}, m_carLog{cl}
{
}

auto Sim::nextEvent() const
{
    return m_queue.top();
}

void Sim::reset()
{
    clearEvents();
    m_highPriorityEvents = 0;
    m_clock = m_start;
}

void Sim::clearEvents()
{
    while (!m_queue.empty()) {
        m_queue.pop();
    }
}

time::TimeOfDay Sim::startTime() const
{
    return m_start;
}

time::TimeOfDay Sim::endTime() const
{
    return m_end;
}

Sim::Duration Sim::interval() const
{
    return m_interval;
}


time::TimeOfDay Sim::currentTime() const
{
    return m_clock;
}

time::TimeOfDay Sim::nextStopTime() const
{
    return currentTime() + interval();
}

void Sim::setStartTime(std::string newStartTime)
{
    auto tmp = time::TimeOfDay{std::move(newStartTime)};
    if (tmp > endTime()) {
        throw std::out_of_range("Start time later than end time!");
    }
    using std::swap;
    swap(tmp, m_start);
}

void Sim::setEndTime(std::string newEndTime)
{
    auto tmp = time::TimeOfDay{std::move(newEndTime)};
    if (tmp < startTime()) {
        throw std::out_of_range("End time earlier than start time!");
    }
    using std::swap;
    swap(tmp, m_end);
}

void Sim::setInterval(int minutes)
{
    m_interval = time::TimeOfDay(minutes);
}

void Sim::scheduleEvent(std::shared_ptr<train::Event> e)
{
    bool highPriority = e->isHighPriority();
    if (highPriority) {
        ++m_highPriorityEvents;
    }
    if ((e->time() < endTime()) || highPriority) {
        m_queue.emplace(std::move(e));
    }
}

bool Sim::isFinished() const
{
    return m_dispatch.allTrainsFinished() ||
           (timeIsUp() && m_highPriorityEvents < 1);
}

bool Sim::timeIsUp() const
{
    return m_clock >= m_end;
}

void Sim::runNextEvent()
{
    auto event = nextEvent();
    syncClockWithEvent(*event);
    event->processEvent(m_log, m_carLog);
    if (event->isHighPriority()) {
        --m_highPriorityEvents;
    }
    m_queue.pop();
}

void Sim::syncClockWithEvent(const train::Event& event)
{
    m_clock = event.time();
}

void Sim::runTo(const Duration& end)
{
    while (nextEvent()->time() < end) {
        runNextEvent();
    }
    m_clock = end;
}

void Sim::runNextInterval()
{
    const auto stopTime = nextStopTime();
    runTo(stopTime);
}

void Sim::runToCompletion()
{
    while (!isFinished()) {
        runNextEvent();
    }
    // the clock can run past the set endtime, so only set it
    // if needed.
    if (m_clock < endTime()) {
        m_clock = endTime();
    }
}

}  // namespace pabo::app
