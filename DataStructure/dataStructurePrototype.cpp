#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

// Availability levels represented as integers
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

    Employee(const std::string& id, const std::string& name, const std::string& type) 
        : id(id), name(name), type(type) {
        availability.fill(DO_NOT_WANT_TO_WORK); // Default to "Do not want to work"
    }

    void setAvailability(int shift, Availability avail) {
        if (shift >= 1 && shift <= 42) {
            availability[shift - 1] = avail; // Shifts are 1-based; array is 0-based
        }
    }

    int getAvailability(int shift) const {
        return availability[shift - 1]; // Returns availability for a given shift
    }
};

class Department {
public:
    std::string name;
    std::unordered_map<int, std::unordered_map<std::string, int>> staffingNeeds; // Shift number to Employee type to count

    Department(const std::string& name) : name(name) {}

    void setStaffingNeeds(int shift, const std::string& employeeType, int count) {
        staffingNeeds[shift][employeeType] = count;
    }

    int getStaffingNeeds(int shift, const std::string& employeeType) const {
        if (staffingNeeds.find(shift) != staffingNeeds.end() && staffingNeeds.at(shift).find(employeeType) != staffingNeeds.at(shift).end()) {
            return staffingNeeds.at(shift).at(employeeType);
        }
        return 0; // If no staffing need is found, return 0
    }
};

class Schedule {
public:
    std::unordered_map<std::string, Department> departments;
    std::unordered_map<std::string, Employee> employees;

    // Schedule structure: [Department][Shift] -> List of Employee IDs
    std::map<std::string, std::map<int, std::vector<std::string>>> schedule;

    void addDepartment(const Department& department) {
        departments[department.name] = department;
    }

    void addEmployee(const Employee& employee) {
        employees[employee.id] = employee;
    }

    void assignShift(const std::string& department, int shift, const std::string& employeeId) {
        if (canAssignShift(department, shift, employeeId)) {
            schedule[department][shift].push_back(employeeId);
        }
    }

    bool canAssignShift(const std::string& department, int shift, const std::string& employeeId) {
        // Implement logic to check constraints:
        // - Maximum 10 shifts in 2 weeks
        // - No more than 2 shifts in a row
        // - Availability constraints
        if (employees.find(employeeId) == employees.end()) return false; // Employee not found
        if (employees[employeeId].getAvailability(shift) == CANNOT_WORK) return false; // Employee cannot work this shift

        // Additional constraints to be implemented here
        return true;
    }
};
