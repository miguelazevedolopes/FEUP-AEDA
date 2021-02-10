#include "systemMonitor.h"

#ifdef _WIN32
inline int getchar_unlocked() { return _getchar_nolock(); }
#endif

int SystemMonitor::findEmployee(const Employee *employee) {
    return sequentialSearch(employees, employee);
}

Vehicle *SystemMonitor::findVehicleClients(const string &licensePlate) {
    int pos;
    if (!clients.empty())
        for (auto &client : clients) {
            pos = sequentialSearch(client->getVehicles(), new Vehicle(licensePlate));
            if (pos != -1)
                return client->getVehicles()[pos];
        }
    return nullptr;
}

int SystemMonitor::findVehicle(const string &licensePlate) {
    return sequentialSearch(vehicles, new Vehicle(licensePlate));
}

int SystemMonitor::findClient(const Client *client) {
    return sequentialSearch(clients, client);
}


void SystemMonitor::save() {
    string vehiclesFileName = "vehicles.txt";
    string employeesFileName = "employees.txt";
    string clientsFileName = "clients.txt";
    string activeClientsFileName = "activeClients.txt";
    string tollsFileName = "tolls.txt";
    string interventionsFileName = "interventions.txt";

    saveInterventionsBST(interventionsFileName);
    saveVectorToFile(tollsFileName, highways);
    saveVectorToFile(employeesFileName, employees);

    vector<Client *> activeClientsVec = getActiveClientsVector();
    saveVectorToFile(activeClientsFileName, activeClientsVec);
    saveVectorToFile(clientsFileName, clients);
    saveVectorToFile(vehiclesFileName, vehicles);
}

template<class T>
void SystemMonitor::saveVectorToFile(const string &vectorFileName, vector<T *> &vec) const {
    ofstream file(vectorFileName);
    if (!vec.empty() && file.is_open())
        for (int i = 0; i < vec.size(); ++i) {
            file << *vec[i];
            if (i != vec.size() - 1)
                file << endl;
        }
    else if (!file.is_open()) throw invalid_argument("Not able to open " + vectorFileName + " file");

    file.close();
}

void SystemMonitor::saveInterventionsBST(string &fileName){
    vector<Intervention> interventionsV = getInterventionsNewestFirst();
    vector<Intervention>::const_iterator it;
    ofstream file(fileName);
    if (!interventionsV.empty() && file.is_open())
    for(it = interventionsV.begin(); it != interventionsV.end(); it++)
    {
        file << *it;
        if (it != interventionsV.end() - 1)
            file << endl;
    }
    else if (!file.is_open()) throw invalid_argument("Not able to open " + fileName + " file");
    file.close();
}

void SystemMonitor::load() {
    string vehiclesFileName = "vehicles.txt";
    string employeesFileName = "employees.txt";
    string clientsFileName = "clients.txt";
    string activeClientsFileName = "activeClients.txt";
    string tollsFileName = "tolls.txt";
    string interventionsFileName = "interventions.txt";

    loadVectorFromFile(vehiclesFileName, vehicles);
    loadVectorFromFile(clientsFileName, clients);
    finishLoadingClients();

    vector<Client *> activeClientsVec;
    loadVectorFromFile(activeClientsFileName, activeClientsVec);
    addActiveClientsVector(activeClientsVec);

    loadVectorFromFile(employeesFileName, employees);
    loadVectorFromFile(tollsFileName, highways);
    finishLoadingLanes();

    loadInterventionsBST(interventionsFileName);
}



Toll* SystemMonitor::findTollInSystem(Toll* toll){
    vector<Highway*>::const_iterator highIt;
    vector<Toll*>::const_iterator tollIt;
    for(highIt = highways.begin(); highIt != highways.end(); highIt++)
    {
        for(tollIt = (*highIt)->getTolls().begin(); tollIt != (*highIt)->getTolls().end(); tollIt++)
        {
            if((*tollIt)->getName() == toll->getName())
                return (*tollIt);
        }
    }
    return NULL;
}

void SystemMonitor::loadInterventionsBST(string &fileName) {
    ifstream file(fileName);
    if (file.is_open()) {
        Intervention *newElement = new Intervention();
        while (file >> *newElement) {
            Toll* toll = new Toll(); toll->setName(newElement->getTollName());
            newElement->setToll(findTollInSystem(toll));
            interventions.insert(*newElement);
            newElement = new Intervention();
        }
        delete newElement;
    } else cout << ("Not able to open " + fileName + " file");
    file.close();
}

template<class T>
void SystemMonitor::loadVectorFromFile(string &vectorFileName, vector<T *> &vec) {
    ifstream file(vectorFileName);
    if (file.is_open()) {
        T *newElement = new T();
        while (file >> *newElement) {
            vec.push_back(newElement);
            newElement = new T();
        }
        delete newElement;
    } else cout << ("Not able to open " + vectorFileName + " file");
    file.close();
}


void SystemMonitor::printHighwaysNumbered() {
    vector<Highway *>::const_iterator it;
    int i = 1;
    for (it = highways.begin(); it != highways.end(); it++) {
        cout << i << ": " << (*it)->getName() << endl;
        i++;
    }
}

