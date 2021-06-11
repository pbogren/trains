#include "train_log.h"
#include <iterator>  // begin, end
#include "train_dispatcher.h"
#include <cassert>
#include <algorithm>  // sort
#include <utility>  // move
#include "time_point.h"

namespace pabo::train {

using time::TimeOfDay;
using TrainView = TrainDispatcher::TrainView;

void TrainLog::log(TrainRecord tr)
{
    m_history.emplace_back(std::move(tr));
    sort();
}

void TrainLog::sort()
{
    using std::begin;
    using std::end;
    std::sort(begin(m_history), end(m_history),
              [](const TrainRecord& lhs, const TrainRecord& rhs) {
                  return lhs.time < rhs.time;
              });
}

std::vector<TrainRecord> TrainLog::view(time::TimeOfDay from, time::TimeOfDay to)
{
    using std::begin;
    using std::end;
    using std::rbegin;
    using std::rend;
    auto first = std::find_if(begin(m_history), end(m_history),
                              [&from](const TrainRecord& tr) {
                                  return tr.time >= from;
                              });
    auto last = std::find_if(rbegin(m_history), rend(m_history),
                             [&to](const TrainRecord& tr) {
                                 return tr.time <= to;
                             });
    std::vector<TrainRecord> res;
    std::for_each(first, last.base(), [&res] (TrainRecord tr) {
        res.emplace_back(std::move(tr));
    });
    return res;
}

TrainRecord TrainLog::viewLast()
{
    assert(!m_history.empty());
    const auto last = m_history.back();
    return last;
}


}  // namespace pabo::train
