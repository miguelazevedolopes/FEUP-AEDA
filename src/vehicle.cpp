#include "vehicle.h"

const string &Vehicle::getLicensePlate() const {
    return licensePlate;
}

Vehicle::Vehicle() { viaVerde = false; }

void Vehicle::setLicensePlate(const string &licensePlate1) {
    Vehicle::licensePlate = licensePlate1;
}

int Vehicle::getCategory() const {
    return category;
}

bool Vehicle::operator==(const Vehicle &rhs) const {
    return licensePlate == rhs.licensePlate;
}

bool Vehicle::operator!=(const Vehicle &rhs) const {
    return !(rhs == *this);
}

void Vehicle::startTrip(Toll *toll, Time *time) {
    if (!trips.empty() && !trips[trips.size() - 1]->isFinished()) {
        cout << "VEHICLE IS ALREADY IN A TRIP. "
             << "VEHICLE MUST PASS THROUGH AN EXIT TOLL "
             << "BEFORE ENTERING AGAIN\n";
        return;
    }
    trips.push_back(new Trip(toll->getName(), toll->getPrice(), time));
}

double Vehicle::endTrip(Toll *toll, Time *time) {

    if ((trips.empty() || trips[trips.size() - 1]->isFinished())) {
        cout << "VEHICLE NEVER ENTERED OR ALREADY EXITED.\n";
        return -1;
    }
    trips[trips.size() - 1]->setEnd(toll->getName(), toll->getPrice());
    trips[trips.size() - 1]->setEndTime(time);

    double price = toll->getPrice() * (1 + 0.1 * (this->getCategory() - 1));
    trips[trips.size() - 1]->setPrice(price);
    return price;
}

void Vehicle::printTrips() {
    if (!getTrips().empty()) {
        for (auto y:(getTrips())) {
            cout << "FROM: " << (y)->getBegin().first << endl;
            cout << "TO: " << (y)->getEnd().first << endl;
            cout << "WHEN: " << (y)->getEndTime()->getDate() << endl;
            cout << "PRICE PAID: " << (y)->getPrice() << endl << endl;
        }
    } else {
        cout << "NO TRIPS TO SHOW" << endl << endl;
    }
    int i;
    while (true) {
        cout << "PRESS 0 TO LEAVE\n";
        cin >> i;
        if (i == 0) return;
    }
}

Vehicle::Vehicle(const string &licensePlate) : licensePlate(licensePlate) {}

Vehicle::Vehicle(const string &licensePlate, int category, bool viaVerde) : licensePlate(licensePlate),
                                                                            category(category), viaVerde(viaVerde) {}

bool Vehicle::isViaVerde() const {
    return viaVerde;
}

void Vehicle::printInfo() {
    cout << "LICENSE PLATE: " << licensePlate << endl;
    cout << "CATEGORY: " << category << endl;
    cout << "VIA VERDE: ";
    if (viaVerde) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

}

void Vehicle::addPayment(double price) {
    trips[trips.size() - 1]->setPrice(price);
}

void Vehicle::changeViaVerde() {
    viaVerde = !viaVerde;
}

vector<Trip *> Vehicle::getTrips() {
    return trips;
}

Trip *Vehicle::getLastTrip() {
    if (trips.empty())
        return nullptr;
    return trips[trips.size() - 1];
}

ostream &operator<<(ostream &os, const Vehicle &vehicle) {
    os << vehicle.getLicensePlate() << " " << vehicle.getCategory() << " " << vehicle.isViaVerde() << " "
       << vehicle.trips.size();
    if (!vehicle.trips.empty()) {
        os << endl;
        for (int i = 0; i < vehicle.trips.size(); ++i) {
            os << *vehicle.trips[i];
            if (i != vehicle.trips.size() - 1)
                os << endl;
        }
    }
    delete &vehicle;
    return os;
}

istream &operator>>(istream &is, Vehicle &vehicle) {
    string licensePlate;
    bool viaVerde;
    int category, numTrips;

    if (is >> licensePlate >> category >> viaVerde >> numTrips) {
        vehicle.setLicensePlate(licensePlate);
        if (category <= 5 && category > 0)
            vehicle.setCategory(category);

        if (viaVerde)
            vehicle.changeViaVerde();

        vector<Trip *> trips = vector<Trip *>();
        if (numTrips > 0) {
            for (int i = 0; i < numTrips; ++i) {
                Trip *trip = new Trip();
                if (is >> *trip) {
                    trips.push_back(trip);
                }
            }
        }
        vehicle.setTrips(trips);

    }

    return is;
}

void Vehicle::setCategory(int category1) {
    Vehicle::category = category1;
}

void Vehicle::setTrips(const vector<Trip *> &trips1) {
    Vehicle::trips = trips1;
}
