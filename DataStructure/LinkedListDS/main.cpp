#include "CSVParser.h"
#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>

int main() {
    // Parse the nurse CSV file to populate the data structure
    parseNursesCSV("/Users/alexteo/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");

    // Parse the constraints CSV file
    parseConstraintsCSV("/Users/alexteo/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse Constraints Test.csv");

    // Print out the constraints
    for (const auto& shiftPair : constraintsMap) {
        std::cout << "Shift: " << shiftPair.first << std::endl;
        for (const auto& deptPair : shiftPair.second) {
            std::cout << "  Department: " << deptPair.first << std::endl;
            for (const auto& typePair : deptPair.second) {
                std::cout << "    Nurse Type: " << typePair.first
                          << ", Required: " << typePair.second << std::endl;
            }
        }
    }

    // Example usage: View nurses by department and type
    viewNursesByDepartmentAndType("Oncology", "LPN");
    viewNursesByDepartmentAndType("Surgery", "RN");
    viewNursesByDepartmentAndTypeJSON("Surgery", "RN");

    // Retrieve the vector of nurses of the specific type
    // You can then traverse these nurse objects in the vector
    // std::vector<Nurse>& nurses = departmentNursesMap["department name"]["nurseType name"];

    return 0;
}
