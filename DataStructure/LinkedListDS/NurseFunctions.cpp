#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>
#include "json.hpp"
#include <fstream>
using json = nlohmann::json;

/**
 * @brief Displays a list of nurses filtered by department and type.
 * 
 * @param department The department of the nurses to be displayed (e.g., "Oncology").
 * @param type The type of nurses to be displayed (e.g., "RN", "LPN").
 * 
 * This function iterates through the global departmentNursesMap and outputs all nurses 
 * who belong to the specified department and type. It prints their nurse number and name.
 */
void viewNursesByDepartmentAndType(const std::string& department, const std::string& type) {
    if (departmentNursesMap.find(department) != departmentNursesMap.end()) {
        const auto& typeMap = departmentNursesMap[department];
        if (typeMap.find(type) != typeMap.end()) {
            const auto& nurses = typeMap.at(type);
            std::cout << "Nurses in " << department << " department, type: " << type << std::endl;
            for (const auto& nurse : nurses) {
                std::cout << "Nurse Number: " << nurse.nurseNumber 
                          << ", Name: " << nurse.fullName << std::endl;
            }
        } else {
            std::cout << "No nurses of type " << type << " in department " << department << std::endl;
        }
    } else {
        std::cout << "No such department: " << department << std::endl;
    }
}

void viewNursesByDepartmentAndTypeJSON(const std::string& department, const std::string& type) {
    if (departmentNursesMap.find(department) != departmentNursesMap.end()) {
        const auto& typeMap = departmentNursesMap[department];
        if (typeMap.find(type) != typeMap.end()) {
            const auto& nurses = typeMap.at(type);
            json result;
            result["department"] = department;
            result["type"] = type;
            
            json nurseArray = json::array();
            for (const auto& nurse : nurses) {
                json nurseJson;
                nurseJson["nurseNumber"] = nurse.nurseNumber;
                nurseJson["fullName"] = nurse.fullName;
                nurseArray.push_back(nurseJson);
            }
            
            result["nurses"] = nurseArray;
            std::cout << result.dump(4) << std::endl; // Pretty print with 4-space indentation
        } else {
            std::cout << json{{"error", "No nurses of type " + type + " in department " + department}}.dump(4) << std::endl;
        }
    } else {
        std::cout << json{{"error", "No such department: " + department}}.dump(4) << std::endl;
    }
}

/**
 * @brief Converts a Nurse object to JSON and writes it to a file.
 * 
 * @param nurse The Nurse object to be converted.
 * @param filename The name of the output JSON file.
 */
void nurseToJSON(const Nurse& nurse, const std::string& filename) {
    // Create a JSON object
    json nurseJson = {
        {"fullName", nurse.fullName},
        {"nurseNumber", nurse.nurseNumber},
        {"nurseType", nurse.nurseType},
        {"department", nurse.department},
        {"shiftPreferences", nurse.shiftPreferences}
    };

    // Write the JSON object to a file
    std::ofstream file(filename);
    if (file.is_open()) {
        file << nurseJson.dump(4); // Pretty print with 4 spaces indentation
        file.close();
        std::cout << "Nurse data saved to " << filename << std::endl;
    } else {
        std::cerr << "Failed to open the file: " << filename << std::endl;
    }
}

// Add a nurse to a specific shift
void add(ShiftSchedule& schedule, int shift, const Nurse& nurse) {
    if (shift < 1 || shift > 42) {
        std::cerr << "Error: Shift number must be between 1 and 42.\n";
        return;
    }
    schedule[shift - 1].push_back(nurse);  // Add nurse to the appropriate shift
}

// Remove a nurse from a specific shift
void remove(ShiftSchedule& schedule, int shift, const Nurse& nurse) {
    if (shift < 1 || shift > 42) {
        std::cerr << "Error: Shift number must be between 1 and 42.\n";
        return;
    }
    auto& shiftNurses = schedule[shift - 1];  // Get the nurses for the shift

    // Use std::remove to find and remove the nurse by nurseNumber
    auto it = std::remove(shiftNurses.begin(), shiftNurses.end(), nurse);
    if (it != shiftNurses.end()) {
        shiftNurses.erase(it, shiftNurses.end());
    } else {
        std::cerr << "Error: Nurse with ID " << nurse.nurseNumber 
                  << " not found in shift " << shift << ".\n";
    }
}
