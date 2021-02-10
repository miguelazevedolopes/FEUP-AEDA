#ifndef AEDA_T8G6_EMPLOYEE_H
#define AEDA_T8G6_EMPLOYEE_H

#include <string>
#include <ostream>

using namespace std;

///Contains info about an employee, such as name, ss_number and working status
///
///The private attribute "working" will be true if the employee is working, or false otherwise

class Employee {
private:
    ///Employee's name
    string name;
    ///Employee's ss number
    int ss_number;
    ///Employee's working status
    ///
    ///true if employee is working, false otherwise
    bool working; //IF EMPLOYEE IS WORKING IN A LANE, VALUE TRUE

public:
    ///Creates Employee with undefined private attributes
    Employee();

    explicit Employee(int ssNumber);

    ///Creates an Employee with name and ss_number and sets working status
    Employee(string n, int ss,bool work=false): name(n), ss_number(ss),working(work){};

    ///Destructor
    ~Employee()= default;;

    ///Returns Employee's name
    ///
    /// \return Employee's name
    const string &getName() const;

    ///Returns Employee's ss number
    ///
    /// \return Employee's ss number
    int getSsNumber() const;

    ///Returns true, if ss_numbers are equal, false otherwise
    ///
    /// \param rhs is another Employee
    /// \return true, if ss_numbers are equal, false otherwise
    bool operator==(const Employee &rhs) const;

    ///Returns true, if the Employee is working, false otherwise
    ///
    /// \return true, if the Employee is working, false otherwise
    bool isWorking() const;

    ///Changes the boolean member working
    ///
    ///If working is true it becomes false and vice-versa
    void changeWorkStatus();

    ///Returns ostream with name and ss_number
    ///
    /// \param os
    /// \param employee
    /// \return ostream with Employee's name and ss number
    friend ostream &operator<<(ostream &os, const Employee &employee);

    ///Return istream with name and ss_number
    ///
    /// \param is
    /// \param employee
    /// \return istream with name and ss_number
    friend istream &operator>>(istream &is, Employee &employee);

    ///Sets Employee name
    ///
    /// \param name
    void setName(const string &name);

    ///Sets Employee ss number
    ///
    /// \param ssNumber
    void setSsNumber(int ssNumber);
};


#endif //AEDA_T8G6_EMPLOYEE_H
