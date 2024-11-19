// to run this file right now:
// make sure to change the path of the constraints, nurse list files, and any files in the compilation statement
// g++ -std=c++11 main.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/Algorithm/neighbor.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/CSVParser.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseList.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseFunctions.cpp -o Testfile
// then: ./Testfile

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
#include "neighbor.h"
// #include "json.hpp"
// using json = nlohmann::json;
using namespace std;

// Global satisfaction score variable
int satisfactionScore = 0;

int numberAssigned = 0;

// Function to randomly generate a number from 1 to 3 (morning, evening, night)
int generateRandomShift()
{
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 3); // Random number between 1 and 3
    return dist(gen);
}

// Helper function to assign nurses to shifts based on their availability not based on preference
void simpleAssignment(vector<vector<Nurse>> &shiftSchedule,
                      const string &department, const string &nurseType,
                      int shift, int &demandRemaining)
{

    // The amount of nurses examined in the vector
    int nurseListCounter = 0;

    // Iterate through the nurses for the given department and nurse type
    for (auto &nurse : departmentNursesMap[department][nurseType])
    {
        // Check if the nurse is not yet assigned to this shift
        if (nurse.scheduledShifts[shift] == 0)
        {
            // Check if the nurse is assigned to more than 2 consecutive shifts
            bool canAssign = true;

            // Check previous shift (if shift > 0)
            if (shift > 0 && nurse.scheduledShifts[shift - 1] == 1 &&
                nurse.scheduledShifts[shift - 2] == 1)
            {
                canAssign = false; // Nurse already worked two consecutive shifts
            }

            // Check next shift (if shift < 41, since we have 42 shifts)
            if (shift < 41 && nurse.scheduledShifts[shift + 1] == 1 &&
                nurse.scheduledShifts[shift + 2] == 1)
            {
                canAssign = false; // Nurse would work two consecutive shifts
            }

            if (canAssign)
            {
                // Assign the nurse to the shift
                add(shiftSchedule, shift, nurse); // Add nurse to shift schedule

                // Mark the nurse as assigned for this shift
                nurse.scheduledShifts[shift] = 1;

                // Decrease demand for the shift by 1
                demandRemaining--;

                // Update the satisfaction score based on the nurse's preference
                satisfactionScore += nurse.shiftPreferences[shift];

                numberAssigned++;

                // If the demand has been fulfilled, exit loop
                if (demandRemaining <= 0)
                {
                    break;
                }
            }
        }
        nurseListCounter++;
        if (nurseListCounter > departmentNursesMap[department][nurseType].size())
        {
            break;
        }
    }
}

// Helper function to assign nurses based on their shift preferences optimized for preference
void optimizePreferenceAssignment(vector<vector<Nurse>> &shiftSchedule,
                                  const string &department, const string &nurseType,
                                  int shift, int &demandRemaining)
{

    bool assignedAny = false;

    // Loop through the nurses for the given department and nurse type
    for (auto &nurse : departmentNursesMap[department][nurseType])
    {
        // Check if nurse is not already assigned to this shift and prefers this shift (preference = 2)
        if (nurse.scheduledShifts[shift] == 0 && nurse.shiftPreferences[shift] == 2)
        {
            // Check if the nurse is assigned to more than 2 consecutive shifts
            bool canAssign = true;

            // Check previous shift (if shift > 0)
            if (shift > 0 && nurse.scheduledShifts[shift - 1] == 1 &&
                nurse.scheduledShifts[shift - 2] == 1)
            {
                canAssign = false; // Nurse already worked two consecutive shifts
            }

            // Check next shift (if shift < 41, since we have 42 shifts)
            if (shift < 41 && nurse.scheduledShifts[shift + 1] == 1 &&
                nurse.scheduledShifts[shift + 2] == 1)
            {
                canAssign = false; // Nurse would work two consecutive shifts
            }

            if (canAssign)
            {
                // Assign the nurse to the shift
                add(shiftSchedule, shift, nurse); // Add nurse to the shift schedule

                // Mark the nurse as assigned for this shift
                nurse.scheduledShifts[shift] = 1;

                // Decrease the demand for the shift by 1
                demandRemaining--;

                // Update the satisfaction score based on the nurse's preference
                satisfactionScore += nurse.shiftPreferences[shift];

                // We have assigned at least one nurse
                assignedAny = true;

                // If the demand has been fulfilled, exit loop
                if (demandRemaining <= 0)
                {
                    break;
                }
            }
        }
    }

    // If demand is still not met, call simpleAssignment to fulfill remaining demand
    if (demandRemaining > 0)
    {
        simpleAssignment(shiftSchedule, department, nurseType, shift, demandRemaining);
    }
}