Highway *SystemMonitor::getHighwayAt(int i) {
    if (highways.empty() || i < 0 || i > highways.size())
        return nullptr;
    return highways[i];
}

string SystemMonitor::licensePlateInput() {
    string licensePlate;
    cout << "\nENTER LICENSE PLATE "
            "(LICENSE PLATE FORMAT SHOULD BE XX-XX-XX)"
            "\n(OR 0 TO EXIT)\n";
    while (true) {

        cin >> licensePlate;
        for (char &i : licensePlate) i = toupper(i);
        if (licensePlate == "0") return "0";
        if (licensePlate.length() == 8 && licensePlate[2] == '-' && licensePlate[5] == '-') {
            return licensePlate;
        }

        cout << "ENTER A VALID LICENSE PLACE\n(LICENSE PLATE FORMAT SHOULD BE XX-XX-XX)\n";
    }
}

Vehicle *SystemMonitor::getVehicle(const string &licensePlate) {
    int i = findVehicle(licensePlate);
    if (i != -1)
        return vehicles[i];
    return nullptr;
}

void SystemMonitor::addVehicle(Vehicle *vehicle) {
    vehicles.push_back(vehicle);
}

void SystemMonitor::addHighway(Highway *highway) {
    highways.push_back(highway);
}


Client *SystemMonitor::login() {
    int nif, pos;

    for (int i = 0; i < 5; ++i) {
        cout << "Please enter nif: (enter 0 if new client)\n";
        cin >> nif;
        if (nif == 0)
            return createNewClient();
        try {
            if (confirmation()) {

                pos = findClient(new Client(nif));

                if (pos != -1)
                    return clients[pos];
            }
        } catch (ConfirmationExitException &exception) {
            ConfirmationExitException::showMessage();
            return nullptr;
        }
    }

    cout << "TOO MANY TRIES\n";
    return nullptr;
}

Employee *SystemMonitor::loginEmployee() {
    int ss, pos;

    while (true) {
        cout << "ENTER EMPLOYEE SS NUMBER: \n";
        cin >> ss;
        if (countDigit(ss) == 9) break;
        else if (ss == 0) return nullptr;
        cout << "NOT A VALID LOGIN NUMBER. TRY AGAIN OR PRESS 0 TO RETURN\n";
    }
    pos = findEmployee(new Employee("Joao", ss));

    if (pos != -1) return employees[pos];

    return nullptr;
}

void SystemMonitor::addVehicleClient(Client *client) {
    cout << "Enter 0 to cancel and exit\n";

    string licensePlate;
    int category;
    bool viaVerde; //true se tiver via verde

    try {
        category = categoryInput();

        viaVerde = viaVerdeInput();

        licensePlate = getNewLicensePlate();

    } catch (CreatingVehicleException &exception) {
        CreatingVehicleException::showMessage();
        return;
    }
    auto *vehicle = new Vehicle(licensePlate, category, viaVerde);
    client->addVehicle(vehicle);
    addVehicle(vehicle);
}

int SystemMonitor::categoryInput() {
    int category;
    while (true) {
        cout << "\nPLEASE ENTER A VALID VEHICLE CATEGORY. CATEGORY MUST BE A NUMBER BETWEEN 1 AND 5 (0 TO EXIT)\n";
        category = getNumberInput();

        if (category == '0')
            throw CreatingVehicleException();
        if (category > '0' && category < '6')
            return category - '0';
    }
}

string SystemMonitor::getNewLicensePlate() {
    string licensePlate;
    while (true) {
        cout << "Please enter new license plate:\n";
        licensePlate = licensePlateInput();

        if (licensePlate == "0")
            throw CreatingVehicleException();
        else if (findVehicleClients(licensePlate) == nullptr)
            return licensePlate;

        cout << "LICENSE PLATE ALREADY IN USE\n";
    }
}

bool SystemMonitor::viaVerdeInput() {
    int viaVerde;
    while (true) {
        cout << "THIS CAR HAS VIA VERDE?\n"
             << "1 - FALSE (DOES NOT HAVE)\n"
             << "2 - TRUE (HAS)\n"
             << "0 - EXIT AND CANCEL\n";

        viaVerde = SystemMonitor::getNumberInput();

        switch (viaVerde) {
            case '0':
                throw CreatingVehicleException();
            case '1':
                return false;
            case '2':
                return true;
            default:
                break;
        }
    }
}

int SystemMonitor::getNumberInput() {
    cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return getchar_unlocked();
}

Client *SystemMonitor::createNewClient() {
    Client *client;
    string name;
    int nif;

    try {
        name = getNewName();
        nif = getNewNif();
    }
    catch (ConfirmationExitException &exception) {
        ConfirmationExitException::showMessage();
        return nullptr;
    }

    client = new Client(name, nif);
    clients.push_back(client);
    return client;
}

