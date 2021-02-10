#ifndef PORTAGENS_TECHNICIAN_H
#define PORTAGENS_TECHNICIAN_H
using namespace std;
#include "iostream"

enum TechnicianSpecialty{
    RevisionTechnician = 0,
    EletronicTechnician = 1,
    InformaticTechnician = 2
};

class Technician {
    ///Uses emumerate TechnicianSpecialty
    int specialty;
    ///Performance is the average repair time in hours
    double performance=0;
    ///Is the number of interventions the technician performed.
    ///Used to calculate the performance
    int interventionCount=0;
public:
    Technician(){}

    Technician(int type):specialty(type){}

    void setSpecialty(int spec);

    void setInterventionCount(int iC);

    void setPerformance(double p);

    int getSpecialty() const {return specialty;}

    double getPerformance(){return performance;}

    void addIntervention(int time);
    ///Overloads the operator. Used to organize the priority queue in the Toll object.
    bool operator<(const Technician o1) const;
    ///Used for saving the data to a file
    friend ostream &operator<<(ostream &os, const Technician &technician);
    ///Used for loading the data from a file
    friend istream &operator>>(istream &is,Technician &technician);
};

class RevisionTechnician: public Technician{
public:
    RevisionTechnician();
};

class EletronicTechnician: public Technician{
public:
    EletronicTechnician();
};

class InformaticTechnician: public Technician{
public:
    InformaticTechnician();
};

class InvalidTechnicianType{
public:
    InvalidTechnicianType(int type);
};

#endif //PORTAGENS_TECHNICIAN_H