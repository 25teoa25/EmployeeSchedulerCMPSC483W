#ifndef NURSE_LIST_H
#define NURSE_LIST_H

#include <string>
#include <vector>
#include <unordered_map>

/**
 * @brief Structure representing a nurse's data.
 * 
 * This structure holds information about a nurse, including their full name, 
 * nurse number, type (e.g., "RN", "LPN"), department, and shift preferences.
 */
struct Nurse {
    std::string fullName;
    int nurseNumber;
    std::string nurseType;
    std::string department;
    std::vector<int> shiftPreferences; // Stores preferences for 42 shifts
    
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

/**
 * @brief Global map for storing nurses categorized by department and type.
 * 
 * This map uses the department name as the key, and each value is another map that categorizes 
 * nurses by their type (e.g., "RN", "LPN"). Each type map holds a vector of nurses.
 */
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Nurse>>> departmentNursesMap;

// No need to declare viewNursesByDepartmentAndType here; it's in NurseFunctions.h

#endif // NURSE_LIST_H
