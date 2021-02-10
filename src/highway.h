//
// Created by Pedro Figueiredo on 11/11/2020.
//

#ifndef PORTAGENS_HIGHWAY_H
#define PORTAGENS_HIGHWAY_H

#include <ostream>
#include "toll.h"
#include "intervention.h"


///Contains info about a highway
///
///@param name highway name
///@param tolls vector with pointers to Toll of this highway
class Highway {
private:
    ///Highway name
    string name;
    ///vector of pointers to Toll
    vector<Toll *> tolls;
public:
    Highway();

    ///Creates Highway with name n
    Highway(string n) : name(n) {}

    ///Returns Highway name
    ///
    /// \return Highway name
    const string &getName() const;

    void setName(const string &name);

    ///Prints Tolls with index;
    ///
    ///Prints Tolls in the following format:\n
    ///1: tollName1\n
    ///2: tollName2
    void printTollsNumbered();

    ///Either prints isExit tolls or entry Tolls
    ///
    /// \param isExit indicates which type of Tolls to print
    ///If isExit is true, then the Tolls printed with be isExit Tolls
    ///Else they will be entry Tolls
    ///@note The Tolls are printed in the same format as printTollsNumbered()
    void printTollsNumbered(bool isExit);

    ///Returns Toll at the index i of tolls
    ///
    /// \param i index
    /// \param exit boolean (true if
    /// \return
    Toll *getTollAt(int i, bool exit);

    ///Adds Toll to the Highway
    ///
    /// \param toll pointer to toll that will be added
    void addToll(Toll *toll);

    ///Returns Highway's tolls
    ///
    /// \return Highways tolls
    vector<Toll *> &getTolls();

    ///Returns number of Tolls specified by the bool
    ///
    /// \param exit
    /// \return
    int getTollsSize(bool exit);

    ///Erases Toll at index i
    ///
    /// \param i index
    void eraseTollAt(int i) { tolls.erase(tolls.begin() + i); }

    ///Returns ostream with Highway's name, number of Tolls and info of each Toll
    ///
    /// \param os ostream
    /// \param highway
    /// \return ostream with Highway's name, number of Tolls and info of each Toll
    /// @see Toll::operator<<
    friend ostream &operator<<(ostream &os, const Highway &highway);

    ///Returns istream with Highway's name, number of Tolls and info of each Toll
    ///
    /// \param is istream
    /// \param highway
    /// \return istream with Highway's name, number of Tolls and info of each Toll
    friend istream &operator>>(istream &is, Highway &highway);

    int repair(int i, Toll *pToll, Intervention *pIntervention);

    bool techExistsInHighway(int spec);

    Technician getClosestTechnician(int i, Toll *pToll);
};


#endif //PORTAGENS_HIGHWAY_H
