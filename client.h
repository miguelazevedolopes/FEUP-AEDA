#ifndef AEDA_T8G6_CLIENT_H
#define AEDA_T8G6_CLIENT_H


#include <ostream>
#include "vehicle.h"

///Contains info about a client, such as name, nif and his/her vehicles

class Client {
private:
    ///Client's name
    string name;
    ///Client's nif
    int nif;
    ///vector of pointers to Vehicle
    vector<Vehicle *> vehicles;

public:
    ///Creates Client with undefined private attributes
    Client();

    ///Creates Client with name and nif
    ///
    /// \param name
    /// \param nif
    Client(string name, int nif);

    ///Creates a Client with only nif and no assigned name
    ///
    /// \param nif
    Client(int nif);

    ///Returns Client's name
    ///
    /// \return Client's name
    const string &getName() const;

    ///Returns pointer vector with Client's Vehicles
    ///
    /// \return vector with Client's Vehicles
    vector<Vehicle *> &getVehicles() ;

    ///Returns pointer to Client's Vehicle with this licensePlate
    ///
    /// \param licensePlate is a string in the format XX-XX-XX
    /// \return pointer to Client's Vehicle with this licensePlate
    Vehicle *getVehicle(string &licensePlate);

    ///Removes Vehicle from Client's vehicles
    ///
    /// \param vehicle is a pointer to the Client's Vehicle which will be removed
    void removeVehicle(Vehicle *vehicle);

    ///Returns true, if the nif's are the same, false otherwise
    ///
    /// \param rhs is another Client
    /// \return true, if the nif's are the same, false otherwise
    bool operator==(const Client &rhs) const;

    ///Returns true, if the nif's are different, false otherwise
    ///
    /// \param rhs is another Client
    /// \return true, if the nif's are different, false otherwise
    bool operator!=(const Client &rhs) const;//REMOVE?

    /// Adds Vehicle to Client's vehicles
    /// \param vehicle is a pointer to the new Vehicle
    void addVehicle(Vehicle *vehicle);

    ///Prints the info about each of the Client's Vehicles
    ///
    ///This function calls Vehicle::printInfo() for each of the Client's Vehicles
    ///@see Vehicle::printInfo()
    void printVehicles();

    /// Changes Client's name
    /// \param name string with the Client's new name
    void changeName(string name);

    /// Changes Client's nif
    /// \param nif int with new nif
    ///@note Used to correct attribution mistake
    void changeNIF(int nif);

    ///Prints Client's Info
    ///
    ///Prints Client's name, nif and number of Vehicles
    void printInfo();

    ///Returns ostream with name, nif, and Vehicle(s) license plate(s)
    ///
    /// \param os
    /// \param client
    /// \return
    friend ostream &operator<<(ostream &os, const Client &client);

    ///Returns istream with name, nif, and Vehicle(s) license plate(s)
    ///
    /// \param is
    /// \param client
    /// \return
    friend istream &operator>>(istream &is, Client &client);

    ///Sets Client's name
    ///
    /// \param name
    void setName(const string &name);

    ///Sets Client's NIF
    ///
    /// \param nif
    void setNif(int nif);

    ///Sets Client's Vehicles
    ///
    /// \param vehicles
    void setVehicles(const vector<Vehicle *> &vehicles);

    ///Returns Client's nif
    ///
    /// \return Client's nif
    int getNif() const;
};


#endif //AEDA_T8G6_CLIENT_H
