#ifndef NURSE_LIST_H
#define NURSE_LIST_H

#include <string>
#include <vector>
#include <algorithm>  // For std::remove
#include <iostream>
#include <unordered_map>

/**
 * @brief Structure representing a nurse's data.
 * 
 * This structure holds information about a nurse, including their full name, 
 * nurse number, type, department, and shift preferences.
 */
struct Nurse {
    std::string fullName;    ///< Full name of the nurse
    int nurseNumber;         ///< Unique identifier for the nurse
    std::string nurseType;   ///< Type of nurse (e.g., "RN", "LPN")
    std::string department;  ///< Department of the nurse (e.g., "Oncology")
    std::vector<int> shiftPreferences; ///< Stores preferences for 42 shifts (0, 1, 2)
    std::vector<int> scheduledShifts; ///< Stores what shifts a nurse is scheduled for

    // Equality operator to compare nurses by nurseNumber for removal
    bool operator==(const Nurse& other) const {
        return nurseNumber == other.nurseNumber;
    }

};

// Fake nurse for a shift unable to be scheduled
// Declare a global instance of Nurse
extern Nurse fakeNurse;
    
// Alias for the shift schedule: Vector of 42 vectors of Nurses
using ShiftSchedule = std::vector<std::vector<Nurse>>;

void add(ShiftSchedule& schedule, int shift, const Nurse& nurse);
void remove(ShiftSchedule& schedule, int shift, const Nurse& nurse);

// Global variables
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Nurse>>> departmentNursesMap;
extern std::unordered_map<int, std::unordered_map<std::string, std::unordered_map<std::string, int>>> constraintsMap;

#endif // NURSE_LIST_H
