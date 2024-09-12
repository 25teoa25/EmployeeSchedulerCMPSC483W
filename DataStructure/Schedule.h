#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "Department.h"
#include "Employee.h"
#include <map>
#include <vector>
#include <unordered_map>

// Schedule class definition
class Schedule {
public:
    // Departments and employees in the system
    std::unordered_map<std::string, Department> departments; // Department name to Department object
    std::unordered_map<std::string, Employee> employees;     // Employee ID to Employee object

    // Schedule structure: [Department][Shift] -> List of Employee IDs
    std::map<std::string, std::map<int, std::vector<std::string>>> schedule;

    // Method to add a department
    void addDepartment(const Department& department);

    // Method to add an employee
    void addEmployee(const Employee& employee);

    // Method to assign an employee to a shift in a department
    void assignShift(const std::string& department, int shift, const std::string& employeeId);

    // Method to check if an employee can be assigned to a shift (constraints are checked here)
    bool canAssignShift(const std::string& department, int shift, const std::string& employeeId) const;

    // Utility method to print the schedule
    void printSchedule() const;
};

#endif // SCHEDULE_H
