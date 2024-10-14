#include "CSVParser.h"
#include "NurseFunctions.h"
//#include "NurseList.h"
#include <iostream>

int main() {
    // Parse the nurse CSV file to populate the data structure
    parseNursesCSV("/Users/alexteo/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");

    // Example usage: View nurses by department and type
    viewNursesByDepartmentAndType("Oncology", "LPN");
    viewNursesByDepartmentAndType("Surgery", "RN");

    return 0;
}