int SystemMonitor::getNewNif() {
    int nif;

    while (true) {
        cout << "ENTER NIF\n";
        cin >> nif;
        if (confirmation() && countDigit(nif) == 9 && findClient(new Client(nif)) == -1)
            break;
        else cout << "ENTER A VALID 9 DIGIT NIF\n";

    }

    return nif;
}

string SystemMonitor::getNewName() {
    string name;

    while (true) {
        cout << "ENTER USERNAME\n";
        cin >> name;
        if (confirmation())
            break;
    }

    return name;
}

bool SystemMonitor::confirmation() {
    while (true) {
        cout << "ENTER:\n"
             << "1 - CONFIRM\n"
             << "2 - ENTER AGAIN\n"
             << "0 - EXIT\n";

        switch (getNumberInput()) {
            case '1':
                return true;
            case '2':
                return false;
            case '0':
                throw ConfirmationExitException();
            default:
                break;
        }

    }
}

int SystemMonitor::countDigit(int n) {
    int cnt = 0;
    while (n != 0) {
        n = n / 10;
        ++cnt;
    }
    return cnt;

}

void SystemMonitor::removeVehicle(Client *client) {
    Vehicle *vehicle;
    string licensePlate;
    while (true) {
        licensePlate = licensePlateInput();
        if (licensePlate == "0") return;
        vehicle = client->getVehicle(licensePlate);
        if (vehicle != nullptr) {
            client->removeVehicle(vehicle);
            return;
        }
        cout << "YOU DO NOT HAVE THIS VEHICLE REGISTERED\n";
    }
}

void SystemMonitor::viewVehicles(Client *client) {

    if (client->getVehicles().empty()) {
        cout << "NO VEHICLES TO SHOW\n";
        return;
    }
    vector<Vehicle *>::iterator it;
    it = client->getVehicles().begin();
    char c;
    while (true) {
        client->printVehicles();
        cout << "SELECT A VEHICLE TO VIEW ITS TRIPS (OR 0 TO GO BACK)" << endl;
        switch (c = (char) getNumberInput()) {
            case '0':
                return;
            default:
                if (showClientVehicleTrips(client, c))
                    return;
        }

    }
}

bool SystemMonitor::showClientVehicleTrips(Client *client, int c) const {
    if (isdigit(c) || c < 1 || c > client->getVehicles().size()) {
        cout << endl;
        client->getVehicles()[(c - '0') - 1]->printTrips();
        cout << endl << endl;
        return true;
    }
    cout << endl << "NOT A VALID INPUT" << endl << endl;
    return false;
}

void SystemMonitor::updateVehicles(Client *client) {
    vector<Vehicle *> vehicles = client->getVehicles();

    while (true) {
        cout << "WHICH VEHICLE DO YOU WANT TO UPDATE THE VIA-VERDE'S STATUS:" << endl;
        client->printVehicles();
        cout << "ENTER A NUMBER (OR PRESS 0 TO RETURN) ";
        switch (char c = getNumberInput()) {
            case '0':
                return;
            default:
                if (isdigit(c)) {
                    Vehicle *vehicle = *vehicles.begin() + (atoi(&c) - 1);

                    updateVehicleViaVerde(vehicle);
                    return;
                } else {
                    cout << endl << "NOT A VALID INPUT" << endl << endl;
                }
                break;
        }
    }
}

void SystemMonitor::updateVehicleViaVerde(Vehicle *vehicle) const {
    cout << endl << (vehicle->isViaVerde()
                     ? "DO YOU WISH TO REMOVE THIS VEHICLE'S VIA VERDE? (PRESS Y/N FOR YES OR NO)"
                     : "DO YOU WISH TO ADD VIA VERDE TO THIS VEHICLE? (PRESS Y/N FOR YES OR NO)");
    if (toupper(getNumberInput()) == 'Y') {
        vehicle->changeViaVerde();
        cout << endl << endl << "DONE" << endl << endl;
        return;
    } else {
        cout << endl << endl << "NO CHANGE WAS DONE" << endl << endl;
        return;
    }
}

void SystemMonitor::showCostsClient(Client *client) {
    if (!(client->getVehicles().empty())) {
        for (auto pVehicle : client->getVehicles()) {
            showCostsVehicle(pVehicle);
        }
    } else cout << "NO VEHICLES TO SHOW" << endl << endl;

}

void SystemMonitor::showCostsVehicle(Vehicle *pVehicle) const {
    cout << "VEHICLE " << (pVehicle)->getLicensePlate() << endl << endl;
    if (!pVehicle->getTrips().empty()) {
        for (auto pTrip : pVehicle->getTrips()) {
            showCostsTrip(pTrip);
        }
    } else {
        cout << "NO TRIPS TO SHOW" << endl << endl;
    }
}

void SystemMonitor::showCostsTrip(const Trip *pTrip) const {
    cout << "FROM: " << (pTrip)->getBegin().first << endl;
    cout << "TO: " << (pTrip)->getEnd().first << endl;
    cout << "WHEN: " << (pTrip)->getEndTime()->getDate() << endl;
    cout << "PRICE PAID: " << (pTrip)->getPrice() << endl << endl;
}

