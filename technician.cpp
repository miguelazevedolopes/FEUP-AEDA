//
// Created by Pedro Figueiredo on 26/12/2020.
//

#include "technician.h"

RevisionTechnician::RevisionTechnician() {}

EletronicTechnician::EletronicTechnician() {}

InformaticTechnician::InformaticTechnician() {}

InvalidTechnicianType::InvalidTechnicianType(int type) {}

void Technician::addIntervention(int time) {
    performance = ((performance * interventionCount) + time) / (interventionCount + 1);
    interventionCount++;
}

bool Technician::operator<(const Technician o1) const {
    return performance<o1.performance;
}

ostream &operator<<(ostream &os, const Technician &technician) {
    os<<technician.specialty<<" "<<technician.interventionCount<<" "<<technician.performance;
    return os;
}
void Technician::setSpecialty(int spec) {specialty=spec;}

void Technician::setInterventionCount(int iC) {interventionCount=iC;}

void Technician::setPerformance(double p) {performance=p;}

istream &operator>>(istream &is,Technician &technician) {
    int specialty;
    double performance;
    int interventionCount;
    is>>specialty>>interventionCount>>performance;
    technician.setSpecialty(specialty);
    technician.setInterventionCount(interventionCount);
    technician.setPerformance(performance);
    return is;
}




