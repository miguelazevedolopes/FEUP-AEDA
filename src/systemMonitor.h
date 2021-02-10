#ifndef AEDA_T8G6_SYSTEMMONITOR_H
#define AEDA_T8G6_SYSTEMMONITOR_H

#include "highway.h"
#include "client.h"
#include "toll.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "bst.h"
#include "intervention.h"
#include <unordered_set>

// ----------------------------------------------------------

struct ClientHash {
    //TODO
    int operator()(const Client *client) const {
        return client->getNif();
    }

    //TODO
    bool operator()(const Client *client1, const Client *client2) const {
        return client1->getNif() == client2->getNif();
    }
};

typedef unordered_set<Client *, ClientHash, ClientHash> HashTableClient;

using namespace std;

///This is the main Class of the system
///
/// @see Highway
/// @see Employee
/// @see Client
/// @see Vehicle
class SystemMonitor {
private:
    vector<Highway *> highways;
    vector<Employee *> employees;
    vector<Client *> clients;
    HashTableClient activeClients;
    vector<Vehicle *> vehicles;
    InterventionTree interventions;
public:
    ///Returns pointer to Vehicle with this license plate
    ///
    /// \param licensePlate
    /// \return pointer to Vehicle with this license plate
    /// @note this function calls sequentialSearch(const vector<Comparable *> &v, const Comparable *x)
    Vehicle *findVehicleClients(const string &licensePlate);

    ///Read from file and loads information onto the various classes
    ///
    ///Calls loadVectorFromFile(string &vectorFileName, vector<T *> &vec)
    void load();

    ///Saves information of the various classes onto the text files
    ///
    ///Calls saveVectorToFile(const string &vectorFileName, vector<T *> &vec)
    void save();

    ///Initiates SystemMonitor
    ///
    ///Calls load()
    SystemMonitor() {
        load();
    };

    ///Closes SystemMonitor
    ///
    ///Calls save()
    ~SystemMonitor() {
        save();
    };

    ///Returns vector of pointers to Highway
    ///
    /// \return vector of pointers to Highway
    vector<Highway *> getHighways();

    Employee *getEmployee(int ssNumber);

    ///If found, returns postion of Employee in vector, else it returns -1
    ///
    /// \param cemployee
    /// \return position of Employee, if  found, else, -1
    int findEmployee(const Employee *cemployee);

    ///Searches for Client in clients vector
    ///
    /// \param client
    /// \return true if found, false, otherwise
    ///
    int findClient(const Client *client);

    ///Prints Highways numbered
    ///
    /// 1: highway1\n
    /// 2: highway2
    void printHighwaysNumbered();

    ///Returns pointer to Highway at index i in highways vector
    ///
    /// \param i index
    /// \return pointer to Highway at index in in highways vector
    Highway *getHighwayAt(int i);

    ///Returns pointer to Vehicle with this license plate
    ///
    /// \param licensePlate
    /// \return Returns pointer to Vehicle with this license plate
    Vehicle *getVehicle(const string &licensePlate);

    ///Removes pointer to Vehicle from vehicles
    static void removeVehicle(Client *client);

    ///Displays a certain Client's Vehicles
    ///
    /// \param client pointer to Client
    void viewVehicles(Client *client);

    ///Displays Client's trips and their prices
    ///
    /// \param client pointer to Client

    void showCostsClient(Client *client);

    ///Allows Client to change info about their Vehicles
    ///
    /// \param client pointer to Client
    void updateVehicles(Client *client);

    ///Adds pointer to Highway to highways vector
    ///
    /// \param highway pointer to Highway
    void addHighway(Highway *highway);

    ///Allows for license plate input
    ///
    ///
    ///@warning Only valid format is "XX-XX-XX"!
    static string licensePlateInput();

    ///Client Login
    ///
    /// \return pointer to Client
    ///@note If the Client is new, a new Client object is created and added to clients vector
    Client *login();

    ///Allows Client to add Vehicle
    ///
    /// \param client pointer to Client
    void addVehicleClient(Client *client);

    ///New License Plate Input Menu
    string getNewLicensePlate();

    ///Category Input Menu
    ///
    /// \return category
    static int categoryInput();

    ///ViaVerde Input Menu
    static bool viaVerdeInput();

    ///Number Input
    static int getNumberInput();

    ///Allows for new Client creation
    ///
    /// \return pointer to Client
    Client *createNewClient();

    ///Counts digits in a number
    ///
    /// \param num
    /// \return number of digits
    static int countDigit(int num);

    ///Requests confirmation from user
    ///
    /// \return true if confirmed, false otherwise
    static bool confirmation();

    ///Name Input
    ///
    /// \return name
    static string getNewName();

    ///NIF Input
    ///
    /// \return NIF
    int getNewNif();

    /// Finds Vehicle with this license plate
    ///
    /// \param licensePlate
    /// \return position of pointer to vehicle in vehicles vector
    int findVehicle(const string &licensePlate);

    ///Allows for Employee login
    ///
    /// \return pointer to Employee
    Employee *loginEmployee();

    ///Allows Client to change name
    static void changeName(Client *client);

    ///Allows Client to change NIF
    void changeNIF(Client *client);

