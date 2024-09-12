#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>
#include <unordered_map>

// Department class definition
class Department {
public:
    std::string name; // Department name
    // Shift number to Employee type to the required count of employees
    std::unordered_map<int, std::unordered_map<std::string, int>> staffingNeeds; 

    // Constructor
    Department(const std::string& name);

    // Method to set staffing needs for a shift
    void setStaffingNeeds(int shift, const std::string& employeeType, int count);

    // Method to get staffing needs for a shift
    int getStaffingNeeds(int shift, const std::string& employeeType) const;
};

#endif // DEPARTMENT_H
