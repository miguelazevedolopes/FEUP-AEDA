#include "client.h"

#include <utility>

const string &Client::getName() const {
    return name;
}

bool Client::operator==(const Client &rhs) const {
    return nif == rhs.nif;
}

bool Client::operator!=(const Client &rhs) const {
    return !(rhs == *this);
}

Client::Client(string name, int nif) : name(std::move(name)), nif(nif) {}

Client::Client(int nif) : nif(nif) {}

vector<Vehicle *> &Client::getVehicles() {
    return vehicles;
}

void Client::addVehicle(Vehicle *vehicle) {
    vehicles.push_back(vehicle);
}

Vehicle *Client::getVehicle(string &licensePlate) {
    int pos = sequentialSearch(vehicles, new Vehicle(licensePlate));
    if (pos == -1)
        return nullptr;

    return vehicles[pos];
}

void Client::removeVehicle(Vehicle *vehicle) {
    int pos = sequentialSearch(vehicles, vehicle);
    vehicles.erase(vehicles.begin() + pos);
}

void Client::printVehicles() {
    if (!vehicles.empty()) {
        cout << "YOUR VEHICLES: ";
        for (int i = 0; i < vehicles.size(); i++) {
            cout << endl << i + 1 << ": ";
            vehicles[i]->printInfo();
        }
        cout << endl << endl;
    } else {
        cout << "NO VEHICLES TO SHOW" << endl;
    }
}

void Client::changeName(string newName) {
    name = newName;
}

void Client::changeNIF(int newNif) {
    nif = newNif;
}

void Client::printInfo() {
    cout << "NAME: " << name << endl;
    cout << "NIF: " << nif << endl;
    cout << "NUMBER OF VEHICLES: " << vehicles.size() << endl << endl;

}

ostream &operator<<(ostream &os, const Client &client) {
    os << client.name << " " << client.nif << " " << client.vehicles.size();
    if (!client.vehicles.empty()) {
        os << endl;
        for (auto vehicle : client.vehicles)
            os << vehicle->getLicensePlate() << " ";
    }
    delete &client;
    return os;
}

istream &operator>>(istream &is, Client &client) {
    string name, licensePlate;
    int nif, numVehicles;
    vector<Vehicle *> vehicles;

    if (is >> name >> nif >> numVehicles) {
        client.setName(name);
        client.setNif(nif);
        for (int i = 0; i < numVehicles; i++) {
            if (is >> licensePlate)
                vehicles.push_back(new Vehicle(licensePlate));
        }
        client.setVehicles(vehicles);
    }
    return is;
}

void Client::setName(const string &name) {
    Client::name = name;
}

void Client::setNif(int nif) {
    Client::nif = nif;
}

void Client::setVehicles(const vector<Vehicle *> &vehicles) {
    Client::vehicles = vehicles;
}

Client::Client() {}

int Client::getNif() const {
    return nif;
}

