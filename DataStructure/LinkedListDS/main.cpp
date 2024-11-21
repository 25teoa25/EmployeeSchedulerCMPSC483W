#include "CSVParser.h"
#include "NurseFunctions.h"
#include "NurseList.h"
#include "LinearPgHelper.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include "json.hpp"
#include "../../Algorithm/neighbor.h"
using json = nlohmann::json;
using namespace std;

int main() {
    // Parse the nurse CSV file to populate the data structure

    // CHANGE THIS TO YOUR DIRECTORY
    parseNursesCSV("/Users/alexteo/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");

    // Parse the constraints CSV file

    // CHANGE THIS TO YOUR DIRECTORY
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

    // -------------------------- Linear Programming Implementation -------------------------------
    // Make sure to put in proper file paths

    ShiftSchedule shiftScheduleLP(42); // LP Shift schedule to be outputted

    // Iterate over all departments in departmentNursesMap
    for (const auto &departmentEntry : departmentNursesMap) {
        const string &department = departmentEntry.first; // Department name
        cout << "Scheduling for department: " << department << endl;

        // Iterate over each nurse type in the current department
        for (const auto &nurseTypeEntry : departmentEntry.second) {
            const string &nurseType = nurseTypeEntry.first; // Nurse type (RN, LPN, etc.)
            auto &nurses = nurseTypeEntry.second;           // List of nurses for this type

            // Iterate over 14 days
            for (int day = 1; day <= 14; ++day) {
                // Shifts in focus
                int morningShiftNumber = (day - 1) * 3;
                int eveningShiftNumber = (day - 1) * 3 + 1;
                int nightShiftNumber = (day - 1) * 3 + 2;

                // Assign the demand for each shift (morning, evening, night) for this day
                int dm = sortedConstraintsMap[morningShiftNumber][department][nurseType]; // Demand for morning shift
                int de = sortedConstraintsMap[eveningShiftNumber][department][nurseType]; // Demand for evening shift
                int dn = sortedConstraintsMap[nightShiftNumber][department][nurseType];   // Demand for night shift

                // Generate a random number to decide which shift to focus on (1 = morning, 2 = evening, 3 = night)
                int randomShift = generateRandomShift();

                if (randomShift == 1) { // Optimize preferences for random morning shift
                    optimizePreferenceAssignment(shiftScheduleLP, department, nurseType, morningShiftNumber, dm);
                    simpleAssignment(shiftScheduleLP, department, nurseType, eveningShiftNumber, de); // Assign nurses to evening shift without optimizing preference
                    simpleAssignment(shiftScheduleLP, department, nurseType, nightShiftNumber, dn);   // Assign nurses to night shift without optimizing preference
                } else if (randomShift == 2){ // Optimize preferences for random evening shift
                    optimizePreferenceAssignment(shiftScheduleLP, department, nurseType, eveningShiftNumber, de);
                    simpleAssignment(shiftScheduleLP, department, nurseType, morningShiftNumber, dm); // Assign nurses to morning shift without optimizing preference
                    simpleAssignment(shiftScheduleLP, department, nurseType, nightShiftNumber, dn);   // Assign nurses to night shift without optimizing preference
                } else { // Optimize preferences for random night shift
                    optimizePreferenceAssignment(shiftScheduleLP, department, nurseType, nightShiftNumber, dn);
                    simpleAssignment(shiftScheduleLP, department, nurseType, morningShiftNumber, dm); // Assign nurses to morning shift without optimizing preference
                    simpleAssignment(shiftScheduleLP, department, nurseType, eveningShiftNumber, de); // Assign nurses to night shift without optimizing preference
                }
            }
            
            // Run each of the neighborhood structures on each nurse type in each department
            for (int count = 0; count <= 99; ++count) {
                satisfactionScoreLinearProgramming = structure1(shiftScheduleLP, department, satisfactionScoreLinearProgramming, nurseType);
                satisfactionScoreLinearProgramming = structure2(shiftScheduleLP, department, satisfactionScoreLinearProgramming, nurseType);
                satisfactionScoreLinearProgramming = structure3(shiftScheduleLP, department, satisfactionScoreLinearProgramming, nurseType);
                satisfactionScoreLinearProgramming = structure4(shiftScheduleLP, department, satisfactionScoreLinearProgramming, nurseType);
                satisfactionScoreLinearProgramming = structure5(shiftScheduleLP, department, satisfactionScoreLinearProgramming, nurseType);
                satisfactionScoreLinearProgramming = structure6(shiftScheduleLP, department, satisfactionScoreLinearProgramming, nurseType);
                satisfactionScoreLinearProgramming = structure7(shiftScheduleLP, department, satisfactionScoreLinearProgramming, nurseType);
                satisfactionScoreLinearProgramming = structure8(shiftScheduleLP, department, satisfactionScoreLinearProgramming, nurseType);
            }
        }
    }

    cout << "Sat score of LP: " << satisfactionScoreLinearProgramming << endl;
    // ------------------------------------------------------------------------------------------

        
    return 0;
}
