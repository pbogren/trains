/**
    @file include/printer.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The definition of the Printer class.
*/
#ifndef INCLUDE_PRINTER_H
#define INCLUDE_PRINTER_H

#include "train.h"
#include "train_log.h"
#include "vehicle.h"
#include <iostream>
#include <string>
#include <vector>

namespace pabo::train {

struct CarRecord;
class Station;
class TrainDispatcher;

// The level of detail that the printer should print the data with.
enum class LogLevel { low = 1,
                      medium,
                      high };

using namespace std::string_literals;

class Printer {
public:
    using Iterator = std::vector<TrainRecord>::iterator;
    using Car = Vehicle;

    Printer(TrainDispatcher& disp);

    // Log level
    void setLogLevel(int lvl);
    [[nodiscard]] LogLevel logLevel() const;
    [[nodiscard]] std::string logLevelAsString() const;

    // Return the current ostream object
    std::ostream& ostream();
    // Set the ostream object
    void setOstream(std::ostream& out);

    // Print a string
    void print(const std::string& str) const;
    void println(const std::string& str = ""s) const;

    // Print a train record
    void print(const TrainRecord&);
    // Print a sequence of train records
    void print(Iterator first, Iterator last);

    // Misc print operations on a train
    void print(const Train& train);
    void printAttachedVehicles(const Train& train);
    void printMissingVehicles(const Train& train);

    // Print the state of a station
    void print(const Station& stn);

    // Misc print operations on a car.
    void print(const Car& car);
    void printCarWithLocation(const Car& car);
    void printCarFeatures(const Car& car);

    // Print a car record i.e. the travel history of a car.
    void print(const CarRecord& rec);

private:
    LogLevel m_logLvl{LogLevel::high};
    TrainDispatcher& m_disp;
    std::ostream* os{&std::cout};
};

}  // namespace pabo::train

#endif
