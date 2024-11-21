#include "CSVParser.h"
#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;

int main() {
    // Parse the nurse CSV file to populate the data structure
    parseNursesCSV("/Users/alexteo/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");

    // Parse the constraints CSV file
    parseConstraintsCSV("/Users/alexteo/EmployeeSchedulerCMPSC483W/NurseContraints.csv");

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
    /*std::cout << "Registered Nurses in Oncology:\n";
    for (const auto& nurse : departmentNursesMap["Oncology"]["RN"]) {
        std::cout << "  " << nurse.fullName << " (" << nurse.nurseType << ")\n";
    }*/
    
    // Create sortedConstraintsMap with std::map for sorted keys
    std::map<int, std::unordered_map<std::string, std::unordered_map<std::string, int>>> sortedConstraintsMap(
        constraintsMap.begin(), constraintsMap.end()
    );

    int counter = 0;
    ShiftSchedule shiftSchedule(42);

    // Iterate over each shifts 1-42
    for (const auto& shiftPair : sortedConstraintsMap) {
        int shift = shiftPair.first;
        std::cout << shift << std::endl;

        // Iterate over each department in the current shift
        for (const auto& deptPair : shiftPair.second) {
            const std::string& department = deptPair.first;
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

                // Get the list of available nurses for the department and type
                auto& availableNurses = departmentNursesMap[department][modifiedNurseType];
                //std::cout << department << nurseType << std::endl;

                // Assign nurses up to the required count (nursesNeeded)
                int assignedCount = 0;

                // First, prioritize nurses with shiftPreferences[shift] = 2
                for (const auto& nurse : availableNurses) {
                    if (assignedCount >= nursesNeeded) break;
    
                    if (nurse.shiftPreferences[shift-1] == 2) {
                        add(shiftSchedule, shift, nurse); // Add nurse to shift schedule
                        assignedCount++;
                    }
                }

                // Next, add nurses with shiftPreferences[shift] = 1 if needed
                if (assignedCount < nursesNeeded) {
                for (const auto& nurse : availableNurses) {
                    if (assignedCount >= nursesNeeded) break;

                    if (nurse.shiftPreferences[shift-1] == 1) {
                        add(shiftSchedule, shift, nurse); // Add nurse to shift schedule
                        assignedCount++;
                    }
                }
            }

                // If there are still not enough nurses, output an error message
                if (assignedCount < nursesNeeded) {
                    std::cerr << "Error: Not enough nurses available for shift " << shift-1 << ". Needed: " 
                    << nursesNeeded << ", Assigned: " << assignedCount << std::endl;
                }
                

            }
        
    
        
        }
        counter++;
    }
    //printShiftSchedule(shiftSchedule);

   
            
    // Convert the ShiftSchedule to JSON
    std::string filename = "shift_schedule.json";
    shiftScheduleToJSON(shiftSchedule, filename);

    // Print the JSON
    //std::cout << jsonOutput.dump(4) << std::endl;

        
    return 0;
}
