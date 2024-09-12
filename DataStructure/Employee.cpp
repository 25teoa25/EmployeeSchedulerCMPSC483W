#include "Employee.h"

Employee::Employee(const std::string& id, const std::string& name, const std::string& type) 
    : id(id), name(name), type(type) {
    availability.fill(DO_NOT_WANT_TO_WORK); // Default to "Do not want to work"
}

void Employee::setAvailability(int shift, Availability avail) {
    if (shift >= 1 && shift <= 42) {
        availability[shift - 1] = avail;
    }
}

int Employee::getAvailability(int shift) const {
    return availability[shift - 1];
}
