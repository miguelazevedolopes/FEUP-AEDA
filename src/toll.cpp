#include "toll.h"
#include <algorithm>
#ifdef _WIN32
inline int getchar_unlocked() { return _getchar_nolock(); }
#endif

const string &Toll::getName() const {
    return name;
}

const string &Toll::getLocation() const {
    return location;
}

const vector<Lane *> &Toll::getLanes() const {
    return lanes;
}

Lane *Toll::getRecommendedLane(bool isViaVerde) {
    if (lanes.empty())
        return nullptr;

    vector<Lane *> recommendedLanes;

    if (isViaVerde)
        recommendedLanes = getViaVerdeLanes();
    else
        recommendedLanes = getNormalLanes();

    if (recommendedLanes.empty())
        return nullptr;

    int c = -1;
    while (c < 0 || c >= recommendedLanes.size()) {
        for (int i = 0; i < recommendedLanes.size(); ++i) {
            cout << "NUMBER OF CROSSINGS TODAY: " << recommendedLanes[i]->getNumCrossings() << endl;
            recommendedLanes[i]->printLaneNumbered(i);
        }
        cout << "WE RECOMMEND LANE 0 THAT HAS THE FEWEST CROSSINGS FOR TODAY\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin.clear();
        c = getchar_unlocked() - '0';
    }
    return recommendedLanes[c];
}

vector<Lane *> Toll::getTypeLanes(bool isViaVerde) {
    vector<Lane *> typeLanes;

    if (!lanes.empty()) {
        for (auto lane : lanes) {
            if (lane->isViaVerde() == isViaVerde) {
                typeLanes.push_back(lane);
            }
        }
    }
    sort(typeLanes.begin(), typeLanes.end()); //sort so lane with less cars appears first
    return typeLanes;
}

vector<Lane *> Toll::getViaVerdeLanes() {
    return getTypeLanes(true);
}

vector<Lane *> Toll::getNormalLanes() {
    return getTypeLanes(false);
}

double Toll::getPrice() const {
    return price;
}

void Toll::addLane(Lane *lane) {
    lanes.push_back(lane);
}

void Toll::addLane(ViaVerdeLane *lane) {
    lanes.push_back(lane);
}

void Toll::removeLaneAt(int i) {
    lanes.erase(lanes.begin() + i);
}

void Toll::printTollNumbered(int i) const {
    cout << i << ": " << getName() << endl;
    cout << "LOCATION: " << getLocation() << endl;
    if (isExitToll()) cout << "EXIT TOLL\n\n";
    else cout << "ENTRY TOLL\n\n";
}

void Toll::viewLanes() const {

    for (int i = 0; i < getLanes().size(); ++i) {
        getLanes()[i]->printLaneNumbered(i + 1);
    }
    if (this->isExitToll()) {
        while (true) {
            int j;
            cout << "SELECT A LANE TO VIEW ITS PAST EMPLOYEES OR ENTER 0 TO GO BACK\n\n";
            cin >> j;
            if (j == 0) return;
            else if (getLanes()[j - 1]->isNormalExitLane() && j > 0 && j <= getLanes().size()) {
                if (getLanes()[j - 1]->getLastEmployees().empty()) {
                    cout << "NO EMPLOYEE HISTORY TO SHOW\n";
                    return;
                }
                for (auto x:getLanes()[j - 1]->getLastEmployees()) {
                    cout << x->getName() << endl;
                    cout << x->getSsNumber() << endl << endl;
                }
            } else cout << "ONLY NORMAL EXIT LANES HAVE EMPLOYEES\n";
        }
    }

}

ostream &operator<<(ostream &os,Toll &toll) {
    os << toll.isExitToll() << " " << toll.name << " " << toll.location << " " << toll.lanes.size() << " "
       << toll.position << " " << toll.price<<" "<<toll.technicians.size();
    if (!toll.lanes.empty()) {
        os << endl;
        for (int i = 0; i < toll.lanes.size(); ++i) {
            os << *toll.lanes[i];
            if (i < toll.lanes.size() - 1)
                os << endl;
        }
        if(!toll.technicians.empty())
            os<<endl;
    }
    while(!toll.technicians.empty()){
        os<<toll.technicians.top();
        toll.technicians.pop();
        if(!toll.technicians.empty())
            os<<endl;
    }
    delete &toll;
    return os;
}

