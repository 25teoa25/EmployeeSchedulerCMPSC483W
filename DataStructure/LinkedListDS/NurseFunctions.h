#ifndef NURSE_FUNCTIONS_H
#define NURSE_FUNCTIONS_H

#include <string>
#include "json.hpp"
#include "NurseList.h"
#include <fstream>
using json = nlohmann::json;

/**
 * @brief Displays a list of nurses filtered by department and type.
 * 
 * @param department The department of the nurses to be displayed (e.g., "Oncology").
 * @param type The type of nurses to be displayed (e.g., "RN", "LPN").
 * 
 * This function outputs the list of nurses in the specified department and of the specified type.
 */
void viewNursesByDepartmentAndType(const std::string& department, const std::string& type);

void viewNursesByDepartmentAndTypeJSON(const std::string& department, const std::string& type);

using ShiftSchedule = std::vector<std::vector<Nurse>>;

void printShiftSchedule(const ShiftSchedule& schedule);

void remove(ShiftSchedule& schedule, int shift, const Nurse& nurse);

void add(ShiftSchedule& schedule, int shift, const Nurse& nurse);

void shiftScheduleToJSON(const ShiftSchedule& schedule, const std::string& filename);
#endif // NURSE_FUNCTIONS_H
