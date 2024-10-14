#ifndef NURSE_FUNCTIONS_H
#define NURSE_FUNCTIONS_H

#include <string>

/**
 * @brief Displays a list of nurses filtered by department and type.
 * 
 * @param department The department of the nurses to be displayed (e.g., "Oncology").
 * @param type The type of nurses to be displayed (e.g., "RN", "LPN").
 * 
 * This function outputs the list of nurses in the specified department and of the specified type.
 */
void viewNursesByDepartmentAndType(const std::string& department, const std::string& type);

#endif // NURSE_FUNCTIONS_H
