/**
    @file include/train_log.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The definition of the car log.
*/
#ifndef INCLUDE_CAR_LOG_H
#define INCLUDE_CAR_LOG_H


#include "time_point.h"
#include "train.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace pabo::train {

struct CarRecord {
    CarRecord(time::TimeOfDay tod, int trainNbr, std::string stationName);

    time::TimeOfDay time;
    int trainNbr;
    std::string destination;
};

class CarLog {
public:
    // Logs the arrival of each car in the train.
    void logArrival(time::TimeOfDay tod, const Train& t, const std::string& stationName);
    // Returns a
    std::vector<CarRecord> viewRecordOf(int id);

private:
    void sort();

    std::unordered_map<int, std::vector<CarRecord>> m_history;
};

}  // namespace pabo::train

#endif
