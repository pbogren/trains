/**
    @file include/train_log.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The definition of the train log.
*/
#ifndef INCLUDE_TRAIN_LOG_H
#define INCLUDE_TRAIN_LOG_H

#include "time_point.h"
#include "train.h"
#include <memory>  // shared_ptr
#include <string>
#include <utility>  // move
#include <vector>

namespace pabo::train {

struct TrainRecord {
    TrainRecord() = default;
    TrainRecord(time::TimeOfDay tp, const Train& t, std::string desc)
        : time{std::move(tp)}
        , train{std::make_shared<const Train>(t)}
        , eventDescription{std::move(desc)}
    {
    }

    // The time that the event occured
    time::TimeOfDay time;

    // A snapshot of the trains state at the time of the event
    std::shared_ptr<const Train> train{nullptr};

    // A description of the event.
    std::string eventDescription;
};

class TrainLog {
public:

    // Add the train record to the log
    void log(TrainRecord tr);

    // Return the records that occured after a given time and up
    // until (and including) a given time.
    std::vector<TrainRecord> view(time::TimeOfDay from, time::TimeOfDay to);

    // View the last record in the log.
    TrainRecord viewLast();

private:
    void sort();

    std::vector<TrainRecord> m_history;
};

}  // namespace pabo::train

#endif
