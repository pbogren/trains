#include "event.h"

namespace pabo::train {

Event::Event(time::TimeOfDay t)
    : m_time{t} {}

void Event::processEvent(TrainLog& log, CarLog& cl)
{
    processEvent_(log, cl);
}

time::TimeOfDay Event::time() const
{
    return m_time;
}

std::string Event::type() const
{
    return type_();
}

bool Event::isHighPriority() const
{
    return isHighPriority_();
}

bool EventComparison::operator()(const std::shared_ptr<Event>& lhs,
                                 const std::shared_ptr<Event>& rhs)
{
    return lhs->time() > rhs->time();
}

}  // namespace pabo::train
