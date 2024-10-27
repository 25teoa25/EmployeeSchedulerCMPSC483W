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
//#include "../Algorithm/neighbor.h"

using namespace std;

//to run this file right now:
//g++ -std=c++11 main.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/CSVParser.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseList.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseFunctions.cpp -o Testfile
//then: ./Testfile


// Global map that contains all nurse information for all departments (moved from NurseList.cpp):
// unordered_map<string, unordered_map<string, vector<Nurse>>> departmentNursesMap
// Outer key is department, inner map key is nurse type, inner map value is a vector of Nurse objects
// Example: {Pediatric: {LPN: <N1,N2,N3>, NA: <N1>, RN: <N1,N2>}, Oncology: {LPN: <N1,N2>, NA: <N1,N2>, RN: <N1>}}


// Function to randomly generate a shift preference
int generateRandomShift() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 3);
    return dist(gen);
}

// Function to check if a nurse can be assigned based on constraints
bool canAssignNurse(int shift, const std::string& department, const std::string& nurseType, int currentCount) {
    if (constraintsMap.count(shift) && constraintsMap[shift].count(department) &&
        constraintsMap[shift][department].count(nurseType)) {
        
        int requiredCount = constraintsMap[shift][department][nurseType];
        return currentCount < requiredCount;
    }
    return true; // Allow assignment if no constraint exists
}

// Function to implement the shift specification algorithm with cumulative satisfaction scoring
void assignShifts(ShiftSchedule& schedule, std::vector<Nurse>& nurses, const std::string& department, 
                  const std::string& nurseType, int shift, int& totalSatisfaction) {
    int dm = 0, de = 0, dn = 0;
    int d1 = 2, d2 = 2, d3 = 1; // Example demand values for morning, evening, night

    dm = d1;
    de = d2;
    dn = d3;

    int nurseCountForShift = 0;

    for (Nurse& nurse : nurses) {
        if (nurse.department == department && shift <= nurse.shiftPreferences.size() &&
            (nurse.shiftPreferences[shift - 1] == 1 || nurse.shiftPreferences[shift - 1] == 2) &&
            canAssignNurse(shift, department, nurseType, nurseCountForShift)) {
            
            int r = generateRandomShift();
            bool assigned = false;

            if (r == 1 && dm > 0) {
                dm--; assigned = true;
            } else if (r == 2 && de > 0) {
                de--; assigned = true;
            } else if (r == 3 && dn > 0) {
                dn--; assigned = true;
            }

            // Assign nurse if demands allow it
            if (assigned) {
                add(schedule, shift - 1, nurse);
                totalSatisfaction += nurse.shiftPreferences[shift - 1];
                nurseCountForShift++;

                // Stop if demand is met
                if (dm == 0 && de == 0 && dn == 0) break;
            }
        }
    }
}

int main() {
    std::string nurseFile = "/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv";
    std::string constraintsFile = "/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse Constraints Test.csv";
    parseNursesCSV(nurseFile);         // Populate departmentNursesMap with nurse data
    parseConstraintsCSV(constraintsFile); // Populate constraintsMap with shift constraints

    ShiftSchedule schedule(42);
    int totalSatisfaction = 0;
    std::string department = "Oncology";

    // Iterate over all nurse types within the specified department
    for (auto& nurseTypeEntry : departmentNursesMap[department]) {
        const std::string& nurseType = nurseTypeEntry.first; // Nurse type key
        auto& nurses = nurseTypeEntry.second;                // Vector of nurses for this type

        for (int shift = 1; shift <= schedule.size(); ++shift) {
            assignShifts(schedule, nurses, department, nurseType, shift, totalSatisfaction);
        }
    }

    for (int i = 0; i < schedule.size(); ++i) {
        std::cout << "Shift " << (i + 1) << ": ";
        for (const Nurse& nurse : schedule[i]) {
            std::cout << nurse.fullName << ", ";
        }
        std::cout << std::endl;
    }

    std::cout << "Overall Satisfaction Score: " << totalSatisfaction << std::endl;
    return 0;
}