void SystemMonitor::changeNIF(Client *client) {

    int newNif = getNewNif();
    client->changeNIF(newNif);

}

void SystemMonitor::changeName(Client *client) {
    string newName = getNewName();
    client->changeName(newName);

}

Lane *SystemMonitor::findEmployeeLane(Employee *pEmployee) {

    for (auto h:highways) {
        for (auto t:h->getTolls()) {
            for (auto l:t->getLanes()) {

                if (l->getEmployee() == pEmployee) {
                    return l;
                }

            }
        }
    }
    return nullptr;
}

Toll *SystemMonitor::findLaneToll(Lane *lane) {
    for (auto h:highways) {
        for (auto t:h->getTolls()) {
            for (auto l:t->getLanes()) {
                if (l == lane) {
                    return t;
                }
            }
        }
    }
    return nullptr;
}

Highway *SystemMonitor::findTollHighway(Toll *toll) {
    for (auto h:highways) {
        for (auto t:h->getTolls()) {
            if (t == toll) return h;
        }
    }
    return nullptr;

}

void SystemMonitor::managerAddHighway() {

    while (true) {

        cout << "ENTER HIGHWAY NAME\n";
        string name;
        cin >> name;
        try {
            if (confirmation()) {
                addHighway(new Highway(name));
                cout << "HIGHWAY ADDED! \n"
                     << "TO CUSTOMIZE IT GO TO THE \"CHANGE EXISTING HIGHWAY\" MENU\n";
                return;
            } else continue;
        }
        catch (ConfirmationExitException &exception) {
            ConfirmationExitException::showMessage();
            return;
        }
    }

}

void SystemMonitor::managerRemoveHighway() {
    if (!highways.empty()) {
        printHighwaysNumbered();
        while (true) {
            cout << "WHICH HIGHWAY DO YOU WISH TO REMOVE\n";
            int i;
            cin >> i;
            try {
                if (confirmation()) {
                    if (i > 0 && i <= highways.size())
                        highways.erase(highways.begin() + (i - 1));
                    cout << "HIGHWAY ERASED SUCCESSFULLY\n";
                    return;
                } else continue;
            }
            catch (ConfirmationExitException &exception) {
                ConfirmationExitException::showMessage();
                return;
            }
        }
    } else {
        cout << "NO HIGHWAYS TO DISPLAY";
        return;
    }

}

void SystemMonitor::managerViewHighways() {
    printHighwaysNumbered();
    int i;
    while (true) {
        cout << "PRESS 0 TO LEAVE\n";
        cin >> i;
        if (i == 0) return;
    }

}


