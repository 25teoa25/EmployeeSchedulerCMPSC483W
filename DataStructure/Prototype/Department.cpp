#include "Department.h"

// Constructor
Department::Department(const std::string& name) : name(name) {}

// Set staffing needs for a specific shift and employee type
void Department::setStaffingNeeds(int shift, const std::string& employeeType, int count) {
    staffingNeeds[shift][employeeType] = count;
}

// Get staffing needs for a specific shift and employee type
int Department::getStaffingNeeds(int shift, const std::string& employeeType) const {
    if (staffingNeeds.find(shift) != staffingNeeds.end() && staffingNeeds.at(shift).find(employeeType) != staffingNeeds.at(shift).end()) {
        return staffingNeeds.at(shift).at(employeeType);
    }
    return 0; // If no staffing need is found, return 0
}
