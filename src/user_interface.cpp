#include "console_IO.h"
#include "user_interface.h"
#include <exception>  // exception
#include <iostream>
#include <stdexcept>  // logic_error

namespace pabo::app {
using namespace IO;

void printExceptionError(const std::exception&);

UserInterface::UserInterface()
{
    setUpStartMenu();
    setUpSimMenu();
    setUpTrainMenu();
    setUpStnMenu();
    setUpVehicleMenu();
    app.initialize();
}

void UserInterface::setUpStartMenu()
{
    startMenu.addItem("Change start time", [this] {
        app.setStartTime();
    });

    startMenu.addItem("Change end time", [this]() {
        app.setEndTime();
    });

    startMenu.addItem("Run simulation", [this]() {
        app.reset();
        app.start();
        enableSimOptions();
        disableStatOptions();
        runSimulationMenu();
    });
}

void UserInterface::runSimulationMenu()
{
    while (simMenu.isRunning()) {
        try {
            runSimMenuOnce();
        }
        catch (const std::logic_error& e) {
            println("\nError: "s + e.what());
            waitForEnter();
        }
    }
    simMenu.reset();
}

void UserInterface::runSimMenuOnce()
{
    clearScreen();
    app.printInterval();
    app.printCurrentTime();
    app.printCurrentLogLevel();

    simMenu.runOnce();

    if (app.simulationIsFinished()) {
        disableSimOptions();
        enableStatOptions();
    }
}

void UserInterface::disableSimOptions()
{
    simMenu.disableItem("Change interval");
    simMenu.disableItem("Run next interval");
    simMenu.disableItem("Run next event");
    simMenu.disableItem("Finish (complete simulation)");
}

void UserInterface::enableSimOptions()
{
    simMenu.enableItem("Change interval");
    simMenu.enableItem("Run next interval");
    simMenu.enableItem("Run next event");
    simMenu.enableItem("Finish (complete simulation)");
}

void UserInterface::enableStatOptions()
{
    simMenu.enableItem("Print statistics");
}

void UserInterface::disableStatOptions()
{
    simMenu.disableItem("Print statistics");
}

void UserInterface::setUpSimMenu()
{
    simMenu.addItem("Change interval", [this]() {
        app.changeInterval();
    });

    simMenu.addItem("Run next interval", [this]() {
        app.runNextInterval();
    });

    simMenu.addItem("Run next event", [this]() {
        app.runNextEvent();
    });

    simMenu.addItem("Finish (complete simulation)", [this]() {
        app.runUntilComplete();
    });

    simMenu.addItem(
            "Print statistics", [this]() {
                app.printStatistics();
            },
            false);

    simMenu.addItem("Change log level", [this]() {
        app.setLogLevel();
    });

    simMenu.addItem("Train menu", [this]() {
        runMenuWithLogLevelInfo(trainMenu);
    });

    simMenu.addItem("Station menu", [this]() {
        runMenuWithLogLevelInfo(stnMenu);
    });

    simMenu.addItem("Vehicle menu", [this]() {
        runMenuWithLogLevelInfo(vehicleMenu);
    });
}

void UserInterface::setUpTrainMenu()
{
    trainMenu.addItem("Show train by number", [this]() {
        app.showTrainByNumber();
    });

    trainMenu.addItem("Show train by vehicle id", [this]() {
        app.showTrainByVehicleId();
    });

    trainMenu.addItem("Show all trains", [this]() {
        app.showAllTrains();
    });

    trainMenu.addItem("Change log level", [this]() {
        app.setLogLevel();
    });
}

void UserInterface::setUpStnMenu()
{
    stnMenu.addItem("Show station names", [this]() {
        app.showStationNames();
    });

    stnMenu.addItem("Show station by name", [this]() {
        app.showStationByName();
    });

    stnMenu.addItem("Show all stations", [this]() {
        app.showAllStations();
    });

    stnMenu.addItem("Change log level", [this]() {
        app.setLogLevel();
    });
}

void UserInterface::setUpVehicleMenu()
{
    vehicleMenu.addItem("Show vehicle by id", [this]() {
        app.showVehicleById();
    });

    vehicleMenu.addItem("Show all vehicles", [this]() {
        app.showAllVehicles();
    });

    vehicleMenu.addItem("Change log level", [this]() {
        app.setLogLevel();
    });
}

void UserInterface::run()
{
    // This loop is a bit more specific so write it manually.
    while (startMenu.isRunning()) {
        try {
            runStartMenuOnce();
        }
        catch (const std::exception& e) {
            if (std::cin.eof()) {
                throw std::runtime_error("Read eof from cin!");
            }
            else {
                printExceptionError(e);
            }
        }
    }
}

void UserInterface::runStartMenuOnce()
{
    clearScreen();
    app.printStartAndEndTimes();
    println("");

    startMenu.runOnce();
}

void UserInterface::runMenuWithLogLevelInfo(Menu& menu)
{
    while (menu.isRunning()) {
        clearScreen();
        app.printCurrentLogLevel();
        menu.runOnceOrPrintError();
    }
    menu.reset();
}

void printExceptionError(const std::exception& e)
{
    println("\nOops... "s + e.what());
    waitForEnter();
}

}  // namespace pabo::app
