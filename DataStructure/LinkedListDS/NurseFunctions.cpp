#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>
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