istream &operator>>(istream &is, Toll &toll) {
    return toll.loadFromFile(is);
}


istream &Toll::loadFromFile(istream &is) {
    string name1, location1;
    int numLanes, pos;
    double price1;
    int sizeTechs;
    vector<Lane *> lanes1;
    if (is >> name1>> location1 >> numLanes >> pos >> price1>>sizeTechs) {
        lanes1.reserve(numLanes);
        for (int i = 0; i < numLanes; i++) {
            lanes1.push_back(loadLaneFromFile(is));
        }
        setName(name1);
        setLocation(location1);
        setLanes(lanes1);
        setPosition(pos);
        setPrice(price1);
    }
    if(sizeTechs>0) {
        Technician tech;
        is >> tech;
        technicians.push(tech);
    }
    return is;
}

void Toll::setName(const string &basicString) {
    Toll::name = basicString;
}

void Toll::setLocation(const string &basicString) {
    Toll::location = basicString;
}

void Toll::setLanes(const vector<Lane *> &lanes1) {
    Toll::lanes = lanes1;
}

void Toll::setPosition(int i) {
    Toll::position = i;
}

void Toll::setPrice(double d) {
    Toll::price = d;
}

int Toll::getPosition() {
    return position;
}
void Toll::addTechnician(int specialty){
    Technician technician(specialty);
    technicians.push(technician);
}
Technician Toll::getTechnician(int technicianSpecialty) {
    Technician technician(-1);
    vector<Technician> temp;
    while(!technicians.empty()){
        technician=technicians.top();
        technicians.pop();
        if(technicians.top().getSpecialty()==technicianSpecialty){
            for(auto x:temp){
                technicians.push(x);
            }
            return technician;
        }
        temp.push_back(technician);
    }
    for(auto x:temp) {
        technicians.push(x);
    }
    return technician;
}

void Toll::updateTechnician(Technician tech) {
    technicians.push(tech);
}

bool Toll::technicianExists(int spec) {
    vector<Technician> temp;
    while(!technicians.empty()){
        if(technicians.top().getSpecialty()==spec){
            for(auto x:temp){
                technicians.push(x);
            }
            return true;
        }
        temp.push_back(technicians.top());
        technicians.pop();
    }
    for(auto x:temp){
        technicians.push(x);
    }
    return false;
}


Toll::Toll() = default;


bool InToll::isExitToll() const {
    return false;
}

bool OutToll::isExitToll() const {
    return true;
}

InToll::InToll(const string &n, const string &loc, const vector<Lane *> &l, const int &pos, const double &price) :
        Toll(n, loc, l, pos, price) {}

Lane *InToll::loadLaneFromFile(istream &is) {
    bool viaVerde;

    if (is >> viaVerde) {
        if (viaVerde) {
            auto *viaVerdeLane = new ViaVerdeLane();
            is >> *viaVerdeLane;
            return viaVerdeLane;
        } else {
            auto *normalLane = new NormalLane();
            is >> *normalLane;
            return normalLane;
        }
    }
    return nullptr;
}

InToll::InToll() = default;

OutToll::OutToll(const string &n, const string &loc, const vector<Lane *> &l, const int &pos, const double &price) :
        Toll(n, loc, l, pos, price) {}

OutToll::OutToll() = default;

Lane *OutToll::loadLaneFromFile(istream &is) {
    bool viaVerde;
    is >> viaVerde;
    if (viaVerde) {
        auto *viaVerdeLane = new ViaVerdeLane();
        is >> *viaVerdeLane;
        return viaVerdeLane;
    } else {
        auto *normalExitLane = new NormalExitLane();
        is >> *normalExitLane;
        return normalExitLane;
    }

}
