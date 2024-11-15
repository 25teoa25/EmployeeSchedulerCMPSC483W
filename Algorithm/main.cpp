/* Shift specification algorithm runs first on the departmentNursesMap, which is fully populated
with all nurse information and preferences. The output of the shift specification algorithm is an
array for a specific department containing 42 indices, each index representing a shift. At each
index will be a vector of nurse objects which represents nurses scheduled for that shift. At
first, nurses will be randomly assigned to the shifts. We will mutate the output data structure
via running it through the neighborhood method. The data structure will be run through each neighborhood 
method x times. After running through each neighborhood method, the final schedule will be 
outputted through our mutated data structure. Also, the happiness score will be outputted for 
the entire data structure.
*/

#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include "../DataStructure/LinkedListDS/NurseFunctions.h"
#include "../DataStructure/LinkedListDS/CSVParser.h"
#include "../DataStructure/LinkedListDS/NurseList.h"
//#include "json.hpp"
//using json = nlohmann::json;
//#include "../Algorithm/neighbor.h"

using namespace std;

//to run this file right now:
//g++ -std=c++11 main.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/CSVParser.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseList.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseFunctions.cpp -o Testfile
//then: ./Testfile

#include "CSVParser.h"
#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;


// Function to randomly generate a number from 1 to 3 (morning, evening, night)
int generateRandomShift() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 3); // Random number between 1 and 3
    return dist(gen);
}

// Helper function to assign nurses to shifts based on their availability
void simpleAssignment(std::vector<std::vector<Nurse>>& shiftSchedule, 
                      const std::string& department, const std::string& nurseType, 
                      int shift, int& demandRemaining) {

    // Iterate through the nurses for the given department and nurse type
    for (auto& nurse : departmentNursesMap[department][nurseType]) {
        // Check if the nurse is not yet assigned to this shift
        if (nurse.isAssignedMap[shift] == 0) {
            // Assign the nurse to the shift
            add(shiftSchedule, shift, nurse);  // Add nurse to shift schedule

            // Mark the nurse as assigned for this shift
            nurse.isAssignedMap[shift] = 1;

            // Reduce the demand for the shift by 1
            --demandRemaining;

            // If the demand has been fulfilled, exit the loop
            if (demandRemaining <= 0) {
                break;
            }
        }
    }
}

// Helper function to assign nurses based on their shift preferences (optimize for preference)
void optimizePreferenceAssignment(std::vector<std::vector<Nurse>>& shiftSchedule, 
                                   const std::string& department, const std::string& nurseType, 
                                   int shift, int& demandRemaining) {
    bool assignedAny = false;

    // Loop through the nurses for the given department and nurse type
    for (auto& nurse : departmentNursesMap[department][nurseType]) {
        // Check if nurse is not already assigned to this shift and prefers this shift (preference = 2)
        if (nurse.isAssignedMap[shift] == 0 && nurse.shiftPreferences[shift] == 2) {
            // Assign the nurse to the shift
            add(shiftSchedule, shift, nurse);  // Add nurse to the shift schedule

            // Mark the nurse as assigned for this shift
            nurse.isAssignedMap[shift] = 1;

            // Reduce the demand for the shift by 1
            --demandRemaining;

            // Indicate that we have assigned at least one nurse
            assignedAny = true;

            // If the demand has been fulfilled, break the loop
            if (demandRemaining <= 0) {
                break;
            }
        }
    }

    // If demand is still not met, call simpleAssignment to fulfill remaining demand
    if (demandRemaining > 0) {
        simpleAssignment(shiftSchedule, department, nurseType, shift, demandRemaining);
    }
}


