/**
    @file src/printer.cpp
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief The implementation of the printer class.
*/

#include "capacity.h"
#include "car_log.h"
#include "printer.h"
#include "station.h"
#include "time_point.h"
#include "train.h"
#include "train_dispatcher.h"
#include "vehicle_type.h"
#include <algorithm>  // for_each
#include <iomanip>
#include <iostream>
#include <memory>  // shared_ptr
#include <stdexcept>  // runtime_error
#include <string>


namespace pabo::train {

Printer::Printer(TrainDispatcher& disp)
    : m_disp{disp}
{
}

std::ostream& Printer::ostream()
{
    return *os;
}

void Printer::setOstream(std::ostream& out)
{
    os = &out;
}

void Printer::setLogLevel(int lvl)
{
    const auto l = static_cast<LogLevel>(lvl);
    switch (l) {
    case LogLevel::low : [[fallthrough]];
    case LogLevel::medium : [[fallthrough]];
    case LogLevel::high :
        m_logLvl = l;
        break;
    default :
        throw std::runtime_error("No such log level!");
    }
}

LogLevel Printer::logLevel() const
{
    return m_logLvl;
}

std::string Printer::logLevelAsString() const
{
    switch(m_logLvl) {
    case LogLevel::low :
        return "LOW";
    case LogLevel::medium :
        return "MEDIUM";
    case LogLevel::high :
        return "HIGH";
    default :
        throw std::runtime_error("No string representation for log level!");
    }
}

void Printer::print(const TrainRecord& tr)
{
    const auto& [time, train, currentEvent] = tr;

    *os << '\n' << time;
    print(*train);
    *os << currentEvent << '\n';
}

void Printer::print(const Train& train)
{
    const auto nbr = train.number();
    const auto state = train.state();

    const auto origin = m_disp.origin(nbr);
    const auto destination = m_disp.destination(nbr);

    const auto schedDeparture = m_disp.scheduledTimeOfDeparture(nbr);
    const auto schedArrival = m_disp.scheduledTimeOfArrival(nbr);
    const auto estDeparture = train.departure();
    const auto estArrival = m_disp.estimatedTimeOfArrival(train);
    const auto departureDelay = m_disp.departureDelay(train);
    const auto arrivalDelay = m_disp.arrivalDelay(train);

    const auto speed = train.currentSpeed();
    const auto maxSpeed = m_disp.maxSpeed(nbr);

    *os << std::fixed << std::setprecision(0)
        << "\nTrain [" << nbr << "] (" << state << ")\n";

    if (m_logLvl >= LogLevel::medium) {
        *os << "from " << origin << ' '
            << schedDeparture << " (" << estDeparture << ")"

            << " to " << destination << ' '
            << schedArrival << " (" << estArrival <<  ") \n";

        if (logLevel() == LogLevel::high) {
            *os << "departure delay " << departureDelay << " "
                << "arrival delay " << arrivalDelay << " "
                << "Speed " << speed << '\n';
        }
    }
}

void Printer::printAttachedVehicles(const Train& train)
{
    const auto attachedCars = train.attachedCars();
    println("\nAttached vehicles");
    if (attachedCars.empty()) {
        println("\t[none]");
        return;
    }
    for (const auto car : attachedCars) {
        print("\t");
        print(*car);
    }
}

void Printer::printMissingVehicles(const Train& train)
{
    const auto missingCarTypes= train.missingCarTypes();
    println("\nMissing vehicles");
    if (missingCarTypes.empty()) {
        println("\t[none]");
        return;
    }
    for (const auto type : missingCarTypes) {
        print("\t");
        println(typeAsString(type));
    }
}

void Printer::print(const std::string& str) const
{
    *os << str;
}

void Printer::println(const std::string& str) const
{
    print(str + '\n');
}

void Printer::print(Iterator first, Iterator last)
{
    std::for_each(first, last, [this](const TrainRecord& tr) {
        print(tr);
    });
}

void Printer::print(const Station& stn)
{
    const auto stnName = stn.name();
    println("----");
    println(stnName);
    println("----");

    println("Trains at station");
    print("-----------------");
    const auto trains = m_disp.trainsAtStation(stnName);
    if (trains.empty()) {
        println("[no trains]");
    }
    else {
        for (const auto train: trains) {
            print(*train);
        }
    }
    println();

    if (logLevel() >= LogLevel::medium) {
        println("Available cars");
        println("--------------");
        const auto cars = stn.availableCars();
        if (cars.empty()) {
            println("[no cars");
        }
        else {
            for (const auto car: cars) {
                print(*car);
            }
        }
        println();
    }
}

void Printer::printCarWithLocation(const Car& car)
{
    print(car);
    const auto location = m_disp.carLocation(car.id());
    println("Location: " + location);
}

void Printer::print(const Car& car)
{
    *os << '[' << car.type() << "] id: " << car.id();
    if (logLevel() > LogLevel::medium) {
        printCarFeatures(car);
    }
    println();
}

void Printer::printCarFeatures(const Car& car)
{
    if (car.hasEngine()) {
        *os << ", max speed: " << car.maxSpeed();
        if (car.fuelConsumption() > 0.0) {
            *os << ", fuel consumption: " << car.fuelConsumption();
        }
        if (car.maxPower() > 0.0) {
            *os << ", max power: " << car.maxPower();
        }
    }
    if (car.carriesPassengers()) {
        const auto count = car.numberOfPassengers();
        *os << ", passengers: " << count;
    }
    if (typeAsString(car.type()) == "Coach") {
        *os << ((car.hasInternet()) ? "" : " no") ;
        *os << " internet onboard";
    }
    if (car.carriesCargo()) {
        *os << " cargo capacity: " << car.cargoCapacity();
    }
    if (car.floorArea() > 0.0) {
        *os << " floor area: " << car.floorArea();
    }
}

void Printer::print(const CarRecord& rec)
{
    const auto& [time, train, destination] = rec;
    *os << time << ": train[" << train << "] -> " << destination << '\n';
}

}  // namespace pabo::train
