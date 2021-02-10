#include <stdio.h>

#ifdef _WIN32
    inline int getchar_unlocked() { return _getchar_nolock(); }
#endif
#include "menu.h"
#include <stdexcept>


menu::menu() {
    this->systemMonitor = new SystemMonitor();
    mainMenu();
    systemMonitor->~SystemMonitor();
    free(systemMonitor);
}

void menu::mainMenu() {
    int c;
    bool firstLoop = true;

    while (true) {

        cout << "\nMAIN MENU\n"
             << "\nPlease enter number:\n"
             << "1 - EMPLOYEE\n"
             << "2 - MANAGER\n"
             << "3 - CLIENT\n"
             << "0 - Save and quit\n";

        if (!firstLoop)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        cin.clear();
        c = getchar_unlocked();

        firstLoop = false;


        switch (c) {
            case employee:
                monitorEmployee();
                break;
            case manager:
                monitorManager();
                break;
            case client:
                clientManager();
                break;
            case quit:
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}

void menu::monitorEmployee() {
    Employee *employee = this->systemMonitor->loginEmployee();
    if (employee == nullptr)
        return;

    Lane *lane = systemMonitor->findEmployeeLane(employee);
    if (lane == nullptr) {
        cout << "EMPLOYEE DOESN'T WORK AT ANY LANE";
        return;

    }

    Toll *toll = systemMonitor->findLaneToll(lane);

    Highway *highway = systemMonitor->findTollHighway(toll);

    while (true) {
        cout << "\nEMPLOYEE MENU:\n\n"
             << "HIGHWAY: " << highway->getName() << "\n"
             << "TOLL: " << toll->getName() << "\n"
             << "THERE'S " << lane->numberVehiclesWaiting() << " VEHICLES WAITING IN YOUR LANE\n\n";
        if (lane->numberVehiclesWaiting() > 0) {
            cout << "\nPLEASE ENTER NUMBER:\n"
                 << "1 - PASS NEXT VEHICLE\n"
                 << "0 - GO BACK\n\n";
        } else {
            cout << "\nPLEASE ENTER NUMBER:\n"
                 << "NO VEHICLES TO PASS\n"
                 << "0 - GO BACK\n\n";
        }
        string lp;
        switch (SystemMonitor::getNumberInput()) {
            case '1':
                lp = lane->passVehicle();
                if (lp != "0") {
                    systemMonitor->getVehicle(lp)->endTrip(toll, new Time);
                    lane->addCrossing();
                    cout << "VEHICLE PASSED";
                } else cout << "NO VEHICLE'S TO PASS";
                break;
            case back:
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}

void menu::monitorManager() {
    while (true) {
        cout << "\nMANAGER MENU\n"
             << "\nPlease enter number:\n"
             << "1 - MANAGE HIGHWAYS\n"
             << "2 - MANAGE EMPLOYEES\n"
             << "3 - MANAGE ACTIVE CLIENTS (HAD VEHICLE THAT PASSED TOLL)\n"
             << "0 - GO BACK\n";

        switch (SystemMonitor::getNumberInput()) {
            case '1':
                manageHighways();
                break;
            case '2':
                manageEmployees();
                break;
            case '3':
                manageClients();
                break;
            case back:
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}

void menu::clientManager() {
    Client *client = this->systemMonitor->login();
    if (client == nullptr)
        return;

    cout << "WELCOME " << client->getName() << endl;
    while (true) {
        cout << "\nCLIENT MENU\n"
             << "\nPlease enter number:\n"
             << "1 - MANAGE VEHICLES\n"
             << "2 - MANAGE COSTS\n"
             << "3 - PASS TOLLS\n"
             << "4 - MANAGE INFO\n"
             << "0 - GO BACK\n";

        switch (SystemMonitor::getNumberInput()) {
            case '1':
                manageVehicles(client);
                break;
            case '2':
                manageCosts(client);
                break;
            case '3':
                operateToll(client);
                break;
            case '4':
                manageInfo(client);
                break;
            case back:
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}

void menu::manageVehicles(Client *client) {
    while (true) {
        cout << "\nVehicles MENU\n"
             << "\nPlease enter number:\n"
             << "1 - ADD VEHICLES\n"
             << "2 - REMOVE VEHICLES\n"
             << "3 - VIEW VEHICLES AND TRIPS\n"
             << "4 - UPDATE VEHICLES\n"
             << "0 - GO BACK\n";

        switch (SystemMonitor::getNumberInput()) {
            case '1':
                this->systemMonitor->addVehicleClient(client);
                break;
            case '2':
                SystemMonitor::removeVehicle(client);
                break;
            case '3':
                systemMonitor->viewVehicles(client);
                break;
            case '4':
                systemMonitor->updateVehicles(client);
                break;
            case back:
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}

void menu::operateToll(Client *client) {
    while (true) {
        cout << "\nOPERATE TOLL\n"
             << "\nPlease enter number:\n"
             << "1 - ENTER TOLL\n"
             << "2 - EXIT TOLL\n"
             << "0 - GO BACK\n";

        switch (SystemMonitor::getNumberInput()) {
            case entry_toll:
                operatePassToll(client, false);
                break;
            case exit_toll:
                operatePassToll(client, true);
                break;
            case back:
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}

void menu::operatePassToll(Client *client, bool exit) {

    Highway *highway = getHighway();
    if (highway == nullptr) {
        cout << "INVALID HIGHWAY\n";
        return;
    }

    if (highway->getTollsSize(exit) == 0) {
        cout << "NO TOLLS TO SHOW\n";
        return;
    }
    highway->printTollsNumbered(exit);
    Toll *toll = getTollInput(exit, highway);
    if (toll == nullptr) {
        cout << "INVALID TOLL\n";
        return;
    }

    if (toll->getLanes().empty()) {
        cout << "TOLL EMPTY\n";
        return;
    }

    string licensePlate;
    Vehicle *vehicle;

    while (true) {
        licensePlate = SystemMonitor::licensePlateInput(); //controla o input
        if (licensePlate == "0") return;
        vehicle = client->getVehicle(licensePlate);
        if (vehicle == nullptr) {
            cout << "YOU DO NOT HAVE THIS VEHICLE REGISTERED, PLEASE ADD VEHICLE AND TRY AGAIN LATER\n";
            continue;
        }
        Lane *lane = toll->getRecommendedLane(vehicle->isViaVerde());

        if (lane == nullptr) {
            if (vehicle->isViaVerde())
                cout << "NO VIA VERDE LANE\n";
            else
                cout << "NO NORMAL LANE\n";

            continue;
        }
        if (!exit) {
            lane->addVehicle(vehicle->getLicensePlate(), 0.0);
            vehicle->startTrip(toll, new Time());
            HashTableClient activeClients = systemMonitor->getActiveClients();
            activeClients.insert(client);
            systemMonitor->setActiveClients(activeClients);
            lane->addCrossing();
            return;
        } else {
            double price; //CALCULAR PREÇO
            Trip *lastTrip = vehicle->getLastTrip();
            if (lastTrip == nullptr || lastTrip->isFinished()) {
                cout << "THIS VEHICLE IS NOT IN TRANSIT, CANNOT EXIT\n";
                continue;
            }
            if (vehicle->isViaVerde()) {
                vehicle->endTrip(toll, new Time());
                lane->addCrossing();
                return;
            }

            lane->addVehicle(vehicle->getLicensePlate(), price);
            lane->addCrossing();
            return;
        }

    }
}

Toll *menu::getTollInput(bool exit, Highway *highway) {
    cout << "CHOOSE TOLL\n";
    int tollNum;
    cin >> tollNum;
    Toll *toll = highway->getTollAt(tollNum, exit);
    return toll;
}

Highway *menu::getHighway() {
    systemMonitor->printHighwaysNumbered();
    cout << "CHOOSE HIGHWAY\n";
    int highwayNum;
    cin >> highwayNum;
    Highway *highway = systemMonitor->getHighwayAt(highwayNum - 1);
    return highway;
}

void menu::manageCosts(Client *client) {
    while (true) {
        systemMonitor->showCostsClient(client);

        cout << "ENTER 0 TO GO BACK" << endl;
        if (SystemMonitor::getNumberInput() == '0') {
            return;
        }
    }

}

void menu::manageInfo(Client *client) {
    while (true) {
        cout << "\nCLIENT INFO:\n\n";
        client->printInfo();
        cout << "\nPlease enter number:\n"
             << "1 - CHANGE NAME\n"
             << "2 - CHANGE NIF\n"
             << "0 - GO BACK\n";
        switch (SystemMonitor::getNumberInput()) {
            case '1':
                SystemMonitor::changeName(client);
                break;
            case '2':
                systemMonitor->changeNIF(client);
                break;
            case back:
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}

void menu::manageHighways() {
    Highway *highway;
    while (true) {
        cout << "\nHIGHWAY MANAGER MENU:\n\n";
        cout << "\nPlease enter number:\n"
             << "1 - ADD HIGHWAY\n"
             << "2 - REMOVE HIGHWAY\n"
             << "3 - CHANGE EXISTING HIGHWAY(ADD OR REMOVE TOLLS,LANES,ETC)\n"
             << "4 - VIEW HIGHWAY LIST\n"
             << "0 - GO BACK\n";
        switch (SystemMonitor::getNumberInput()) {
            case '1':
                systemMonitor->managerAddHighway();
                break;
            case '2':
                systemMonitor->managerRemoveHighway();
                break;
            case '3':

                if (systemMonitor->getHighways().empty()) {
                    cout << "NO HIGHWAYS TO MANAGE" << endl;
                    return;
                }

                highway = systemMonitor->getHighwayAt((systemMonitor->selectHighway()));
                manageExistingHighways(highway);
                break;
            case '4':
                systemMonitor->managerViewHighways();
            case '0':
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }

}

void menu::manageExistingHighways(Highway *highway) {

    while (true) {
        cout << "\nHIGHWAY MANAGER MENU:\n\n";
        cout << "\nPlease enter number:\n"
             << "1 - ADD ENTRY TOLL\n"
             << "2 - ADD EXIT TOLL\n"
             << "3 - REMOVE TOLL\n"
             << "4 - MANAGE TOLLS\n"
             << "5 - VIEW HIGHWAY TOLLS\n"
             << "0 - GO BACK\n";
        switch (SystemMonitor::getNumberInput()) {
            case '1':
                systemMonitor->managerAddToll(highway, false);
                break;
            case '2':
                systemMonitor->managerAddToll(highway, true);
                break;
            case '3':
                SystemMonitor::managerRemoveToll(highway);
                break;
            case '4':
                managerManageToll(highway);
                break;
            case '5':
                SystemMonitor::viewHighwayTolls(highway);
                break;
            case '0':
                return;
            default:
                cout << "\nPlease enter another number\n";

        }
    }
}

void menu::managerManageToll(Highway *highway) {
    Toll *toll = SystemMonitor::selectToll(highway);
    while (true) {

        if (toll->isExitToll()) {
            cout << "\nEXIT-TOLL MANAGER MENU:\n\n";
            cout << "\nPlease enter number:\n"
                 << "1 - ADD NORMAL EXIT LANE\n"
                 << "2 - ADD VIA VERDE EXIT LANE\n"
                 << "3 - CHANGE EXIT LANE EMPLOYEES\n"
                 << "4 - REMOVE LANE\n"
                 << "5 - VIEW EXIT LANES AND PAST EMPLOYEES\n"
                 << "6 - VIEW TOLL PAST EMPLOYEES\n"
                 << "7 - REPAIR MENU\n"
                 << "8 - ADD TECHNICIAN\n"
                 << "0 - GO BACK\n";
        } else {
            cout << "\nENTRY-TOLL MANAGER MENU:\n\n";
            cout << "\nPlease enter number:\n"
                 << "1 - ADD NORMAL LANE\n"
                 << "2 - ADD VIA VERDE LANE\n"
                 << "3 - REMOVE LANE\n"
                 << "4 - VIEW ENTRY LANES\n"
                 << "5 - REPAIR MENU\n"
                 << "6 - ADD TECHNICIAN\n"
                 << "0 - GO BACK\n";
        }
        if (!toll->isExitToll()) {
            switch (SystemMonitor::getNumberInput()) {
                case '1':
                    systemMonitor->managerAddLane(toll, false);
                    break;
                case '2':
                    systemMonitor->managerAddLane(toll, true);
                    break;
                case '3':
                    SystemMonitor::removeLane(toll);
                    break;
                case '4':
                    toll->viewLanes();
                    break;
                case '5':
                    repairToll(highway, toll);
                    break;
                case '6':
                    systemMonitor->addTech(toll);
                case '0':
                    return;
                default:
                    cout << "\nPlease enter another number\n";
            }
        } else {
            switch (SystemMonitor::getNumberInput()) {
                case '1':
                    systemMonitor->managerAddLane(toll, false);
                    break;
                case '2':
                    systemMonitor->managerAddLane(toll, true);
                    break;
                case '3':
                    systemMonitor->changeLaneEmployee(toll);
                    break;
                case '4':
                    SystemMonitor::removeLane(toll);
                    break;
                case '5':
                    toll->viewLanes();
                    break;
                case '6':
                    SystemMonitor::viewLastEmployees(toll);
                    break;
                case '7':
                    repairToll(highway, toll);
                    break;
                case '8':
                    systemMonitor->addTech(toll);
                case '0':
                    return;
                default:
                    cout << "\nPlease enter another number\n";
            }
        }
    }
}

void menu::repairToll(Highway *pHighway, Toll *pToll) {
    double duration; int choice;
    cout << "\nTOLL REPAIR MENU:\n\n";
    cout << "\nCHOOSE THE TYPE OF REPAIR:\n"
         << "1 - REVISION\n"
         << "2 - ELECTRONIC REPAIR\n"
         << "3 - INFORMATIC REPAIR\n"
         << "0 - GO BACK\n";
    while (true) {
        cin >> choice;
        if(choice>0&&choice<4) {
            if(pHighway->techExistsInHighway(choice-1)) {
                Intervention *i = systemMonitor->scheduleIntervention(pToll, choice-1);
                cout << "HOW LONG DID THE INTERVENTION LAST? (IN HOURS)\n";
                cin >> duration;
                while(1) {
                    if(duration<=0){
                        cout<<"ENTER A VALID NUMBER: \n";
                        cin>>duration;
                    }
                    else{
                        break;
                    }
                }
                systemMonitor->completeIntervention(i, duration);
                pHighway->repair(choice - 1, pToll, i);
                return;
            }
            else cout <<"NO TECHNICIAN WITH THAT SPECIALTY IS AVAILABLE\n";
        }
        else if(choice==0){
            return;
        }
        else{
            cout << "\nPlease enter a valid number\n";
        }
    }
}

void menu::manageEmployees() {
    while (true) {
        cout << "\nEMPLOYEE MANAGE MENU:\n\n";
        cout << "\nPlease enter number:\n"
             << "1 - ADD EMPLOYEE\n"
             << "2 - REMOVE EMPLOYEE\n"
             << "3 - CHANGE EMPLOYEE'S WORK LANE\n"
             << "4 - VIEW EMPLOYEE LIST\n"
             << "0 - GO BACK\n";
        switch (SystemMonitor::getNumberInput()) {
            case '1':
                systemMonitor->managerAddEmployee();
                break;
            case '2':
                systemMonitor->managerRemoveEmployee();
                break;
            case '3':
                systemMonitor->changeEmployeeLane();
                break;
            case '4':
                systemMonitor->viewEmployees();
                break;
            case '0':
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}

void menu::manageClients() {
    while (true) {
        cout << "\nACTIVE CLIENTS MANAGE MENU:\n\n";
        cout << "\nPlease enter number:\n"
             << "1 - SEARCH CLIENT LIST\n"
             << "2 - VIEW CLIENT LIST\n"
             << "0 - GO BACK\n";
        switch (SystemMonitor::getNumberInput()) {
            case '1':
                searchActiveClients();
                break;
            case '2':
                systemMonitor->viewAllActiveClients();
                break;
            case '0':
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}

void menu::searchActiveClients() {
    while (true) {
        cout << "\nSEARCH ACTIVE CLIENTS MENU:\n\n";
        cout << "\nPlease enter number:\n"
             << "1 - SEARCH CLIENT LIST BY NIF\n"
             << "2 - SEARCH CLIENT LIST BY NAME\n"
             << "3 - SEARCH CLIENT LIST BY NUMBER OF VEHICLES\n"
             << "0 - GO BACK\n";
        switch (SystemMonitor::getNumberInput()) {
            case '1':
                systemMonitor->searchActiveClientsByNIF();
                break;
            case '2':
                systemMonitor->searchActiveClientsByName();
                break;
            case '3':
                systemMonitor->searchActiveClientsByNumVehicles();
                break;
            case '0':
                return;
            default:
                cout << "\nPlease enter another number\n";
        }
    }
}