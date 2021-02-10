#ifndef AEDA_T8G6_MENU_H
#define AEDA_T8G6_MENU_H

#include <iostream>
#include "systemMonitor.h"

using namespace std;

///Class used for user interface
///
///@see systemMonitor
class menu {
private:
    SystemMonitor *systemMonitor;

    ///Displays main menu
    ///
    ///1 - EMPLOYEE calls monitorEmployee()\n
    ///2 - MANAGER calls monitorManager()\n
    ///3 - CLIENT calls clientManager()\n
    void mainMenu();

    ///Starts Employee monitor
    ///
    ///Allows login by calling SystemMonitor::loginEmployee()
    ///Then displays information about the Lane where the Employee is working
    ///And allows to pass Vehicles
    void monitorEmployee();

    ///Starts Manager monitor
    ///
    ///Allows manager to manage Highways or Employees\n
    ///1 - MANAGE HIGHWAYS calls manageHighways()\n
    ///2 - MANAGE EMPLOYEES calls manageEmployees()\n
    void monitorManager();

    /// Allows Client to operate toll
    ///
    /// \param client
    /// @see operatePassToll(Client *client, bool exit)
    void operateToll(Client *client);


public:
    ///Initiates SystemMonitor and displays menu
    ///
    ///Calls mainMenu()
    ///@see SystemMonitor
    ///@see mainMenu()
    menu();

    ///Starts Client Monitor
    ///
    /// 1 - MANAGE VEHICLES\n
    /// 2 - MANAGE COSTS\n
    /// 3 - PASS TOLLS\n
    /// 4 - MANAGE INFO\n
    void clientManager();

    ///Allows Client to manage their Vehicles
    ///
    /// \param client 
    void manageVehicles(Client *client);

    ///Allows Client to start or end a Trip
    ///
    /// \param client
    /// \param exit
    /// @see getHighway() @see getTollInput(bool exit, Highway *highway)
    void operatePassToll(Client *client, bool exit);

    /// Displays Client's Trips and their costs
    ///
    /// Calls systemMonitor::showCostsClient(Client *client)
    /// \param pClient

    ///@see systemMonitor::showCostsClient(Client *client)
    void manageCosts(Client *pClient);

    /// Allows Client to manage their info
    ///
    /// Client is able to change name and/or NIF
    /// \param client
    void manageInfo(Client *client);

    ///Displays Highway Management Menu
    ///
    ///1 - ADD HIGHWAY\n
    ///2 - REMOVE HIGHWAY\n
    ///3 - CHANGE EXISTING HIGHWAY(ADD OR REMOVE TOLLS,LANES,ETC)\n
    ///4 - VIEW HIGHWAY LIST\n
    void manageHighways();

    ///Allows Manager to change Existing Highways
    ///
    /// \param highway
    ///It allows Manager to manage Highways' Tolls and Lanes
    void manageExistingHighways(Highway* highway);

    ///Allows Manager to change Toll information
    ///
    /// \param highway
    void managerManageToll(Highway *highway);

    ///Displays Highways numbered and allows user to choose a Highway
    ///
    ///Calls SystemMonitor::printHighwaysNumbered()
    /// \return pointer to chosen Highway
    Highway *getHighway();

    ///Displays Tolls numbered and allows user to choose a Toll
    ///
    /// Calls
    /// \param exit this is true for exit Toll, false otherwise
    /// \param highway Highway where Toll is located
    /// \return pointer to chosen Toll
    static Toll *getTollInput(bool exit, Highway *highway) ;

    ///Displays Employee Manangement Menu
    ///
    ///1 - ADD EMPLOYEE\n
    ///2 - REMOVE EMPLOYEE\n
    ///3 - CHANGE EMPLOYEE'S WORK LANE\n
    ///4 - VIEW EMPLOYEE LIST\n
    void manageEmployees();

    void manageClients();

    void searchActiveClients();
  
    void repairToll(Highway *pHighway, Toll *pToll);
};

enum main_menu {
    quit = '0', employee, manager, client
};
enum employee_menu {
    back = '0', operate
};

enum operate_toll {
    entry_toll = '1', exit_toll
};


#endif //AEDA_T8G6_MENU_H