    ///Finds Lane where the Employee is working
    ///
    /// \param pEmployee
    /// \return pointer to Lane where Employee is working
    Lane *findEmployeeLane(Employee *pEmployee);

    ///Finds Toll where Lane is located
    ///
    /// \param lane
    /// \return pointer to Toll where Lane is located
    Toll *findLaneToll(Lane *lane);

    ///Finds Highway where Toll is located
    ///
    /// \param toll
    /// \return pointer to Highway where Toll is located
    Highway *findTollHighway(Toll *toll);

    ///Allows Manager to add Highway
    void managerAddHighway();

    ///Allows Manager to remove Highway
    void managerRemoveHighway();

    ///Displays Highways for Manager
    ///
    ///@see printHighwaysNumbered()
    void managerViewHighways();

    ///Highway selection
    ///
    /// \return index of selected highway
    ///@see printHighwaysNumbered()
    int selectHighway();

    ///Allows Manager to Add Toll to Highway
    void managerAddToll(Highway *phighway, bool exit);

    ///Shows Highway Tolls
    static void viewHighwayTolls(Highway *phighway);

    ///Allows Manager to remove Toll from Highway
    static void managerRemoveToll(Highway *phighway);

    ///Toll selection from Highway
    ///
    /// \param pHighway pointer to Highway
    /// \return pointer to chosen Highway
    static Toll *selectToll(Highway *pHighway);

    ///Allows Manager to add a Lane to Toll
    ///
    /// \param pToll
    /// \param viaVerde specifies type of Toll to be added
    ///@note if viaVerde is true, then the adde Lane will be a ViaVerdeLane
    void managerAddLane(Toll *pToll, bool viaVerde);

    ///Prints Employees Numbered
    ///
    ///1: employee1
    ///2: employee2
    void printEmployeesNumbered();

    ///Allows for Employee selection
    ///
    /// \return pointer to selected Employee
    Employee *selectEmployee();

    ///Shows Lanes within a Toll
    ///
    /// \param pToll pointer to Toll
    void viewLanes(Toll *pToll);

    ///Allows Manager to remove lane from Toll
    ///
    /// \param pToll pointer to Toll
    static void removeLane(Toll *pToll);

    ///Allows Manager to Change Lane where an Employee is working
    ///
    /// \param pToll pointer to Toll
    void changeLaneEmployee(Toll *pToll);

    ///Allows Manager to add an Employee
    void managerAddEmployee();

    ///Allows Manager to remove an Employee
    void managerRemoveEmployee();

    /// Change Employee's Lane
    void changeEmployeeLane();

    /// Displays Employees Numbered
    ///
    /// Calls printEmployeesNumbered()
    void viewEmployees();

    ///Template function to save to files
    template<class T>
    void saveVectorToFile(const string &vectorFileName, vector<T *> &vec) const;

    ///Shows Employee history for this Lane
    ///
    /// \param pToll pointer Toll
    static void viewLastEmployees(Toll *pToll);

    ///Template function to load from files
    template<class T>
    void loadVectorFromFile(string &vectorFileName, vector<T *> &vec);

    void loadInterventionsBST(string &fileName);
    void saveInterventionsBST(string &fileName);

    Toll* findTollInSystem(Toll* toll);

    ///Finishes Loading Clients
    void finishLoadingClients();

    ///Finishes Loading Lanes
    void finishLoadingLanes();

    bool showClientVehicleTrips(Client *client, int c) const;

    void updateVehicleViaVerde(Vehicle *vehicle) const;

    void showCostsTrip(const Trip *pTrip) const;

    void showCostsVehicle(Vehicle *pVehicle) const;

    static string getTollNameInput();

    static string getTollLocationInput();

    static int getTollPositionInput();

    static double getTollPriceInput();

    void addNormalExitLane(Toll *pToll);

    void createNormalExitLaneWithoutEmployee(Toll *pToll) const;

    void createNormalExitLaneWithEmployee(Toll *pToll, Employee *employee) const;

    void removeEmployeeLane(Employee *employee);

    void addVehicle(Vehicle *vehicle);

    vector<Client *> getActiveClientsVector() const;

    void addActiveClientsVector(vector<Client *> activeClientsVec);

    const HashTableClient &getActiveClients() const;

    void setActiveClients(const HashTableClient &activeClients);

    void viewAllActiveClients();

    void printAllActiveClientsNumbered();

    void searchActiveClientsByName();

    void printActiveClientsNumberedByNIF(int nif);

    void printActiveClientsNumberedByNumVehciles(int numVehicles);

    void printActiveClientsNumberedByName(const string &name);

    void searchActiveClientsByNIF();

    void searchActiveClientsByNumVehicles();

    Intervention* scheduleIntervention(Toll *toll, int type);

    void completeIntervention(Intervention *intervention, double duration);

    vector<Intervention> getInterventionsNewestFirst();

    vector<Intervention> getInterventionsOldestFirst();

    //friend Intervention;
    void addTech(Toll *pToll);
};

///Thrown when an input is cancelled
class ConfirmationExitException : exception {
public:
    static void showMessage() {
        cout << "OPERATION CANCELED\n";
    }
};

#endif //AEDA_T8G6_SYSTEMMONITOR_H