int SystemMonitor::selectHighway() {
    printHighwaysNumbered();
    int i;
    while (true) {
        cout << "SELECT A HIGHWAY TO MANAGE:\n";
        cin >> i;
        if (i > 0 && i <= highways.size()) {
            return i - 1;
        } else cout << "ENTER A VALID NUMBER\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }

}

void SystemMonitor::managerAddToll(Highway *phighway, bool exit) {

    string name, location;
    int position;
    double price;

    try {
        name = getTollNameInput();

        location = getTollLocationInput();

        position = getTollPositionInput();

        price = getTollPriceInput();
    }
    catch (ConfirmationExitException &exception) {
        ConfirmationExitException::showMessage();
        return;
    }

    vector<Lane *> l;
    if (exit) phighway->addToll(new OutToll(name, location, l, position, price));
    else phighway->addToll(new InToll(name, location, l, position, price));
    cout << "\n\nTOLL ADDED SUCCESSFULLY\n ";
}

double SystemMonitor::getTollPriceInput() {
    while (true) {
        double price;
        cout << "ENTER TOLL PRICE : \n";
        cin >> price;

        try {
            if (confirmation()) {
                return price;
            } else continue;
        }
        catch (ConfirmationExitException &exception) {
            throw ConfirmationExitException();
        }
    }
}

int SystemMonitor::getTollPositionInput() {
    while (true) {
        int position;
        cout << "ENTER TOLL POSITION : \n";
        cin >> position;

        try {
            if (confirmation()) {
                return position;
            } else continue;
        }
        catch (ConfirmationExitException &exception) {
            throw ConfirmationExitException();
        }
    }
}

string SystemMonitor::getTollLocationInput() {
    while (true) {
        string location;
        cout << "ENTER TOLL LOCATION: \n";
        cin >> location;

        try {
            if (confirmation()) {
                return location;
            } else continue;
        }
        catch (ConfirmationExitException &exception) {
            throw ConfirmationExitException();

        }
    }
}

string SystemMonitor::getTollNameInput() {
    while (true) {
        string name;
        cout << "ENTER TOLL NAME: \n";
        cin >> name;

        try {
            if (confirmation()) {
                return name;
            } else continue;
        }
        catch (ConfirmationExitException &exception) {
            throw ConfirmationExitException();
        }
    }
}


vector<Highway *> SystemMonitor::getHighways() {
    return highways;
}

void SystemMonitor::viewHighwayTolls(Highway *phighway) {
    if (phighway->getTolls().empty()) {
        cout << "NO TOLLS TO SHOW\n";
        return;
    }
    phighway->printTollsNumbered();
    int i;
    while (true) {
        cout << "PRESS 0 TO LEAVE\n";
        cin >> i;
        if (i == 0) return;
    }
}

void SystemMonitor::managerRemoveToll(Highway *phighway) {
    if (!phighway->getTolls().empty()) {
        phighway->printTollsNumbered();
        while (true) {
            cout << "WHICH HIGHWAY DO YOU WISH TO REMOVE\n";
            int i;
            cin >> i;
            try {
                if (confirmation()) {
                    if (i > 0 && i <= phighway->getTolls().size()) {
                        phighway->eraseTollAt(i - 1);
                    }
                    cout << "TOLL ERASED SUCCESSFULLY\n";
                    return;
                } else continue;
            }
            catch (ConfirmationExitException &exception) {
                ConfirmationExitException::showMessage();
                return;
            }
        }
    } else {
        cout << "NO TOLLS TO DISPLAY";
        return;
    }
}

Toll *SystemMonitor::selectToll(Highway *pHighway) {
    pHighway->printTollsNumbered();
    int i;
    while (true) {
        cout << "SELECT A TOLL TO MANAGE:\n";
        cin >> i;
        if (i > 0 && i <= pHighway->getTolls().size()) {
            return pHighway->getTolls()[i - 1];
        } else cout << "ENTER A VALID NUMBER\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

void SystemMonitor::managerAddLane(Toll *pToll, bool viaVerde) {
    if (!pToll->isExitToll() && !viaVerde) {
        pToll->addLane(new NormalLane());
        return;
    }
    if (viaVerde) {
        pToll->addLane(new ViaVerdeLane());
        return;
    } else {
        if (pToll->isExitToll()) {
            addNormalExitLane(pToll);
        }
    }
}

void SystemMonitor::addNormalExitLane(Toll *pToll) {
    cout << "DO YOU WANT TO ADD AN EMPLOYEE TO THIS LANE NOW?\n"
            "YOU CAN DO IT LATER AT THE EMPLOYEE MENU (PRESS Y/N FOR YES OR NO)\n";
    char ans;
    Employee *employee = nullptr;
    while (true) {
        cin >> ans;
        if (toupper(ans) == 'Y') {
            employee = selectEmployee();
            if (employee->isWorking()) {
                cout << "THIS EMPLOYEE IS ALREADY WORKING IN A LANE DO YOU WANT TO MOVE HIM?\n";
                try {
                    if (confirmation())
                        removeEmployeeLane(employee);
                    else
                        employee = nullptr;
                } catch (ConfirmationExitException &exception) {
                    ConfirmationExitException::showMessage();
                    return;
                }
            }
        }
        if (toupper(ans) == 'Y' || toupper(ans) == 'N') {
            cout << "YOU CAN ALWAYS ADD OR CHANGE AN EMPLOYER LATER IN THE EMPLOYEE MENU.\n";
            createNormalExitLaneWithEmployee(pToll, employee);
            return;
        }
        if (ans == '0') {
            return;
        }

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "ENTER A VALID INPUT (Y/N) OR 0 TO RETURN\n";
    }
}

void SystemMonitor::removeEmployeeLane(Employee *employee) {
    Lane *oldLane = findEmployeeLane(employee);
    oldLane->setEmployee(nullptr);
}

void SystemMonitor::createNormalExitLaneWithEmployee(Toll *pToll, Employee *employee) const {
    queue<pair<string, double>> vehicleQueue;
    vector<Employee *> lastEmployees;
    employee->changeWorkStatus();
    pToll->addLane(new NormalExitLane(0, vehicleQueue, employee, lastEmployees));
    cout << "LANE WAS CREATED\n\n";
}

void SystemMonitor::printEmployeesNumbered() {
    vector<Employee *>::const_iterator it;
    int i = 1;
    for (it = employees.begin(); it != employees.end(); it++) {
        cout << i << ": " << (*it)->getName() << endl;
        if ((*it)->isWorking()) cout << "WORKING" << endl << endl;
        else cout << "NOT WORKING" << endl << endl;
        i++;
    }
}

void SystemMonitor::printAllActiveClientsNumbered() {
    int i = 1;
    for (const auto &client : activeClients) {
        cout << i << ": " << client->getName() << endl;
        cout << "NIF: " << client->getNif() << endl;
        cout << "NUMBER OF VEHICLES: " << client->getVehicles().size() << endl << endl;
        i++;
    }
}

void SystemMonitor::printActiveClientsNumberedByNIF(int nif) {
    int i = 1;
    for (const auto &client : activeClients) {
        if (client->getNif() == nif) {
            cout << i << ": " << client->getName() << endl;
            cout << "NIF: " << client->getNif() << endl;
            cout << "NUMBER OF VEHICLES: " << client->getVehicles().size() << endl << endl;
            i++;
        }
    }
}

void SystemMonitor::printActiveClientsNumberedByName(const string &name) {
    int i = 1;
    for (const auto &client : activeClients) {
        if (client->getName() == name) {
            cout << i << ": " << client->getName() << endl;
            cout << "NIF: " << client->getNif() << endl;
            cout << "NUMBER OF VEHICLES: " << client->getVehicles().size() << endl << endl;
            i++;
        }
    }
}

void SystemMonitor::printActiveClientsNumberedByNumVehciles(int numVehicles) {
    int i = 1;
    for (const auto &client : activeClients) {
        if (client->getVehicles().size() == numVehicles) {
            cout << i << ": " << client->getName() << endl;
            cout << "NIF: " << client->getNif() << endl;
            cout << "NUMBER OF VEHICLES: " << client->getVehicles().size() << endl << endl;
            i++;
        }
    }
}

Employee *SystemMonitor::selectEmployee() {
    printEmployeesNumbered();
    int i;
    while (true) {
        cout << "SELECT AN EMPLOYEE:\n";
        cin >> i;
        if (i > 0 && i <= employees.size()) {
            return employees[i - 1];
        } else cout << "ENTER A VALID NUMBER\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

void SystemMonitor::removeLane(Toll *pToll) {
    pToll->viewLanes();
    int i;
    while (true) {
        cout << "SELECT A LANE TO ERASE:\n";
        cin >> i;
        if (i > 0 && i <= pToll->getLanes().size()) {
            pToll->removeLaneAt(i - 1);
            return;
        } else cout << "ENTER A VALID NUMBER\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

void SystemMonitor::changeLaneEmployee(Toll *pToll) {
    pToll->viewLanes();

    Lane *lane;
    int i;
    while (true) {
        cout << "SELECT A LANE:\n";
        cin >> i;
        if (i > 0 && i <= pToll->getLanes().size()) {
            lane = pToll->getLanes()[i - 1];
            break;
        } else cout << "ENTER A VALID NUMBER\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    Employee *employee = selectEmployee();
    if (employee->isWorking()) {
        cout << "THIS EMPLOYEE IS ALREADY WORKING IN A LANE DO YOU WANT TO MOVE HIM?\n"
                "(PRESS Y/N FOR YES OR NO)\n";
        char ans;
        while (true) {
            cin >> ans;
            if (toupper(ans) == 'Y') {
                Lane *oldLane = findEmployeeLane(employee);
                oldLane->setEmployee(nullptr);
                if (lane->getEmployee() != nullptr) {
                    lane->getEmployee()->changeWorkStatus();
                }

                //employee->changeWorkStatus();
                lane->setEmployee(employee);
                lane->addToEmployeeList();
                return;
            } else if ((toupper(ans) == 'N') || (ans == '0')) return;
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "ENTER A VALID INPUT (Y/N) OR 0 TO RETURN\n";
        }
    } else {
        if (lane->getEmployee() != nullptr) {
            lane->getEmployee()->changeWorkStatus();
        }

        employee->changeWorkStatus();
        lane->setEmployee(employee);
        lane->addToEmployeeList();
        return;
    }

}

void SystemMonitor::managerAddEmployee() {
    while (true) {

        cout << "ENTER EMPLOYEE NAME\n";
        string name;
        cin >> name;
        try {
            if (confirmation()) {
                cout << "ENTER EMPLOYEE SS NUMBER (9 DIGITS):\n";
                int ss;
                while (true) {
                    cout << "ENTER SS\n";
                    cin >> ss;
                    if (confirmation() && countDigit(ss) == 9 && findEmployee(new Employee(name, ss)) == -1) {
                        employees.push_back(new Employee(name, ss));
                        return;
                    }

                }

            } else continue;
        }
        catch (ConfirmationExitException &exception) {
            ConfirmationExitException::showMessage();
            return;
        }
    }

}

void SystemMonitor::managerRemoveEmployee() {
    int cnt = 1;
    for (auto x:employees) {
        cout << cnt << ": " << x->getName() << endl << endl;
        cnt++;
    }
    while (true) {
        cout << "SELECT AN EMPLOYEE OR PRESS 0 TO RETURN\n";
        cin >> cnt;
        if (cnt < 1 || cnt > employees.size()) {
            cout << "ENTER A VALID INPUT\n";
            cin.clear();
            cin.ignore(10000, '\n');
        } else break;
    }
    employees.erase(employees.begin() + (cnt - 1));
}

void SystemMonitor::changeEmployeeLane() {
    if (employees.empty()) {
        cout << "NO EMPLOYEES TO CHANGE LANE\n";
        return;
    }
    if (highways.empty()) {
        cout << "NO HIGHWAYS TO INSERT EMPLOYEES\n";
        return;
    }
    Employee *employee = selectEmployee();
    Highway *highway = highways[selectHighway()];
    Toll *pToll;
    highway->printTollsNumbered(true);
    int i;
    while (true) {
        cout << "SELECT A TOLL TO MANAGE OR PRESS 0 TO RETURN:\n";
        cin >> i;
        if (i == 0) return;
        if (i > 0 && i <= highway->getTolls().size()) {
            pToll = highway->getTollAt(i, true);
            break;
        } else cout << "ENTER A VALID NUMBER\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    i = 0;
    for (int j = 0; j < pToll->getLanes().size(); j++) {
        cout << "LANE " << j + 1 << endl << "MANUAL EXIT LANE: ";
        if ((pToll->getLanes()[j])->isNormalExitLane()) {
            cout << "YES\n";
        } else cout << "NO\n";
    }
    Lane *lane;

    while (true) {
        cout << "SELECT A LANE OR PRESS 0 TO RETURN:\n";
        cin >> i;
        if (i > 0 && i <= pToll->getLanes().size() && (pToll->getLanes()[i - 1])->isNormalExitLane()) {
            lane = pToll->getLanes()[i - 1];
            break;
        } else if (i == 0) return;
        else if (!(pToll->getLanes()[i - 1])->isNormalExitLane()) cout << "ONLY MANUAL EXIT LANES HAVE EMPLOYEES\n";
        else cout << "ENTER A VALID NUMBER\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    if (employee->isWorking()) {
        cout << "THIS EMPLOYEE IS ALREADY WORKING IN A LANE DO YOU WANT TO MOVE HIM?\n"
                "(PRESS Y/N FOR YES OR NO)\n";
        char ans;
        while (true) {
            cin >> ans;
            if (toupper(ans) == 'Y') {
                Lane *oldLane = findEmployeeLane(employee);
                oldLane->setEmployee(nullptr);
                if (lane->getEmployee() != nullptr) {
                    lane->getEmployee()->changeWorkStatus();
                }
                //employee->changeWorkStatus();
                lane->setEmployee(employee);
                lane->addToEmployeeList();

                return;
            } else if ((toupper(ans) == 'N') || (ans == '0')) return;
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "ENTER A VALID INPUT (Y/N) OR 0 TO RETURN\n";
        }
    } else {

        if (lane->getEmployee() != nullptr) {

            lane->getEmployee()->changeWorkStatus();
        }
        employee->changeWorkStatus();
        lane->setEmployee(employee);
        lane->addToEmployeeList();
        return;
    }
}

void SystemMonitor::viewEmployees() {
    printEmployeesNumbered();
    int i;
    while (true) {
        cout << "PRESS 0 TO LEAVE\n";
        cin >> i;
        if (i == 0) return;
    }
}

void SystemMonitor::viewLastEmployees(Toll *pToll) {
    int cnt = 0;
    for (auto x:pToll->getLanes()) {
        cnt++;
        if (x->isNormalExitLane()) {
            if (x->getLastEmployees().empty()) {
                cout << "LANE " << cnt << ": ";
                cout << "NO EMPLOYEE HISTORY FOR THIS LANE\n\n";
                continue;
            }


            cout << "LANE " << cnt << ":" << endl << endl;
            for (int i = 0; i < x->getLastEmployees().size() - 1; i++) {
                cout << x->getLastEmployees()[i]->getName() << endl;
            }
            cout << "CURRENT EMPLOYEE :";
            if (x->getEmployee() != nullptr) {
                cout << x->getEmployee()->getName() << endl << endl;
            } else cout << "NO EMPLOYEE" << endl << endl;

        } else {
            cout << "LANE " << cnt << ": ";
            cout << "NOT A MANUAL EXIT LANE\n\n";
        }
    }
}

void SystemMonitor::finishLoadingClients() {
    HashTableClient loadActiveClients;
    if (!clients.empty()) {
        for (auto &client : clients) {
            if (!client->getVehicles().empty()) {
                vector<Vehicle *> vecVehicles;
                for (auto &vehic : client->getVehicles()) {
                    Vehicle *vehicle = getVehicle(vehic->getLicensePlate());
                    delete vehic;
                    if (vehicle != nullptr) {
                        vecVehicles.push_back(vehicle);
                        if (!vehicle->getTrips().empty())
                            loadActiveClients.insert(client);
                    } else cout << "Error loading client vehicle!\n";
                }
                client->setVehicles(vecVehicles);
            }
        }
    }
    setActiveClients(loadActiveClients);
}

void SystemMonitor::finishLoadingLanes() {
    if (!employees.empty() && !highways.empty())
        for (auto &highway : highways)
            if (!highway->getTolls().empty())
                for (auto &toll : highway->getTolls())
                    if (!toll->getLanes().empty())
                        for (auto &lane : toll->getLanes()) {
                            if (lane->isNormalExitLane()) {
                                if (lane->getEmployee() != nullptr) {
                                    Employee *employee = getEmployee(lane->getEmployee()->getSsNumber());
                                    if (employee != nullptr)
                                        lane->setEmployee(employee);
                                    else
                                        lane->setEmployee(nullptr);
                                }
                                if (!lane->getLastEmployees().empty()) {
                                    Employee *employee;
                                    vector<Employee *> lastEmployees;
                                    for (int i = 0; i < lane->getLastEmployees().size(); ++i) {
                                        if (lane->getLastEmployees()[i] != nullptr) {
                                            employee = getEmployee(lane->getLastEmployees()[i]->getSsNumber());
                                            delete lane->getLastEmployees()[i];
                                            if (employee != nullptr)
                                                lastEmployees.push_back(employee);
                                        }
                                    }
                                    lane->setLastEmployees(lastEmployees);
                                }
                            }
                        }
}

Employee *SystemMonitor::getEmployee(int ssNumber) {
    int i = sequentialSearch(employees, new Employee(ssNumber));
    if (i == -1)
        return nullptr;
    return employees[i];
}

const HashTableClient &SystemMonitor::getActiveClients() const {
    return activeClients;
}

void SystemMonitor::setActiveClients(const HashTableClient &activeClients) {
    SystemMonitor::activeClients = activeClients;
}

void SystemMonitor::viewAllActiveClients() {
    printAllActiveClientsNumbered();
    int i;
    while (true) {
        cout << "PRESS 0 TO LEAVE\n";
        cin >> i;
        if (i == 0) return;
    }
}

void SystemMonitor::searchActiveClientsByName() {
    string name;
    try {
        name = getNewName();
    } catch (ConfirmationExitException &exception) {
        ConfirmationExitException::showMessage();
        return;
    }
    printActiveClientsNumberedByName(name);
}

void SystemMonitor::searchActiveClientsByNIF() {
    int nif;
    try {
        nif = getNewNif();
    } catch (ConfirmationExitException &exception) {
        ConfirmationExitException::showMessage();
        return;
    }
    printActiveClientsNumberedByNIF(nif);
}

void SystemMonitor::searchActiveClientsByNumVehicles() {
    int numVehicles;
    cout << "ENTER NUMBER OF VEHICLES\n";
    try {
        numVehicles = getNumberInput() - '0';
    } catch (ConfirmationExitException &exception) {
        ConfirmationExitException::showMessage();
        return;
    }
    printActiveClientsNumberedByNumVehciles(numVehicles);
}

vector<Client *> SystemMonitor::getActiveClientsVector() const {
    vector<Client *> activeClientsVec;
    for (const auto &client : activeClients) {
        Client *pClient = new Client();
        *pClient = *client;
        activeClientsVec.push_back(pClient);
    }
    return activeClientsVec;
}

void SystemMonitor::addActiveClientsVector(vector<Client *> activeClientsVec) {
    for (auto &pClient : activeClientsVec) {
        activeClients.insert(pClient);
    }
}

Intervention* SystemMonitor::scheduleIntervention(Toll *toll, int type) {
    Time* time = new Time();
    int date[3];
    date[0] = time->getDay();
    date[1] = time->getMonth();
    date[2] = time->getYear();
    Intervention* i = new Intervention(date, toll, type);
    interventions.insert(*i);
    return i;
    if(type == RevisionIntervention)
    {
        Revision* r = new Revision(date, toll);
        interventions.insert(*r);
        return r;
    }
    else if(type == ElectronicIntervention)
    {
        ElectronicRepair* er = new ElectronicRepair(date,toll);
        interventions.insert(*er);
        return er;
    }
    else if(type == InformaticIntervention){
        InformaticRepair* ir = new InformaticRepair(date, toll);
        interventions.insert(*ir);
        return ir;
    }
    else throw(InvalidInterventionType(type));
}

void SystemMonitor::completeIntervention(Intervention *intervention, double duration) {
    Intervention findInter = interventions.find(*intervention);
    if(findInter != *intervention)
        throw(InterventionNotFound(intervention));
    intervention->setDuration(duration); intervention->setDone(true);
    if(!interventions.remove(findInter))
        throw(InterventionNotFound(&findInter));
    interventions.insert(*intervention);
}


vector<Intervention> SystemMonitor::getInterventionsNewestFirst() {
    BSTItrIn<Intervention> it(interventions.getInterventions());
    vector<Intervention> ret;
    while(!it.isAtEnd()){
        ret.push_back(it.retrieve());
        it.advance();
    }
    return ret;
}

vector<Intervention> SystemMonitor::getInterventionsOldestFirst() {
    vector<Intervention> aux = getInterventionsNewestFirst(), ret;
    for(vector<Intervention>::reverse_iterator it = aux.rbegin(); it != aux.rend(); it++)
    {
        ret.push_back(*it);
    }
    return ret;
}

void SystemMonitor::addTech(Toll *pToll) {
    cout<<"SELECT THE TECHNICIAN'S SPECIALTY:\n"
        <<"1 - REVISION TECHNICIAN\n"
        <<"2 - ELECTRONIC REPAIR TECHNICIAN\n"
        <<"3 - INFORMATIC REPAIR TECHNICIAN\n"
        <<"0 - GO BACK\n";

    int choice;
    cin>>choice;
    while(1) {
        if (choice > 0 && choice < 4) {
            pToll->addTechnician(choice-1);
            return;
        }
        else if(choice==0){
            return;
        }
        else{
            cout<<"ENTER A VALID OPTION\n";
        }
    }
}
