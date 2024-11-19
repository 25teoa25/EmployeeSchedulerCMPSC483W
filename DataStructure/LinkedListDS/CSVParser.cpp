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
 * @brief Parses a CSV file with dynamic nurse types and populates the constraintsMap.
 * 
 * @param fileName The name of the CSV file to parse.
 */
void parseConstraintsCSV(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << fileName << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> nurseTypes;

    // Read the header row to extract dynamic nurse types
    std::getline(file, line);
    std::stringstream headerStream(line);
    std::string column;

    // Skip "Department" and "Shift (1 - 42)" columns, extract nurse types dynamically
    std::getline(headerStream, column, ','); // Skip "Department"
    std::getline(headerStream, column, ','); // Skip "Shift (1 - 42)"
    while (std::getline(headerStream, column, ',')) {
        nurseTypes.push_back(column);
    }

    // Parse the remaining rows to populate the constraints map
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string department;
        int shift, nurseCount;

        // Extract the department and shift number
        std::getline(ss, department, ',');
        ss >> shift;
        ss.ignore(); // Ignore the comma

        // Extract nurse counts for each dynamic nurse type
        for (const auto& nurseType : nurseTypes) {
            ss >> nurseCount;
            ss.ignore(); // Ignore the comma

            // Insert the value into the constraintsMap
            constraintsMap[shift][department][nurseType] = nurseCount;
        }
    }

    file.close();
    std::cout << "CSV parsing completed, constraintsMap populated." << std::endl;
}
