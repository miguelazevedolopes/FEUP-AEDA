#include "time.h"
#include <sstream>

int Time::getHour() const {
    return hour;
}

int Time::getMinute() const {
    return minute;
}

int Time::getSecond() const {
    return second;
}

void Time::setTime(int h, int m, int s) {
    hour = h;
    minute = m;
    second = s;
}

Time &Time::operator=(const Time &rhs) {
    setTime(rhs.getHour(), rhs.getMinute(), rhs.getSecond());
    setYear(rhs.year);
    setMonth(rhs.month);
    setDay(rhs.day);
    return *this;
}

string Time::getDate() const {
    stringstream ss;

    ss << day << "/" << month << "/" << year << " " << hour << ":" << minute << ":" << second;
    return ss.str();
}

Time::Time(int day, int month, int year, int hour, int minute, int second) : day(day), month(month), year(year),
                                                                             hour(hour), minute(minute),
                                                                             second(second) {}

ostream &operator<<(ostream &os, const Time &time1) {
    os << time1.day << " " << time1.month << " " << time1.year << " " << time1.hour << " " << time1.minute << " "
       << time1.second;
    delete &time1;
    return os;
}

istream &operator>>(istream &is, Time &time) {
    int day, month, year, hour, minute, second;
    is >> day >> month >> year >> hour >> minute >> second;
    time = Time(day, month, year, hour, minute, second);
    return is;
}

void Time::setDay(int day1) {
    Time::day = day1;
}

void Time::setMonth(int month1) {
    Time::month = month1;
}

void Time::setYear(int year1) {
    Time::year = year1;
}

void Time::setHour(int hour1) {
    Time::hour = hour1;
}

void Time::setMinute(int minute1) {
    Time::minute = minute1;
}

void Time::setSecond(int second1) {
    Time::second = second1;
}

int Time::getDay() const {
    return day;
}

int Time::getMonth() const {
    return month;
}

int Time::getYear() const {
    return year;
}
