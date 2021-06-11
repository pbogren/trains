/**
    @file include/user_interface.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Definition of the UserInterface class.
*/

#ifndef INCLUDE_USER_INTERFACE_H
#define INCLUDE_USER_INTERFACE_H

#include "trains_app.h"
#include "ui_menu.h"
#include <string>

namespace pabo::app {

using namespace std::string_literals;

class UserInterface {
public:
    UserInterface();
    void run();

private:
    void setUpStartMenu();
    void runSimulationMenu();
    void runSimMenuOnce();
    void enableSimOptions();
    void disableSimOptions();
    void enableStatOptions();
    void disableStatOptions();

    void setUpSimMenu();
    void setUpTrainMenu();
    void setUpStnMenu();
    void setUpVehicleMenu();
    void runMenuWithLogLevelInfo(Menu& menu);

    void runStartMenuOnce();
    void printStatistics();

    TrainsApp app;

    app::Menu startMenu{"Start menu"s};
    app::Menu simMenu{"Simulation menu"s};
    app::Menu trainMenu{"Train menu"s};
    app::Menu stnMenu{"Station menu"s};
    app::Menu vehicleMenu{"Vehicle menu"s};
};

}  // namespace pabo::app

#endif