int main()
{
    // Parse the nurse CSV file to populate the departmentNursesMap
    parseNursesCSV("Y:/git/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");

    // Parse the constraints CSV file to populate constraintsMap
    parseConstraintsCSV("Y:/git/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/NurseContraints.csv");

    // Constraint map structure: <Shift # : <Department name : <Nurse Type : # Needed>>>
    map<int, unordered_map<string, unordered_map<string, int>>> sortedConstraintsMap(
        constraintsMap.begin(), constraintsMap.end());

    // Print constraints map
    // for (const auto& shiftPair : sortedConstraintsMap) {
    //     cout << "Shift: " << shiftPair.first << endl;
    //     for (const auto& deptPair : shiftPair.second) {
    //         cout << "  Department: " << deptPair.first << endl;
    //         for (const auto& typePair : deptPair.second) {
    //             cout << typePair.first
    //                       << ", Required: " << typePair.second << endl;
    //         }
    //     }
    // }

    ShiftSchedule shiftSchedule(42); // Final shift schedule to be outputted

    // Iterate over all departments in departmentNursesMap
    for (const auto &departmentEntry : departmentNursesMap)
    {
        const string &department = departmentEntry.first; // Department name
        cout << "Scheduling for department: " << department << endl;

        // Iterate over each nurse type in the current department
        for (const auto &nurseTypeEntry : departmentEntry.second)
        {
            const string &nurseType = nurseTypeEntry.first; // Nurse type (RN, LPN, etc.)
            auto &nurses = nurseTypeEntry.second;           // List of nurses for this type

            // Iterate over 14 days
            for (int day = 1; day <= 14; ++day)
            {
                // Shifts in focus
                int morningShiftNumber = (day - 1) * 3 + 1;
                int eveningShiftNumber = (day - 1) * 3 + 2;
                int nightShiftNumber = (day - 1) * 3 + 3;

                string nurseTypeFormatted;
                if (nurseType == "NA")
                {
                    nurseTypeFormatted = "Nurse Type: Nurse Assistant";
                }
                else if (nurseType == "RN")
                {
                    nurseTypeFormatted = "Nurse Type: Registered Nurse";
                }
                else if (nurseType == "LPN")
                {
                    nurseTypeFormatted = "Nurse Type: Licensed Practical Nurse";
                }

                // Assign the demand for each shift (morning, evening, night) for this day
                int dm = sortedConstraintsMap[morningShiftNumber][department][nurseTypeFormatted]; // Demand for morning shift
                int de = sortedConstraintsMap[eveningShiftNumber][department][nurseTypeFormatted]; // Demand for evening shift
                int dn = sortedConstraintsMap[nightShiftNumber][department][nurseTypeFormatted];   // Demand for night shift

                // Generate a random number to decide which shift to focus on (1 = morning, 2 = evening, 3 = night)
                int randomShift = generateRandomShift();

                if (randomShift == 1)
                { // Optimize preferences for random morning shift
                    optimizePreferenceAssignment(shiftSchedule, department, nurseType, morningShiftNumber, dm);
                    simpleAssignment(shiftSchedule, department, nurseType, eveningShiftNumber, de); // Assign nurses to evening shift without optimizing preference
                    simpleAssignment(shiftSchedule, department, nurseType, nightShiftNumber, dn);   // Assign nurses to night shift without optimizing preference
                }
                else if (randomShift == 2)
                { // Optimize preferences for random evening shift
                    optimizePreferenceAssignment(shiftSchedule, department, nurseType, eveningShiftNumber, de);
                    simpleAssignment(shiftSchedule, department, nurseType, morningShiftNumber, dm); // Assign nurses to morning shift without optimizing preference
                    simpleAssignment(shiftSchedule, department, nurseType, nightShiftNumber, dn);   // Assign nurses to night shift without optimizing preference
                }
                else
                { // Optimize preferences for random night shift
                    optimizePreferenceAssignment(shiftSchedule, department, nurseType, nightShiftNumber, dn);
                    simpleAssignment(shiftSchedule, department, nurseType, morningShiftNumber, dm); // Assign nurses to morning shift without optimizing preference
                    simpleAssignment(shiftSchedule, department, nurseType, eveningShiftNumber, de); // Assign nurses to night shift without optimizing preference
                }
            }

            // cout << "Satisfaction before: " << satisfactionScore << endl;
            satisfactionScore = structure1(shiftSchedule, department, satisfactionScore, nurseType);
            // cout << "Satisfaction after: " << satisfactionScore << endl;
            satisfactionScore = structure2(shiftSchedule, department, satisfactionScore, nurseType);
            satisfactionScore = structure3(shiftSchedule, department, satisfactionScore, nurseType);
            satisfactionScore = structure4(shiftSchedule, department, satisfactionScore, nurseType);
            satisfactionScore = structure5(shiftSchedule, department, satisfactionScore, nurseType);
            satisfactionScore = structure6(shiftSchedule, department, satisfactionScore, nurseType);
            satisfactionScore = structure7(shiftSchedule, department, satisfactionScore, nurseType);
            satisfactionScore = structure8(shiftSchedule, department, satisfactionScore, nurseType);
        }
    }

    // printShiftSchedule(shiftSchedule);
    // string filename = "shift_schedule.json";
    // shiftScheduleToJSON(shiftSchedule, filename);
    cout << "Satisfaction score of current schedule: " << satisfactionScore << endl;
    cout << "Number assigned: " << numberAssigned * 2 << endl;

    return 0;
}