int main() {
    // Parse the nurse CSV file to populate the data structure
    parseNursesCSV("/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");

    // Parse the constraints CSV file
    parseConstraintsCSV("/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseContraints.csv");

    ShiftSchedule shiftSchedule(42);

    // Iterate over all departments in departmentNursesMap
    for (const auto& departmentEntry : departmentNursesMap) {
        const std::string& department = departmentEntry.first;  // Department name
        std::cout << "Scheduling for department: " << department << std::endl;

        // Iterate over each nurse type in the current department
        for (const auto& nurseTypeEntry : departmentEntry.second) {
            const std::string& nurseType = nurseTypeEntry.first;  // Nurse type (RN, LPN, etc.)
            auto& nurses = nurseTypeEntry.second;                  // List of nurses for this type

            // Iterate over 14 days
            for (int day = 1; day <= 14; ++day) {
                // You can perform shift assignment logic here for each day and nurse type
                std::cout << "Processing Day " << day << " for " << nurseType << " in " << department << std::endl;

                // Shifts in focus
                int morningShiftNumber = (day - 1) * 3 + 1;
                int eveningShiftNumber = (day - 1) * 3 + 2;
                int nightShiftNumber = (day - 1) * 3 + 3;

                // Assign the demand for each shift (morning, evening, night) for this day
                int dm = constraintsMap[morningShiftNumber][department][nurseType];  // Demand for Shift 1 (morning)
                int de = constraintsMap[eveningShiftNumber][department][nurseType];  // Demand for Shift 2 (evening)
                int dn = constraintsMap[nightShiftNumber][department][nurseType];  // Demand for Shift 3 (night)

                std::cout << "Demand for Day " << day << " (" << department << " - " << nurseType << "): "
                      << "Morning (dm) = " << dm << ", Evening (de) = " << de << ", Night (dn) = " << dn << std::endl;

                // Generate a random number to decide which shift to focus on (1 = morning, 2 = evening, 3 = night)
                int randomShift = generateRandomShift();
                std::cout << "Random shift chosen for day " << day << ": " << randomShift << std::endl;

                if (randomShift == 1) {
                    optimizePreferenceAssignment(shiftSchedule, department, nurseType, morningShiftNumber, dm);
                    simpleAssignment(shiftSchedule, department, nurseType, eveningShiftNumber, de);
                    simpleAssignment(shiftSchedule, department, nurseType, nightShiftNumber, dn);
                } else if (randomShift == 2) {
                    optimizePreferenceAssignment(shiftSchedule, department, nurseType, eveningShiftNumber, de);
                    simpleAssignment(shiftSchedule, department, nurseType, morningShiftNumber, dm);
                    simpleAssignment(shiftSchedule, department, nurseType, nightShiftNumber, dn);
                } else {
                    optimizePreferenceAssignment(shiftSchedule, department, nurseType, nightShiftNumber, dn);
                    simpleAssignment(shiftSchedule, department, nurseType, morningShiftNumber, dm);
                    simpleAssignment(shiftSchedule, department, nurseType, eveningShiftNumber, de);
                }

            }
        }
    }

    return 0;
}


// // Function to randomly generate a shift preference
// int generateRandomShift() {
//     static std::random_device rd;
//     static std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dist(1, 3);
//     return dist(gen); // Randomly pick between 1 (morning), 2 (evening), or 3 (night)
// }

// // Function to check if a nurse can be assigned based on constraints
// bool canAssignNurse(int shift, const std::string& department, const std::string& nurseType, int currentCount) {
//     if (constraintsMap.count(shift) && constraintsMap[shift].count(department) &&
//         constraintsMap[shift][department].count(nurseType)) {
        
//         int requiredCount = constraintsMap[shift][department][nurseType];
//         return currentCount < requiredCount;
//     }
//     return true; // Allow assignment if no constraint exists
// }

// // Function to implement the shift specification algorithm with cumulative satisfaction scoring
// void assignShifts(ShiftSchedule& schedule, const std::vector<Nurse>& nurses, const std::string& department, 
//                   const std::string& nurseType, int shift, int& totalSatisfaction) {
//     int dm = 0, de = 0, dn = 0;
//     int d1 = 2, d2 = 2, d3 = 1; // Example demand values for morning, evening, night

