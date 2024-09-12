#include "Schedule.h"
#include <iostream>

// Add a department to the schedule
void Schedule::addDepartment(const Department& department) {
    departments[department.name] = department;
}

// Add an employee to the schedule
void Schedule::addEmployee(const Employee& employee) {
    employees[employee.id] = employee;
}

// Assign an employee to a specific shift in a department
void Schedule::assignShift(const std::string& department, int shift, const std::string& employeeId) {
    if (canAssignShift(department, shift, employeeId)) {
        schedule[department][shift].push_back(employeeId);
    } else {
        std::cerr << "Cannot assign " << employeeId << " to shift " << shift << " in department " << department << " due to constraints." << std::endl;
    }
}

// Check if an employee can be assigned to a specific shift in a department
bool Schedule::canAssignShift(const std::string& department, int shift, const std::string& employeeId) const {
    if (employees.find(employeeId) == employees.end()) return false; // Employee not found
    if (employees.at(employeeId).getAvailability(shift) == CANNOT_WORK) return false; // Employee cannot work this shift

    // Additional constraints should be implemented here (e.g., max shifts in a period, consecutive shifts, etc.)
    return true;
}

// Print the current schedule
void Schedule::printSchedule() const {
    for (const auto& dept : schedule) {
        std::cout << "Department: " << dept.first << std::endl;
        for (const auto& shift : dept.second) {
            std::cout << "  Shift " << shift.first << ": ";
            for (const auto& empId : shift.second) {
                std::cout << empId << " ";
            }
            std::cout << std::endl;
        }
    }
}
