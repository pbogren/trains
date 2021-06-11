/**
    @file include/event.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The definition of events
*/
#ifndef INCLUDE_EVENT_H
#define INCLUDE_EVENT_H

#include "time_point.h"
#include "train_dispatcher.h"
#include <memory>
#include <string>

namespace pabo::train {

using TrainView = TrainDispatcher::TrainView;

class TrainLog;
class CarLog;

class Event {
public:
    Event(time::TimeOfDay t);
    void processEvent(TrainLog&, CarLog&);
    [[nodiscard]] time::TimeOfDay time() const;
    [[nodiscard]] std::string type() const;
    [[nodiscard]] bool isHighPriority() const;

    Event(const Event&) = delete;
    Event(Event&&) = delete;
    Event& operator=(const Event&) = delete;
    Event& operator=(Event&&) = delete;


    virtual ~Event() = default;

private:
    virtual void processEvent_(TrainLog&, CarLog&) = 0;
    virtual std::string type_() const = 0;
    virtual bool isHighPriority_() const = 0;

protected:
    time::TimeOfDay m_time{0};
};

class EventComparison {
public:
    bool operator()(const std::shared_ptr<Event>& lhs,
                    const std::shared_ptr<Event>& rhs);
};

}  // namespace pabo::train

#endif
