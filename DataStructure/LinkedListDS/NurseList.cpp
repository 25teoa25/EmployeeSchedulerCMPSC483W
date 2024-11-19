#include "NurseList.h"
#include <iostream>

/**
 * @brief Global map for storing nurses categorized by department and type.
 * 
 * The map uses the department name as the key, and each value is another map that categorizes nurses 
 * by their type (e.g., "RN", "LPN"). Each nurse type map holds a vector of nurses.
 */
std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Nurse>>> departmentNursesMap;

std::unordered_map<int, std::unordered_map<std::string, std::unordered_map<std::string, int>>> constraintsMap;

Nurse fakeNurse = {
    "Fake Nurse",        // fullName
    -1,               // nurseNumber
    "RN",              // nurseType
    "Oncology",        // department
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {}
};

