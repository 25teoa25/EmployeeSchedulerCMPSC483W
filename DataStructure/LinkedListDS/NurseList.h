#ifndef NURSE_LIST_H
#define NURSE_LIST_H

#include <string>
#include <vector>
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

    /**
     * @brief Gets the shift preference for a specific shift.
     * 
     * @param shiftNumber The shift number (1-42).
     * @return int The preference for the specified shift (0, 1, 2).
     */
    int getShiftPreference(int shiftNumber) const {
        if (shiftNumber >= 1 && shiftNumber <= shiftPreferences.size()) {
            return shiftPreferences[shiftNumber - 1];
        }
        return -1; // Invalid shift number
    }
};

// Global variables
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Nurse>>> departmentNursesMap;
extern std::unordered_map<int, std::unordered_map<std::string, std::unordered_map<std::string, int>>> constraintsMap;

#endif // NURSE_LIST_H
