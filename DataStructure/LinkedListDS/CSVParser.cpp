#include "CSVParser.h"
#include "NurseList.h"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Reads a CSV file and populates nurse data into the departmentNursesMap.
 * 
 * @param fileName The name of the CSV file to read.
 * 
 * This function processes the CSV file, where each line represents a nurse and their attributes. 
 * It parses the nurse's name, number, type, department, and shift preferences. 
 * The nurse data is stored in a global map categorized by department and type.
 */
void parseNursesCSV(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string line;
    
    // Skip the header line
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        Nurse nurse;
        int shiftCount = 0;

        // Extract data from the CSV line
        std::getline(ss, nurse.fullName, ',');
        std::getline(ss, token, ',');
        nurse.nurseNumber = std::stoi(token);
        std::getline(ss, nurse.nurseType, ',');
        std::getline(ss, nurse.department, ',');

        // Store shift preferences (42 shifts)
        while (std::getline(ss, token, ',') && shiftCount < 42) {
            nurse.shiftPreferences.push_back(std::stoi(token));
            shiftCount++;
        }

        // Store nurse in the map by department and nurse type
        departmentNursesMap[nurse.department][nurse.nurseType].push_back(nurse);
    }
    
    file.close();
}
