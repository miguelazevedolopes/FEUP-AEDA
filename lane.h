#ifndef AEDA_T8G6_LANE_H
#define AEDA_T8G6_LANE_H

#include "employee.h"
#include "sequentialSearch.h"
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

///Contains info about a Lane, such as numCrossings and vehicleQueue
///
///@param vehicleQueue contains pairs of licensePlate and prices
///Example: XX-XX-XX, 1.60
///This indicates the Vehicle with the licensePlate XX-XX-XX will pay 1.60€ at this Lane

class Lane {
protected:
    ///Number of Crossing
    ///
    ///This is the number of Vehicles that passed this lane
    int numCrossings;
    ///Queue of Vehicles
    queue<pair<string, double>> vehicleQueue; //pair<licensePlate, price>

public:
    ///Creates a Lane with 0 crossings
    Lane() { this->numCrossings = 0; }

    ///Destructor
    virtual ~Lane() = default;

    ///Creates a Lane with chosen numCrossings
    explicit Lane(int numCrossings);

    ///Creates a Lane with numCrossings crossings and a queue with <licensePlate, price> pairs
    ///
    /// \param numCrossings
    /// \param vehicleQueue
    Lane(int numCrossings, const queue<pair<string, double>> vehicleQueue);

    ///Returns Number of Crossings
    ///
    /// \return Number of Crossings
    int getNumCrossings() const;

    ///Pops the front vehicle from queue
    ///
    /// \return true if successful, false otherwise
    ///@note This function will return false when vehicleQueue is empty
    string passVehicle();

    ///Adds Crossing
    ///increments numCrossings\n
    ///
    /// \return poped price pair
    void addCrossing();

    ///Adds the pair licensePlate, price to the queue
    ///
    /// \param licensePlate
    /// \param price
    ///@see ViaVerdeLane::addVehicle(string licensePlate, double price)
    virtual void addVehicle(string licensePlate, double price);


    ///Returns true if it is a ViaVerde Lane
    virtual bool isViaVerde() const { return false; }

    ///Returns true if it is a Normal Exit Lane
    [[nodiscard]] virtual bool isNormalExitLane() const = 0;




    ///Returns pointer to Employee
    ///
    /// \return pointer to Employee
    virtual Employee *getEmployee() { return nullptr; }

    ///Returns number of Vehicles in queue
    ///
    /// \return number of Vehicles in queue
    int numberVehiclesWaiting() { return vehicleQueue.size(); }

    ///Set Employee
    ///
    /// \param employee
    virtual void setEmployee(Employee *employee) {};

    ///Returns vector with Last Employees
    virtual vector<Employee *> getLastEmployees() { return vector<Employee *>(); };

    ///Compares number of crossings between 2 Lanes
    bool operator<(const Lane &rhs) const;

    ///Adds Employee to vector lastEmployees
    virtual void addToEmployeeList() {};

    ///Sets last Employees
    virtual void setLastEmployees(vector<Employee *> lastE) {};

    ///Compares number of crossings between 2 Lanes
    bool operator>(const Lane &rhs) const;

    ///Compares number of crossings between 2 Lanes
    bool operator<=(const Lane &rhs) const;

    ///Compares number of crossings between 2 Lanes
    bool operator>=(const Lane &rhs) const;

    ///Returns ostream with the pairs <licensePlate, price> in queue
    ///
    /// \param os ostream
    /// \param lane
    /// \return ostream with the pairs <licensePlate, price> in queue
    friend ostream &operator<<(ostream &os, const Lane &lane);

    ///Returns istream with the pairs <licensePlate, price> in queue
    ///
    /// \param is istream
    /// \param lane
    /// \return
    friend istream &operator>>(istream &is, Lane &lane);

    ///Allows lane to be saved to file
    virtual ostream &saveToFile(ostream &os) const;

    ///Allows lane to be loaded from file
    virtual istream &loadFromFile(istream &is);

    ///Prints Lane with index
    void printLaneNumbered(int i) const;
};

///Child of Lane
class NormalLane : public Lane {
public:
    ///Creates a NormalLane
    NormalLane() {}

    ///Checks if Lane is a NormalExitLane
    virtual bool isNormalExitLane() {return false;}

    ///Creates NormalLane with chosen number of crossings
    ///
    /// \param numCrossings
    ///@see Lane(int numCrossings)
    NormalLane(int numCrossings) : Lane(numCrossings) {}

    ///Creates NormalLane with chosen number of crossings and a vehicle queue
    ///
    /// \param numCrossings
    /// \param vehicleQueue
    ///@see Lane(int numCrossings, const queue<pair<string, double>> vehicleQueue)
    NormalLane(int numCrossings, const queue<pair<string, double>> vehicleQueue) : Lane(numCrossings, vehicleQueue) {}

    ///Checks if it is a Normal Exit Lane
    bool isNormalExitLane() const override { return false; }

};

///Child of NormalLane
class NormalExitLane : public Lane {
private:
    ///pointer to Employee that is working in
    Employee *employee;
    ///vector of pointers to Employee
    vector<Employee *> lastEmployees;

public:
    /// Creates Normal Exit Lane
    ///
    /// \param numCrossings
    /// \param vehicleQueue
    /// \param employee
    /// \param lastEmployees
    ///@see Lane(int numCrossings, const queue<pair<string, double>> vehicleQueue)
    NormalExitLane(int numCrossings, const queue<pair<string, double>> vehicleQueue, Employee *employee,
                   const vector<Employee *> &lastEmployees);

    NormalExitLane();

    ///Checks if it is a Normal Exit Lane
    bool isNormalExitLane() const override { return true; }


    ///Return pointer to Employee working in NormalExitLane
    ///
    /// \return pointer to Employee working in NormalExitLane
    Employee *getEmployee();

    ///Changes Employee working in NormalExitLane
    ///
    /// \param employee pointer to Employee
    void setEmployee(Employee *employee);

    ///Returns vector of pointers to Employee
    vector<Employee *> getLastEmployees() { return lastEmployees; }

    void setLastEmployees(vector<Employee *> lastE);

    ///Adds Employee to vector lastEmployees
    void addToEmployeeList() override;;

    ostream &saveToFile(ostream &os) const override;

    istream &loadFromFile(istream &is) override;

    istream &loadVehicleQueueFromFile(istream &is);

    istream &loadCurrentEmployeeFromFile(istream &is);

    istream &loadLastEmployeesFromFile(istream &is);

    ostream &saveVehicleQueueToFile(ostream &os) const;

    ostream &saveCurrentEmployeeToFile(ostream &os) const;

    ostream &saveLastEmployeesToFile(ostream &os) const;


};

///Child of Lane
class ViaVerdeLane : public Lane {

public:
    ///Creates Via Verde Lane
    ViaVerdeLane();

    ///Creates Via Verde Lane with chosen number of crossings
    ///
    /// \param numCrossings
    ViaVerdeLane(int numCrossings);

    /// Passes Vehicle through ViaVerdeLane
    ///
    /// \param licensePlate
    /// \param price
    ///@note Adds the pair <licensePlate, price> to queue vehicleQueue and pops it, incrementing numCrossings
    ///@see Lane::addVehicle()
    ///@see addCrossing()
    void addVehicle(string licensePlate, double price);

    ///Checks if it is a Via Verde Lane
    bool isViaVerde() const { return true; }

    ///Checks if it is a Normal Exit Lane
    bool isNormalExitLane() const override { return false; }
};

#endif //AEDA_T8G6_LANE_H
