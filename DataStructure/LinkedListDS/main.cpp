#include "CSVParser.h"
#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>

int main() {
    // Parse the nurse CSV file to populate the data structure
    parseNursesCSV("/Users/rstack/Documents/CMPSC483W/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");

    // Parse the constraints CSV file
    parseConstraintsCSV("/Users/rstack/Documents/CMPSC483W/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse Constraints Test.csv");

    // Example usage: View nurses by department and type
    viewNursesByDepartmentAndType("Oncology", "LPN");
    viewNursesByDepartmentAndType("Surgery", "RN");
    viewNursesByDepartmentAndTypeJSON("Surgery", "RN");

    // Example: Get the required RN nurses for Shift 1 in Oncology
    int rnRequired = constraintsMap[1]["Oncology"]["RN"];
    std::cout << "Required RN nurses for Shift 1 in Oncology: " << rnRequired << std::endl;

    // Retrieve the vector of nurses of the specific type
    std::vector<Nurse>& nurses = departmentNursesMap["department name"]["nurseType name"];

    return 0;
}
