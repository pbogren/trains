/**
    @file include/trains_app.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The user interface for the trains app
*/
#ifndef INCLUDE_TRAINS_APP_H
#define INCLUDE_TRAINS_APP_H

#include "car_log.h"
#include "printer.h"
#include "simulator.h"
#include "station.h"
#include "train.h"
#include "train_dispatcher.h"
#include "train_log.h"
#include <vector>

namespace pabo::time {
class TimeOfDay;
}

namespace pabo::app {

using train::LogLevel;
using train::Printer;
using train::TrainDispatcher;
using train::TrainLog;

class TrainsApp {
public:
    TrainsApp() = default;


    void initialize();
    void start();
    void reset();

    // Sim
    void printStartAndEndTimes();
    void printInterval();
    void printCurrentTime();
    void printNewTime();

    void changeInterval();
    void setStartTime();
    void setEndTime();

    void runNextInterval();
    void printHistory(time::TimeOfDay start, time::TimeOfDay end);
    void runNextEvent();
    void printLast();
    void runUntilComplete();

    bool simulationIsFinished();
    void logIfFinished();
    void setLogLevel();

    // Train
    void showTrainByNumber();
    void showTrainByVehicleId();
    void showAllTrains();
    void showTrain(const train::Train& train);

    // Station
    void showStationNames();
    void showAllStationNames();
    void showStationByName();
    void showAllStations();

    // Vehicle
    void showVehicleById();
    void showAllVehicles();
    void showVehicleHistory(int id);

    void printStatistics();

    // Log
    void printCurrentLogLevel();
    [[nodiscard]] LogLevel logLevel() const;
    void writeLogToFile();

private:
    TrainDispatcher m_dispatch;
    TrainLog m_log;
    CarLog m_carLog;
    Simulator m_sim{m_dispatch, m_log, m_carLog};
    Printer m_printer{m_dispatch};
    std::vector<train::Station> m_initialStationStates;
};

}  // namespace pabo::app

#endif
