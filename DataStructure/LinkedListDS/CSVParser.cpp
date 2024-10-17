#include "CSVParser.h"
#include "NurseList.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

// Helper function to trim leading and trailing whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return ""; // Empty or whitespace-only string
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// Function to parse nurse data from the CSV file
void parseNursesCSV(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        std::istringstream ss(line);
        Nurse nurse;
        std::string token;
        int shiftCount = 0;

        try {
            std::getline(ss, nurse.fullName, ',');
            std::getline(ss, token, ',');
            nurse.nurseNumber = std::stoi(token); // Convert nurse number
            std::getline(ss, nurse.nurseType, ',');
            std::getline(ss, nurse.department, ',');

            while (std::getline(ss, token, ',') && shiftCount < 42) {
                nurse.shiftPreferences.push_back(std::stoi(token)); // Convert shift preferences
                shiftCount++;
            }

            departmentNursesMap[nurse.department][nurse.nurseType].push_back(nurse);

        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid data: " << line << " - Skipping this entry." << std::endl;
        }
    }

    file.close();
}

/**
 * @brief Reads a CSV of department constraints and stores the data in the global constraintsMap.
 * 
 * @param fileName The name of the CSV file to read.
 * 
 * This function processes the CSV, where each row specifies the required nurses for a given 
 * shift, department, and nurse type.
 */
void parseConstraintsCSV(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header

    int lineNumber = 1; // Track line number for better error reporting

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue; // Skip empty lines

        std::istringstream ss(line);
        std::string shiftStr, department, nurseType, requiredStr;

        try {
            // Read and trim each field
            std::getline(ss, shiftStr, ',');
            std::getline(ss, department, ',');
            std::getline(ss, nurseType, ',');
            std::getline(ss, requiredStr, ',');

            shiftStr = trim(shiftStr);
            department = trim(department);
            nurseType = trim(nurseType);
            requiredStr = trim(requiredStr);

            // Ensure fields are not empty
            if (shiftStr.empty() || department.empty() || nurseType.empty() || requiredStr.empty()) {
                std::cerr << "Skipping malformed entry on line " << lineNumber << ": " << line << std::endl;
                continue;
            }

            // Convert fields to appropriate data types
            int shift = std::stoi(shiftStr);
            int required = std::stoi(requiredStr);

            // Insert into the constraints map
            constraintsMap[shift][department][nurseType] = required;

        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid data on line " << lineNumber << ": " << line
                      << " - " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Constraints successfully parsed from " << fileName << std::endl;
}
