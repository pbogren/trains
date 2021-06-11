/**
    @file include/simulator.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The definition of the simulator class.
*/
#ifndef INCLUDE_SIMULATOR_H
#define INCLUDE_SIMULATOR_H

#include "event.h"
#include "time_point.h"
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace pabo::train {
class TrainLog;
class CarLog;
class TrainDispatcher;
}

namespace pabo::app {

using std::vector;
using std::shared_ptr;
using train::EventComparison;
using std::priority_queue;
using pabo::train::TrainLog;
using pabo::train::CarLog;

class Simulator {
public:
    Simulator(train::TrainDispatcher&, TrainLog&, CarLog&);
    using Duration = time::TimeOfDay;

    // Queries
    [[nodiscard]] time::TimeOfDay startTime() const;
    [[nodiscard]] time::TimeOfDay endTime() const;
    [[nodiscard]] Duration interval() const;
    // Stop time for next interval
    [[nodiscard]] time::TimeOfDay nextStopTime() const;
    [[nodiscard]] time::TimeOfDay currentTime() const;
    [[nodiscard]] bool isFinished() const;
    [[nodiscard]] bool timeIsUp() const;

    // Reset the state of the simulator
    void reset();

    // Set time
    void setStartTime(std::string);
    void setEndTime(std::string);
    void setInterval(int minutes);

    // Event handling.
    void scheduleEvent(std::shared_ptr<train::Event>);
    void runNextEvent();
    void runNextInterval();
    void runToCompletion();

private:
    using EventPtr = shared_ptr<train::Event>;

    void clearEvents();
    [[nodiscard]] auto nextEvent() const;
    void syncClockWithEvent(const train::Event& event);
    void runTo(const Duration& end);

    time::TimeOfDay m_start{"00:00"};
    time::TimeOfDay m_end{"23:59"};
    time::TimeOfDay m_clock{"00:00"};
    Duration m_interval{"00:10"};

    train::TrainDispatcher& m_dispatch;
    TrainLog& m_log;
    CarLog& m_carLog;
    int m_highPriorityEvents{0};
    priority_queue<EventPtr, vector<EventPtr>, EventComparison> m_queue;
};


}  // namespace pabo::app

#endif
