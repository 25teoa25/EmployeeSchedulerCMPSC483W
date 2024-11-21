#include <iostream>
#include "LinearPgHelper.h"
#include "NurseList.h"

using namespace std;

// Function to randomly generate a number from 1 to 3 (morning, evening, night)
int generateRandomShift() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 3); // Random number between 1 and 3
    return dist(gen);
}

// Helper function to assign nurses to shifts based on their availability not based on preference
void simpleAssignment(vector<vector<Nurse>> &shiftSchedule,
                      const string &department, const string &nurseType,
                      int shift, int &demandRemaining) {

    // Iterate through the nurses for the given department and nurse type
    for (auto &nurse : departmentNursesMap[department][nurseType]) {
        // Check if the nurse is not yet assigned to this shift
        if (nurse.scheduledShifts[shift] == 0) {
            // Check if the nurse is assigned to more than 2 consecutive shifts
            bool canAssign = true;

            // Check previous shift (if shift > 0)
            if (shift > 0 && nurse.scheduledShifts[shift - 1] == 1 && nurse.scheduledShifts[shift - 2] == 1) {
                canAssign = false; // Nurse already worked two consecutive shifts
            }

            // Check next shift (if shift < 41, since we have 42 shifts)
            if (shift < 41 && nurse.scheduledShifts[shift + 1] == 1 && nurse.scheduledShifts[shift + 2] == 1) {
                canAssign = false; // Nurse would work two consecutive shifts
            }

            if (canAssign) {
                // Assign the nurse to the shift
                if (shift == 0) {
                    add(shiftSchedule, 1, nurse); // Add nurse to shift schedule
                } else if (shift == 41) {
                    add(shiftSchedule, 42, nurse); // Add nurse to shift schedule
                } else {
                    add(shiftSchedule, shift, nurse); // Add nurse to shift schedule
                }

                // Mark the nurse as assigned for this shift
                nurse.scheduledShifts[shift] = 1;

                // Decrease demand for the shift by 1
                demandRemaining--;

                // Update the satisfaction score based on the nurse's preference
                satisfactionScoreLinearProgramming += nurse.shiftPreferences[shift];

                // If the demand has been fulfilled, exit loop
                if (demandRemaining <= 0) {
                    break;
                }
            }
        }
    }
}

// Helper function to assign nurses based on their shift preferences optimized for preference
void optimizePreferenceAssignment(vector<vector<Nurse>> &shiftSchedule,
                                  const string &department, const string &nurseType,
                                  int shift, int &demandRemaining) {

    bool assignedAny = false;

    // Loop through the nurses for the given department and nurse type
    for (auto &nurse : departmentNursesMap[department][nurseType]) {
        // Check if nurse is not already assigned to this shift and prefers this shift (preference = 2)
        if (nurse.scheduledShifts[shift] == 0 && nurse.shiftPreferences[shift] == 2) {
            // Check if the nurse is assigned to more than 2 consecutive shifts
            bool canAssign = true;

            // Check previous shift (if shift > 0)
            if (shift > 0 && nurse.scheduledShifts[shift - 1] == 1 && nurse.scheduledShifts[shift - 2] == 1) {
                canAssign = false; // Nurse already worked two consecutive shifts
            }

            // Check next shift (if shift < 41, since we have 42 shifts)
            if (shift < 41 && nurse.scheduledShifts[shift + 1] == 1 && nurse.scheduledShifts[shift + 2] == 1) {
                canAssign = false; // Nurse would work two consecutive shifts
            }

            if (canAssign) {
                // Assign the nurse to the shift
                if (shift == 0) {
                    add(shiftSchedule, 1, nurse); // Add nurse to shift schedule
                } else if (shift == 41) {
                    add(shiftSchedule, 42, nurse); // Add nurse to shift schedule
                } else {
                    add(shiftSchedule, shift, nurse); // Add nurse to shift schedule
                }

                // Mark the nurse as assigned for this shift
                nurse.scheduledShifts[shift] = 1;

                // Decrease the demand for the shift by 1
                demandRemaining--;

                // Update the satisfaction score based on the nurse's preference
                satisfactionScoreLinearProgramming += nurse.shiftPreferences[shift];

                // We have assigned at least one nurse
                assignedAny = true;

                // If the demand has been fulfilled, exit loop
                if (demandRemaining <= 0) {
                    break;
                }
            }
        }
    }

    // If demand is still not met, call simpleAssignment to fulfill remaining demand
    if (demandRemaining > 0) {
        simpleAssignment(shiftSchedule, department, nurseType, shift, demandRemaining);
    }
}