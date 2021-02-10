//
// Created by Pedro Figueiredo on 26/12/2020.
//

#ifndef PORTAGENS_INTERVENTION_H
#define PORTAGENS_INTERVENTION_H

#include <ostream>
#include "toll.h"
#include "bst.h"
//#include "systemMonitor.h"

enum InterventionType{
    RevisionIntervention = 0,
    ElectronicIntervention = 1,
    InformaticIntervention = 2
};

class Intervention {
private:
    ///Initial Date in the format [day, month, year]
    int date[3]; // [dia, mes, ano]
    ///Duration in hours
    int duration; //
    ///Pointer to Toll
    Toll* toll;
    ///Uses enumerate InterventionType
    int type;
    ///True if the Intervention has been completed, false otherwise
    bool done;

    string tollName;
public:
    ///Returns the duration of the intervention
    int getDuration() const;
    ///Sets the duration of the intervention
    void setDuration(int duration);
    ///Sets the boolean done to chosen value
    void setDone(bool done);

    void setType(int type);

    void setDate(int date[3]);

    void setTollName(string name);

    string getTollName();

    void setToll(Toll *toll);

    Toll *getToll() const;

    ///Default Constructor
    Intervention(){};
    ///Constructor sets duration to 0 and done to false, when an Intervention is created
    Intervention(int *date, Toll *toll, int type);
    ///Two Interventions are considered equal if their date, toll and type are the same
    bool operator==(const Intervention &rhs) const;
    ///Two Interventions are different if they are not equal @see bool Intervention::operator!=(const Intervention &rhs) const
    bool operator!=(const Intervention &rhs) const;
    ///Less Than operator used in the BST
    ///
    ///Allows the Interventions to be order by date, then by toll and, if needed, by the type
    bool operator<(const Intervention &rhs) const;

    friend ostream &operator<<(ostream &os, const Intervention &intervention);

    friend istream &operator>>(istream &is, Intervention &intervention);

};



class Revision: public Intervention{
public:
    Revision(int *date, Toll *toll);
};

class Repair: public Intervention{
public:
    Repair(int *date, Toll *toll, int interventionType);
};

class ElectronicRepair: public Repair{
public:
    ElectronicRepair(int *date, Toll *toll);
};

class InformaticRepair: public Repair{
public:
    InformaticRepair(int *date, Toll *toll);
};


class InvalidInterventionDate{
public:
    InvalidInterventionDate(int date[3]);
};

class InvalidInterventionType{
public:
    InvalidInterventionType(int type);

};

class InterventionNotFound{
    Intervention* inter;
public:
    InterventionNotFound(Intervention* inter): inter(inter){};

};

class InterventionTree {
    BST<Intervention> interventions;
public:
    InterventionTree(): interventions(Intervention()){};
    bool isEmpty();
    void insert(Intervention inter);
    Intervention find(Intervention inter);
    bool remove(Intervention inter);
    const BST<Intervention> &getInterventions() const;
};

#endif //PORTAGENS_INTERVENTION_H
