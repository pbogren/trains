#include "console_IO.h"
#include "path.h"
#include "simulator.h"
#include "station.h"
#include "time_point.h"
#include "train_connection.h"
#include "train_dispatcher.h"
#include "start_event.h"
#include "trains_app.h"
#include <cassert>
#include <fstream>
#include <iterator>  // begin, end
#include <memory>  // make_unique
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace pabo::train {
class Event;
}

namespace pabo::app {

using App = TrainsApp;
using namespace pabo::train;
using namespace pabo::IO;
using train::Event;

std::vector<TrainConnection> readConnectionsFromFile(const std::string&);
std::vector<Station> readStationsFromFile(const std::string&);
std::vector<Path> readMapFromFile(const std::string&);


void App::initialize()
{
    clearScreen();
    print("Reading Trains.txt...");
    auto connections = readConnectionsFromFile("Trains.txt");
    println("Ok!");

    print("Reading TrainStations.txt...");
    auto stations = readStationsFromFile("TrainStations.txt");
    println("Ok!");
    m_initialStationStates = stations;

    print("Reading TrainMap.txt...");
    auto map = readMapFromFile("TrainMap.txt");
    println("Ok!");

    m_dispatch = TrainDispatcher(std::move(connections),
                                 std::move(stations),
                                 std::move(map));
}

std::vector<TrainConnection> readConnectionsFromFile(const std::string& fname)
{
    auto file = std::ifstream{fname};
    if (!file) { throw std::runtime_error("Could not read " + fname + '.'); }

    auto connections = std::vector<TrainConnection>{};
    for (TrainConnection conn; file >> conn;) {
        connections.emplace_back(std::move(conn));
    }
    assert(connections.size() == 130);
    return connections;
}


std::vector<Station> readStationsFromFile(const std::string& fname)
{
    auto file = std::ifstream{fname};
    if (!file) { throw std::runtime_error("Could not read " + fname + '.'); }

    std::vector<Station> stations{};
    auto carCount = 0;
    for (Station station; file >> station;) {
        carCount += station.carCount();
        stations.emplace_back(std::move(station));
    }
    assert(stations.size() == 8);
    assert(carCount == 741);

    return stations;
}

std::vector<Path> readMapFromFile(const std::string& fname)
{
    auto file = std::ifstream{fname};
    if (!file) { throw std::runtime_error("Could not read " + fname + '.'); }

    auto map = std::vector<Path>{};
    for (Path p; file >> p;) {
        map.push_back(p);
    }
    assert(map.size() == 28);
    return map;
}

void App::start()
{
    auto e = std::make_unique<StartEvent>(m_sim, m_dispatch);
    m_sim.scheduleEvent(std::move(e));
    m_sim.runNextEvent();
}

void App::reset()
{
    m_sim.reset();
    m_log = TrainLog{};
    m_carLog = CarLog{};
    initialize();
}

void App::setStartTime()
{
    auto time = get<std::string>("Enter new start time (hh:mm): ");
    m_sim.setStartTime(time);
}

void App::setEndTime()
{
    auto time = get<std::string>("Enter new end time (hh:mm): ");
    m_sim.setEndTime(time);
}

void App::printStartAndEndTimes()
{
    print("Current start time: ");
    println(m_sim.startTime());
    print("Current end time: ");
    println(m_sim.endTime());
}

void App::printInterval()
{
    print("Current interval: ");
    println(m_sim.interval());
}

void App::printCurrentLogLevel()
{
    println("Log level: " + m_printer.logLevelAsString() + "\n");
}

void App::printCurrentTime()
{
    print("Current time: ");
    println(m_sim.currentTime());
}

void App::printNewTime()
{
    print("\nTime is now ");
    println(m_sim.currentTime());
}

void App::changeInterval()
{
    auto time = get<int>("Enter new interval in minutes: ");
    if (time < 0) {
        throw std::out_of_range("Time must not be negative!");
    }
    m_sim.setInterval(time);
}

void App::runNextInterval()
{
    const auto start = m_sim.currentTime();
    const auto stop = m_sim.nextStopTime();
    m_sim.runNextInterval();
    printHistory(start, stop);
    printNewTime();
    logIfFinished();
    waitForEnter();
}

void App::logIfFinished()
{
    if (simulationIsFinished()) {
        writeLogToFile();
    }
}

void App::printHistory(time::TimeOfDay start, time::TimeOfDay end)
{
    auto records = m_log.view(start, end);
    m_printer.print(std::begin(records), std::end(records));
}

void App::runNextEvent()
{
    m_sim.runNextEvent();
    printLast();
    printNewTime();
    logIfFinished();
    waitForEnter();
}

void App::runUntilComplete()
{
    const auto start = m_sim.currentTime();
    m_sim.runToCompletion();
    const auto stop = m_sim.currentTime();
    printHistory(start, stop);
    printNewTime();
    writeLogToFile();
    waitForEnter();
}

void App::printLast()
{
    auto last = m_log.viewLast();
    m_printer.print(last);
}

bool App::simulationIsFinished()
{
    return m_sim.isFinished();
}

void App::setLogLevel()
{
    clearScreen();
    println("Set log level");
    print("1. low\n2. medium\n3. high\n");
    const auto choice = get<int>("> ");
    m_printer.setLogLevel(choice);
}

void App::showTrainByNumber()
{
    clearScreen();
    m_printer.println("Find train by number");
    const auto nbr = get<int>("Enter number: ");
    const auto train = m_dispatch.viewTrain(nbr);
    showTrain(train);
    waitForEnter();
}

void App::showTrainByVehicleId()
{
    clearScreen();
    println("Find train by vehicle id..:");
    const auto id = get<int>("Enter id: ");
    try {
        const auto train = m_dispatch.viewTrainByVehicleId(id);
        showTrain(train);
        waitForEnter();
    }
    catch (const std::out_of_range& e) {
        println(e.what());
        waitForEnter();
    }
}

void App::showAllTrains()
{
    clearScreen();
    for (const auto nbr: m_dispatch.trainNumbers()) {
        const auto train = m_dispatch.viewTrain(nbr);
        showTrain(train);
    }
    waitForEnter();
}

void App::showTrain(const train::Train& train)
{
    m_printer.print(train);
    if (logLevel() >= LogLevel::high) {
        m_printer.print("");
        m_printer.printAttachedVehicles(train);
        m_printer.print("");
        m_printer.printMissingVehicles(train);
    }
}

LogLevel App::logLevel() const
{
    return m_printer.logLevel();
}

void App::showStationNames()
{
    clearScreen();
    m_printer.println("All station names: ");
    m_printer.println("---");
    showAllStationNames();
    waitForEnter();
}

void App::showAllStationNames()
{
    const auto& names = m_dispatch.stationNames();
    auto beg = std::begin(names);
    const auto end = std::end(names);
    for (auto i = 1; beg != end; ++beg, ++i) {
        const auto idx = std::to_string(i);
        m_printer.println(idx + ". " + *beg);
    }
}

void App::showAllStations()
{
    m_printer.println("All stations: ");
    m_printer.println("---");
    for (const auto name: m_dispatch.stationNames()) {
        const auto station = m_dispatch.viewStation(name);
        m_printer.print(station);
    }
    waitForEnter();
}

void App::showStationByName()
{
    clearScreen();
    m_printer.println("Show station by name");
    m_printer.println("---");
    showAllStationNames();
    const auto choice = get<int>("Enter nbr: ");
    try {
        const auto name = m_dispatch.stationNames().at(choice - 1);
        const auto station = m_dispatch.viewStation(name);
        clearScreen();
        m_printer.print(station);
        waitForEnter();
    }
    catch (const std::out_of_range&) {
        throw std::out_of_range("No such option!");
    }
}

void App::showVehicleById()
{
    clearScreen();
    println("Show vehicle by id...");
    const auto id = get<int>("Enter id: ");
    const auto car = m_dispatch.viewCar(id);

    m_printer.println();
    m_printer.printCarWithLocation(*car);
    if (m_printer.logLevel() >= LogLevel::medium) {
        showVehicleHistory(id);
    }
    waitForEnter();
}

void App::showAllVehicles()
{
    println("Showing all vehicles...");
    for (const auto& car: m_dispatch.viewAllCars()) {
        m_printer.printCarWithLocation(*car);
    }
    waitForEnter();
}

void App::showVehicleHistory(const int carId)
{
    m_printer.println("Travel history (arrivals):");
    try {
        const auto history = m_carLog.viewRecordOf(carId);
        for (const auto& record: history) {
            m_printer.print(record);
        }
    }
    catch (const std::out_of_range&) {
        println("[no history recorded]");
    }
}

void App::printStatistics()
{
    clearScreen();
    m_printer.println("Number of vehicles at start is simulation:");
    m_printer.println("-----------------------------------------");
    for (const auto& station: m_initialStationStates) {
        const auto vehicleCount = std::to_string(station.carCount());
        m_printer.println(station.name() + " = "s + vehicleCount);
    }
    m_printer.println();

    m_printer.println("Time information:");
    m_printer.println("----------------");
    m_printer.print("Total departure delay time: ");
    IO::println(m_dispatch.totalDepartureDelay());
    m_printer.print("Total arrival delay time: ");
    IO::println(m_dispatch.totalArrivalDelay());

    m_printer.print("End time for simulation: ");
    IO::println(m_sim.currentTime());
    m_printer.println();


    m_printer.println("Trains that never left the station: ");
    m_printer.println("----------------------------------");
    for (const auto nbr: m_dispatch.nonDepartedTrainNumbers()) {
        const auto train = m_dispatch.viewTrain(nbr);
        showTrain(train);
        m_printer.println();
    }
    m_printer.println();

    m_printer.println("Delayed trains: ");
    m_printer.println("--------------");
    for (const auto nbr: m_dispatch.delayedTrainNumbers()) {
        const auto train = m_dispatch.viewTrain(nbr);
        showTrain(train);
        m_printer.println();
    }

    waitForEnter();
}

void App::writeLogToFile()
{
    using namespace std::string_literals;
    const auto filename = "Trainsim.log"s;
    auto file = std::ofstream(filename);
    if (!file) {
        throw std::runtime_error("Could not write to " + filename);
    }

    auto& old = m_printer.ostream();
    m_printer.setOstream(file);
    m_printer.print(m_sim.startTime().asString());
    m_printer.println(": STARTING SIMULATION");
    printHistory(m_sim.startTime(), m_sim.currentTime());
    m_printer.println();
    m_printer.print(m_sim.currentTime().asString());
    m_printer.println(": ENDING SIMULATION");
    m_printer.setOstream(old);

    println("\nWrote log to "s + filename);
}

};  // namespace pabo::app
