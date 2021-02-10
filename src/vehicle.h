#ifndef AEDA_T8G6_VEHICLE_H
#define AEDA_T8G6_VEHICLE_H

#include <string>
#include <vector>
#include <ostream>
#include "trip.h"
#include "toll.h"

using namespace std;


///Contains info about a Vehicle such as license plate, category, ViaVerde status and trips
///
///@param trips is a vector of pointers to Trip

class Vehicle {
private:
    ///Vehicle's license plate
    string licensePlate;
    ///Vehicle's category
    int category;
    ///Vehicle's ViaVerde status
    bool viaVerde; //true se tiver via verde
    ///Vehicle Trips
    vector<Trip *> trips;

public:
    ///Creates Vehicle with undefined attributes
    Vehicle();

    /// Creates Vehicle with license plate, category and ViaVerde status
    ///
    /// \param licensePlate
    /// \param category
    /// \param viaVerde
    Vehicle(const string &licensePlate, int category, bool viaVerde);

    /// Creates a Vehicle with license plate, category and sets ViaVerde status to false
    ///
    /// \param plate
    /// \param cat
    Vehicle(string plate, int cat) : licensePlate(plate), category(cat) {
        viaVerde = 0;
    }

    ///Initializes license plate
    ///
    /// \param licensePlate
    Vehicle(const string &licensePlate);

    ///Destructor
    ~Vehicle() = default;

    ///Returns price
    ///
    /// \param price
    void addPayment(double price);

    ///Returns vector of pointers to Trip
    ///
    /// \return vector of pointers to Trip
    vector<Trip *> getTrips();

    ///Returns license plate
    ///
    /// \return license plate
    const string &getLicensePlate() const;

    ///Checks if a Vehicle has ViaVerde
    ///
    /// \return true if the Vehicle has ViaVerde, false otherwise
    bool isViaVerde () const;

    ///Changes ViaVerde status
    void changeViaVerde();

    ///Sets Vehicle's license plate
    ///
    /// \param licensePlate
    void setLicensePlate(const string &licensePlate);

    ///Sets Vehicle's category
    ///
    /// \param category
    void setCategory(int category);

    ///Checks if 2 vehicles are equal
    ///
    /// \param rhs
    /// \return true if the license plate is the same
    bool operator==(const Vehicle &rhs) const;

    ///Checks if 2 vehicles are different
    ///
    /// \param rhs
    /// \return true if license plates are different
    bool operator!=(const Vehicle &rhs) const;

    ///Returns Vehicle's category
    ///
    /// \return
    int getCategory() const;

    ///Starts trip
    ///
    /// \param toll points to begin Toll
    /// \param time points to begin Time
    void startTrip(Toll *toll, Time *time);

    ///Ends Trip
    ///
    /// \param toll points to end Toll
    /// \param time points to end Time
    /// \return price
    double endTrip(Toll *toll, Time *time);

    ///Returns pointer to last Trip
    ///
    /// \return pointer to last Trip
    ///@note the last trip will be in the last index of the vector trip
    Trip* getLastTrip();

    ///Prints all of the Vehicle's Trips
    void printTrips();

    ///Prints Vehicle's Info
    ///
    ///Prints license plate, category and ViaVerde status
    void printInfo();

    ///Returns ostream with Vehicle's info
    ///
    /// \param os ostream
    /// \param vehicle
    /// \return ostream with Vehicle's info
    friend ostream &operator<<(ostream &os, const Vehicle &vehicle);

    ///Returns istream with Vehicle's info
    ///
    /// \param is
    /// \param vehicle
    /// \return Returns istream with Vehicle's info
    friend istream &operator>>(istream &is, Vehicle &vehicle);

    ///Sets Vehicle's Trips
    ///
    /// \param trips
    void setTrips(const vector<Trip *> &trips);
};

///Thrown when Vehicle creation is cancelled
class CreatingVehicleException : exception {
public:
    static void showMessage() {
        cout << "CREATING VEHICLE CANCELED\n";
    }
};

#endif //AEDA_T8G6_VEHICLE_H
