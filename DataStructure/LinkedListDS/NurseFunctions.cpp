#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>

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
