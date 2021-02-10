#include "trip.h"

#include <utility>

Trip::Trip(string begLocation, double begPrice, Time *begTime) : beginTime(begTime), endTime(new Time(0, 0, 0)) {
    pricePaid = 0;
    begin.first = std::move(begLocation);
    begin.second = begPrice;
    end.first = "empty";
    end.second = 0;
    this->finished = false;
}

Trip::Trip(const pair<string, double> &begin, const pair<string, double> &anEnd, Time *beginTime, Time *endTime,
           bool finished, double pricepaid) : begin(begin), end(anEnd), beginTime(beginTime), endTime(endTime),
                                              finished(finished), pricePaid(pricepaid) {}

void Trip::setEnd(string location, double price) {
    end.first = std::move(location);
    end.second = price;
    finished = true;
}

void Trip::setEndTime(Time *time) {
    endTime = time;
}

pair<string, double> Trip::getBegin() const {
    return begin;
}

pair<string, double> Trip::getEnd() const {
    return end;
}

bool Trip::isFinished() const {
    return finished;
}

Time *Trip::getEndTime() const {
    return endTime;
}

void Trip::setPrice(double price) {
    pricePaid = price;
}

double Trip::getPrice() const {
    return pricePaid;
}

ostream &operator<<(ostream &os, const Trip &trip) {
    os << trip.begin.first << " " << trip.begin.second << " " << trip.end.first << " " << trip.end.second << " "
       << *trip.beginTime << " " << *trip.endTime << " " << trip.finished << " " << trip.pricePaid;

    delete &trip;
    return os;
}

istream &operator>>(istream &is, Trip &trip) {
    pair<string, double> begin, end;
    Time *beginTime = new Time;
    Time *endTime = new Time;
    bool finished;
    double pricePaid;

    is >> begin.first >> begin.second >> end.first >> end.second >> *beginTime >> *endTime >> finished >> pricePaid;

    trip = Trip(begin, end, beginTime, endTime, finished, pricePaid);

    return is;
}

Trip::Trip() = default;
