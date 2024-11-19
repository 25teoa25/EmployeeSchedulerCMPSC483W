#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>
#include "json.hpp"
#include <fstream>
#include <random> // For generating random numbers
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

/**
 * @brief Adds a nurse to a specific shift in the schedule.
 * 
 * @param schedule The ShiftSchedule vector where nurses are assigned to shifts.
 * @param shift The shift number (1-based index) where the nurse will be added.
 * @param nurse The Nurse object to be added to the specified shift.
 * 
 * This function appends the provided Nurse object to the shift specified in the schedule.
 * It validates that the shift number is within the valid range (1-42).
 */
void add(ShiftSchedule& schedule, int shift, const Nurse& nurse) {
    if (shift < 1 || shift > 42) {
        std::cerr << "Error: Shift number must be between 1 and 42.\n";
        return;
    }
    schedule[shift - 1].push_back(nurse);  // Add nurse to the appropriate shift
}

/**
 * @brief Removes a nurse from a specific shift in the schedule.
 * 
 * @param schedule The ShiftSchedule vector where nurses are assigned to shifts.
 * @param shift The shift number (1-based index) from which the nurse will be removed.
 * @param nurse The Nurse object to be removed from the specified shift.
 * 
 * This function removes the provided Nurse object from the shift specified in the schedule.
 * It validates that the shift number is within the valid range (1-42) and that the nurse exists.
 */
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

void printShiftSchedule(const ShiftSchedule& schedule) {
    for (size_t i = 0; i < schedule.size(); ++i) {
        const auto& shiftNurses = schedule[i];
        std::cout << "Shift " << i  << ":\n";
        
        if (shiftNurses.empty()) {
            std::cout << "  No nurses assigned.\n";
        } else {
            for (const auto& nurse : shiftNurses) {
                std::cout << "  Nurse ID: " << nurse.nurseNumber
                          << ", Name: " << nurse.fullName
                          << ", Dept: " << nurse.department
                            << ", Type: " << nurse.nurseType 
                            << ", Score:" <<nurse.shiftPreferences[i+1] <<'\n';
            }
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Converts the entire shift schedule to JSON format and writes it to a file.
 * 
 * @param schedule The ShiftSchedule vector containing nurses assigned to each shift.
 * @param filename The name of the output JSON file.
 * 
 * This function creates a JSON object representing the schedule of all shifts. Each shift
 * includes its shift number and the list of nurses assigned to it. If no nurses are assigned
 * to a shift, it records that information as well. The JSON is printed to the console and saved
 * to the specified file.
 */
void shiftScheduleToJSON(const ShiftSchedule& schedule, const std::string& filename) {
    nlohmann::json jsonOutput = nlohmann::json::array();  // Create a JSON array to hold all shifts

    // Iterate over the shifts
    for (size_t i = 0; i < schedule.size(); ++i) {
        const auto& shiftNurses = schedule[i];
        nlohmann::json shiftJson;  // Create a JSON object for each shift

        // Assign the shift number first
        shiftJson["1shift"] = i + 1;

        // Check if nurses are assigned to this shift
        if (shiftNurses.empty()) {
            shiftJson["nurses"] = "No nurses assigned";  // If no nurses are assigned
        } else {
            nlohmann::json nursesJson = nlohmann::json::array();  // Array to hold nurse data
            for (const auto& nurse : shiftNurses) {
                nlohmann::json nurseJson;
                nurseJson["nurseID"] = nurse.nurseNumber;
                nurseJson["name"] = nurse.fullName;
                nurseJson["department"] = nurse.department;
                nurseJson["nurseType"] = nurse.nurseType;
                nurseJson["score"] = nurse.shiftPreferences[i];
                nursesJson.push_back(nurseJson);  // Add each nurse's info to the array
            }
            shiftJson["nurses"] = nursesJson;  // Add the nurses array to the shift
        }

        jsonOutput.push_back(shiftJson);  // Add the shift's JSON to the output array
    }

    // Pretty print the entire JSON output to the console
    std::cout << jsonOutput.dump(4) << std::endl;

    // Open a file and write the JSON data to it
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << jsonOutput.dump(4);  // Write pretty-printed JSON to the file
        outFile.close();  // Close the file
        std::cout << "JSON data has been saved to " << filename << std::endl;
    } else {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
    }
}

/**
 * @brief Randomly selects a nurse from a specified shift and nurse type.
 * 
 * @param schedule The ShiftSchedule vector containing nurse objects for all shifts.
 * @param shift The shift number (1-based index).
 * @param nurseType The type of nurse to filter (e.g., "RN", "LPN").
 * @return Nurse A randomly selected nurse of the specified type from the given shift.
 *         Returns a fake nurse if no matching nurse is found.
 */
Nurse getRandomNurseFromShift(const ShiftSchedule& schedule, int shift, const std::string& nurseType) {
    if (shift < 1 || shift > schedule.size()) {
        std::cerr << "Error: Invalid shift number provided. Must be between 1 and " << schedule.size() << ".\n";
        return fakeNurse; // Return the global fakeNurse as a placeholder
    }

    // Get the nurses assigned to the given shift
    const auto& shiftNurses = schedule[shift - 1];

    // Filter nurses based on the provided nurse type
    std::vector<Nurse> matchingNurses;
    for (const auto& nurse : shiftNurses) {
        if (nurse.nurseType == nurseType) {
            matchingNurses.push_back(nurse);
        }
    }

    // Check if there are no matching nurses
    if (matchingNurses.empty()) {
        std::cerr << "No nurses of type " << nurseType << " found for shift " << shift << ".\n";
        return fakeNurse; // Return the fakeNurse as a placeholder
    }

    // Randomly select a nurse from the matching nurses
    std::random_device rd; // Seed for random number generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> dist(0, matchingNurses.size() - 1);

    int randomIndex = dist(gen);
    return matchingNurses[randomIndex];
}

/**
 * @brief Prints all nurses assigned to a specific shift in the schedule.
 * 
 * @param schedule The ShiftSchedule vector containing nurses for all shifts.
 * @param shift The 1-based index of the shift to display (e.g., 1 for the first shift).
 */
void printNursesForShift(const ShiftSchedule& schedule, int shift) {
    if (shift < 1 || shift > schedule.size()) {
        std::cerr << "Error: Shift index out of bounds. Valid range is 1 to " << schedule.size() << ".\n";
        return;
    }

    const auto& shiftNurses = schedule[shift - 1]; // Convert to 0-based index
    std::cout << "Nurses assigned to Shift " << shift << ":\n";

    if (shiftNurses.empty()) {
        std::cout << "  No nurses assigned.\n";
        return;
    }

    for (const auto& nurse : shiftNurses) {
        std::cout << "  Nurse ID: " << nurse.nurseNumber
                  << ", Name: " << nurse.fullName
                  << ", Department: " << nurse.department
                  << ", Type: " << nurse.nurseType << '\n';
    }
}