#include "CSVParser.h"
#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>

int main() {
    // Parse the nurse CSV file to populate the data structure
    parseNursesCSV("/Users/rstack/Documents/CMPSC483W/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");

    // Parse the constraints CSV file
    parseConstraintsCSV("/Users/rstack/Documents/CMPSC483W/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseContraints.csv");

    // Print out the constraints
    for (const auto& shiftPair : constraintsMap) {
        std::cout << "Shift: " << shiftPair.first << std::endl;
        for (const auto& deptPair : shiftPair.second) {
            std::cout << "  Department: " << deptPair.first << std::endl;
            for (const auto& typePair : deptPair.second) {
                std::cout << typePair.first
                          << ", Required: " << typePair.second << std::endl;
            }
        }
    }

    // Example usage: View nurses by department and type
    //viewNursesByDepartmentAndType("Oncology", "LPN");
    //viewNursesByDepartmentAndType("Surgery", "RN");
    //viewNursesByDepartmentAndTypeJSON("Surgery", "RN");

    // Retrieve the vector of nurses of the specific type
    // You can then traverse these nurse objects in the vector
    // std::vector<Nurse>& nurses = departmentNursesMap["department name"]["nurseType name"];


     //brute force code
    // Example debug print statement to check the list of Registered Nurses in Oncology
    std::cout << "Registered Nurses in Oncology:\n";
    for (const auto& nurse : departmentNursesMap["Oncology"]["RN"]) {
        std::cout << "  " << nurse.fullName << " (" << nurse.nurseType << ")\n";
    }
    
    // Iterate over each shifts 1-42
    for (const auto& shiftPair : constraintsMap) {
        int shift = shiftPair.first;
        std::cout << shift << std::endl;
        ShiftSchedule shiftSchedule(42);

        // Iterate over each department in the current shift
        for (const auto& deptPair : shiftPair.second) {
            const std::string& department = deptPair.first;
            std::cout << "Department1:" << department << std::endl;
            //ShiftSchedule shiftSchedule(42);
            // Map to store assigned nurses for the current department in this shift
            std::map<std::string, std::vector<std::string>> assignedNurses;

        // Iterate over each nurse type in the current department
            for (const auto& typePair : deptPair.second) {
                const std::string& nurseType = typePair.first;

                // Create a copy of nurseType to modify
            std::string modifiedNurseType = nurseType;
            const std::string prefix = "Nurse Type: ";

            // Check if the prefix exists at the start and remove it
            if (modifiedNurseType.rfind(prefix, 0) == 0) {  // rfind returns 0 if prefix is at the start
                modifiedNurseType = modifiedNurseType.substr(prefix.length());  // Remove the prefix
            }

            if (modifiedNurseType == "Nurse Assistant"){
                modifiedNurseType = "NA";
            }
            else if (modifiedNurseType == "Registered Nurse"){
                modifiedNurseType = "RN";
            }
            else if (modifiedNurseType == "Licensed Practical Nurse"){
                modifiedNurseType = "LPN";
            }
                int nursesNeeded = typePair.second;
                //std::cout << ":::" << modifiedNurseType << ":::" << nursesNeeded << std::endl;

                // Get the list of available nurses for the department and type
                auto& availableNurses = departmentNursesMap[department][modifiedNurseType];
                std::cout << department << nurseType << std::endl;

                // Assign nurses up to the required count (nursesNeeded)
                int assignedCount = 0;
                for (const auto& nurse : availableNurses) {
                    if (assignedCount >= nursesNeeded) break;
                    add(shiftSchedule, shift, nurse); // Add nurse to shift schedule
                    
                    //std::cout << "HERE" << nurse.nurseNumber << std::endl;
                    assignedCount++;
                }  

            }
        
    
        
        }
        printShiftSchedule(shiftSchedule);
        std::cout << "ENDING";
    }

   
            
        
    return 0;
}
