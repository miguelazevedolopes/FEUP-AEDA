#include "highway.h"

const string &Highway::getName() const {
    return name;
}

Toll *Highway::getTollAt(int i, bool exit) {
    int cnt = 0;
    vector<Toll *>::iterator it;
    if (!tolls.empty() && exit) {
        for (it = tolls.begin(); it != tolls.end(); it++) {
            if ((*it)->isExitToll()) {
                cnt++;
            }
            if (cnt == i) {
                return *it;
            }
        }
    } else if (!tolls.empty() && !exit) {
        for (it = tolls.begin(); it != tolls.end(); it++) {
            if (!(*it)->isExitToll()) {
                cnt++;
            }
            if (cnt == i) {
                return *it;
            }
        }
    }
    return nullptr;
}

void Highway::printTollsNumbered(bool isExit) {
    vector<Toll *>::const_iterator it;
    int i = 1;
    for (it = tolls.begin(); it != tolls.end(); it++) {
        if ((*it)->isExitToll() == isExit) {
            cout << i << ": " << (*it)->getName() << endl;
            i++;
        }
    }
}

void Highway::addToll(Toll *toll) {
    tolls.push_back(toll);
}

vector<Toll *> &Highway::getTolls() {
    return tolls;
}

void Highway::printTollsNumbered() {
    if (!tolls.empty())
        for (int i = 0; i < tolls.size(); ++i) {
            tolls[i]->printTollNumbered(i + 1);
        }
}

ostream &operator<<(ostream &os, const Highway &highway) {
    os << highway.name << " " << highway.tolls.size() << endl;
    if (!highway.tolls.empty())
        for (int i = 0; i < highway.tolls.size(); ++i) {
            os << *highway.tolls[i];
            if (i != highway.tolls.size() - 1)
                os << endl;
        }
    delete &highway;
    return os;
}

istream &operator>>(istream &is, Highway &highway) {
    string highway_name;
    int numTolls;
    if (is >> highway_name >> numTolls) {
        highway.setName(highway_name);

        bool type;
        while (numTolls > 0 && is >> type) {
            if (type)//type = true -> is exit toll
            {
                auto *outToll = new OutToll();
                is >> *outToll;
                highway.addToll(outToll);
            } else {
                auto *inToll = new InToll();
                is >> *inToll;
                highway.addToll(inToll);
            }
            numTolls--;
        }
    }
    return is;
}

int Highway::getTollsSize(bool exit) {
    int cnt = 0;
    for (auto x:tolls) {
        if (x->isExitToll() == exit) {
            cnt++;
        }
    }
    return cnt;
}

void Highway::setName(const string &name1) {
    Highway::name = name1;
}

int Highway::repair(int i, Toll *pToll, Intervention *pIntervention) {
    Technician technicianOnCall=pToll->getTechnician(i);
    if(technicianOnCall.getSpecialty()==-1){
        technicianOnCall=getClosestTechnician(i,pToll);
        technicianOnCall.addIntervention(pIntervention->getDuration());
        pToll->updateTechnician(technicianOnCall);
        return 1;
    }
    else{
        technicianOnCall.addIntervention(pIntervention->getDuration());
        pToll->updateTechnician(technicianOnCall);
        return 1;
    }

}


Technician Highway::getClosestTechnician(int i, Toll *pToll) {
    int min=99999;
    Technician borrowedTechnician(-1);
    for(auto x:tolls){
        if (abs(x->getPosition()-pToll->getPosition())<min){
            if (x->getTechnician(i).getSpecialty()!=-1) {
                min = abs(x->getPosition() - pToll->getPosition()) < min;
                borrowedTechnician = x->getTechnician(i);
            }
        }
    }
    return borrowedTechnician;

}

bool Highway::techExistsInHighway(int spec) {
    for(auto x:tolls){
        if(x->technicianExists(spec))
            return true;
    }
    return false;
}

Highway::Highway() = default;