//     dm = d1; // Set the initial morning shift demand
//     de = d2; // Set the initial evening shift demand
//     dn = d3; // Set the initial night shift demand

//     int nurseCountForShift = 0;

//     for (const Nurse& nurse : nurses) {
//         if (nurse.department == department && shift <= nurse.shiftPreferences.size() &&
//             (nurse.shiftPreferences[shift - 1] == 1 || nurse.shiftPreferences[shift - 1] == 2) &&
//             canAssignNurse(shift, department, nurseType, nurseCountForShift)) {
            
//             int r = generateRandomShift(); // Randomize shift assignment
//             bool assigned = false;

//             // Assign a shift based on random number and demand
//             if (r == 1 && dm > 0) { // Morning shift
//                 dm--;
//                 assigned = true;
//             } else if (r == 2 && de > 0) { // Evening shift
//                 de--;
//                 assigned = true;
//             } else if (r == 3 && dn > 0) { // Night shift
//                 dn--;
//                 assigned = true;
//             }

//             // Assign nurse if shift demand allows
//             if (assigned) {
//                 add(schedule, shift - 1, nurse); // Add nurse to shift schedule
//                 totalSatisfaction += nurse.shiftPreferences[shift - 1]; // Update satisfaction score
//                 nurseCountForShift++;

//                 // Stop if all demands are met
//                 if (dm == 0 && de == 0 && dn == 0) break;
//             }
//         }
//     }
// }

// int main() {
//     //std::string nurseFile = "/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv";
//     //std::string constraintsFile = "/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseContraints.csv";
//     parseNursesCSV("/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");         // Populate departmentNursesMap with nurse data
//     parseConstraintsCSV("/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseContraints.csv"); // Populate constraintsMap with shift constraints

//     // Print out the constraints
//     for (const auto& shiftPair : constraintsMap) {
//         std::cout << "Shift: " << shiftPair.first << std::endl;
//         for (const auto& deptPair : shiftPair.second) {
//             std::cout << "  Department: " << deptPair.first << std::endl;
//             for (const auto& typePair : deptPair.second) {
//                 std::cout << typePair.first
//                           << ", Required: " << typePair.second << std::endl;
//             }
//         }
//     }

//     ShiftSchedule schedule(42); // Initialize schedule with 42 shifts
//     int totalSatisfaction = 0;

//     // Iterate over all departments in departmentNursesMap
//     for (const auto& departmentEntry : departmentNursesMap) {
//         const std::string& department = departmentEntry.first; // Department name
//         std::cout << "Scheduling for department: " << department << std::endl;

//         // Iterate over all nurse types in the current department
//         for (const auto& nurseTypeEntry : departmentEntry.second) {
//             const std::string& nurseType = nurseTypeEntry.first; // Nurse type
//             auto& nurses = nurseTypeEntry.second;                // List of nurses for this type

//             // Assign shifts for this nurse type in the current department
//             for (int shift = 1; shift <= schedule.size(); ++shift) {
//                 assignShifts(schedule, nurses, department, nurseType, shift, totalSatisfaction); // Assign shifts
//             }
//         }
//     }

//     // // Output the shift assignments
//     // for (int i = 0; i < schedule.size(); ++i) {
//     //     std::cout << "Shift " << (i + 1) << ": ";
//     //     for (const Nurse& nurse : schedule[i]) {
//     //         std::cout << nurse.fullName << ", ";
//     //     }
//     //     std::cout << std::endl;
//     // }

//     printShiftSchedule(schedule);

//     // Print the overall satisfaction score
//     // std::cout << "Overall Satisfaction Score: " << totalSatisfaction << std::endl;

//     // Convert the ShiftSchedule to JSON and save to a file
//     std::string filename = "shift_schedule.json";
//     shiftScheduleToJSON(schedule, filename);

//     return 0;
// }
