#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <array>

enum Availability {
    CANNOT_WORK = 0,
    DO_NOT_WANT_TO_WORK = 1,
    WANT_TO_WORK = 2
};

class Employee {
public:
    std::string id;
    std::string name;
    std::string type;
    std::array<int, 42> availability; // Array of size 42 for shifts 1-42

    // Default Constructor
    Employee() : id(""), name(""), type("") {}

    // Constructor
    Employee(const std::string& id, const std::string& name, const std::string& type);

    void setAvailability(int shift, Availability avail);
    int getAvailability(int shift) const;
};

#endif // EMPLOYEE_H
