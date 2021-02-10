#include "lane.h"

istream &loadFromFile(istream &is, Lane &lane);

int Lane::getNumCrossings() const {
    return numCrossings;
}

void Lane::addCrossing() {
    this->numCrossings++;
}

void Lane::addVehicle(string licensePlate, double price) {
    vehicleQueue.push(pair<string, double>(licensePlate, price));
}

Lane::Lane(int numCrossings, const queue<pair<string, double>> vehicleQueue) : numCrossings(numCrossings),
                                                                               vehicleQueue(vehicleQueue) {}

bool Lane::operator<(const Lane &rhs) const {
    return numCrossings < rhs.numCrossings;
}

bool Lane::operator>(const Lane &rhs) const {
    return rhs < *this;
}

bool Lane::operator<=(const Lane &rhs) const {
    return !(rhs < *this);
}

bool Lane::operator>=(const Lane &rhs) const {
    return !(*this < rhs);
}

string Lane::passVehicle() {
    string lp;
    if (!vehicleQueue.empty()) {
        lp = vehicleQueue.front().first;
        vehicleQueue.pop();
        return lp;
    }
    return "0";
}

ostream &operator<<(ostream &os, const Lane &lane) {
    lane.saveToFile(os);
    delete &lane;
    return os;
}

istream &operator>>(istream &is, Lane &lane) {
    return lane.loadFromFile(is);
}

istream &Lane::loadFromFile(istream &is) {
    int numCross;
    if (is >> numCross)
        numCrossings = numCross;

    return is;
}

ostream &Lane::saveToFile(ostream &os) const {
    os << isViaVerde() << " " << numCrossings;
    return os;
}

Lane::Lane(int numCrossings) : numCrossings(numCrossings) {}

void Lane::printLaneNumbered(int i) const {
    cout << "LANE " << i << ":\n";
    cout << "VIA VERDE: ";
    if (isViaVerde()) {
        cout << "YES\n\n";
    } else cout << "NO\n\n";
}

void NormalExitLane::setEmployee(Employee *employee1) {
    this->employee = employee1;
}

Employee *NormalExitLane::getEmployee() {
    return employee;
}

NormalExitLane::NormalExitLane(int numCrossings, const queue<pair<string, double>> vehicleQueue, Employee *employee,
                               const vector<Employee *> &lastEmployees) : Lane(numCrossings, vehicleQueue),
                                                                          employee(employee),
                                                                          lastEmployees(lastEmployees) {}

ostream &NormalExitLane::saveToFile(ostream &os) const {
    Lane::saveToFile(os);

    saveVehicleQueueToFile(os);
    saveCurrentEmployeeToFile(os);
    saveLastEmployeesToFile(os);

    return os;
}

ostream &NormalExitLane::saveLastEmployeesToFile(ostream &os) const {
    os << lastEmployees.size() << " ";
    if (!lastEmployees.empty()) {
        for (auto lastEmployee : lastEmployees) {
            os << lastEmployee->getSsNumber() << " ";
        }
    }
    return os;
}

ostream &NormalExitLane::saveCurrentEmployeeToFile(ostream &os) const {
    bool hasCurrentEmployee = (employee != nullptr);
    os << " " << hasCurrentEmployee << " ";
    if (hasCurrentEmployee)
        os << employee->getSsNumber() << " ";
    return os;
}

ostream &NormalExitLane::saveVehicleQueueToFile(ostream &os) const {
    os << " " << vehicleQueue.size();
    if (!vehicleQueue.empty()) {
        queue<pair<string, double>> copy(vehicleQueue);
        while (!copy.empty()) {
            os << " " << copy.front().first << " " << copy.front().second;
            copy.pop();
        }
    }
    return os;
}

istream &NormalExitLane::loadFromFile(istream &is) {
    Lane::loadFromFile(is);

    loadVehicleQueueFromFile(is);
    loadCurrentEmployeeFromFile(is);
    loadLastEmployeesFromFile(is);

    return is;
}

istream &NormalExitLane::loadLastEmployeesFromFile(istream &is) {
    int ssNumber2, employeesSize;
    is >> employeesSize;
    while (employeesSize > 0 && is >> ssNumber2) {
        lastEmployees.push_back(new Employee(ssNumber2));
        employeesSize--;
    }
    return is;
}

istream &NormalExitLane::loadCurrentEmployeeFromFile(istream &is) {
    bool hasCurrentEmployee;
    int ssNumber;
    if (is >> hasCurrentEmployee) {
        if (hasCurrentEmployee) {
            is >> ssNumber;
            employee = new Employee(ssNumber);
        } else {
            employee = nullptr;
        }
    }
    return is;
}

istream &NormalExitLane::loadVehicleQueueFromFile(istream &is) {
    int vehicleQueueSize;
    pair<string, double> pair;
    if (is >> vehicleQueueSize) {
        if (vehicleQueueSize > 0) {
            for (int i = 0; i < vehicleQueueSize; i++) {
                is >> pair.first >> pair.second;
                vehicleQueue.push(pair);
            }
        }
    }
    return is;
}

NormalExitLane::NormalExitLane() = default;

void NormalExitLane::setLastEmployees(vector<Employee *> lastE) {
    NormalExitLane::lastEmployees = lastE;
}

void NormalExitLane::addToEmployeeList() { lastEmployees.push_back(employee); }

void ViaVerdeLane::addVehicle(string licensePlate, double price) {
    Lane::addVehicle(licensePlate, price);
    addCrossing();
}

ViaVerdeLane::ViaVerdeLane() {
    numCrossings = 0;
}

ViaVerdeLane::ViaVerdeLane(int numCrossings) : Lane(numCrossings) {}
