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

/**
 * @brief Randomly selects a nurse from a specified shift and nurse type.
 * 
 * @param schedule The ShiftSchedule vector containing nurse objects for all shifts.
 * @param shift The shift number (1-based index).
 * @param nurseType The type of nurse to filter (e.g., "RN", "LPN").
 * @return Nurse A randomly selected nurse of the specified type from the given shift.
 *         Returns a fake nurse if no matching nurse is found.
 */
Nurse getRandomNurseFromShift(const ShiftSchedule& schedule, int shift, const std::string& nurseType);

void printShiftSchedule(const ShiftSchedule& schedule);

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
void remove(ShiftSchedule& schedule, int shift, const Nurse& nurse);

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
void add(ShiftSchedule& schedule, int shift, const Nurse& nurse);

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
void shiftScheduleToJSON(const ShiftSchedule& schedule, const std::string& filename);
#endif // NURSE_FUNCTIONS_H

/**
 * @brief Prints all nurses assigned to a specific shift in the schedule.
 * 
 * @param schedule The ShiftSchedule vector containing nurses for all shifts.
 * @param shift The 1-based index of the shift to display (e.g., 1 for the first shift).
 */
void printNursesForShift(const ShiftSchedule& schedule, int shift);

/**
 * @brief Compares three satisfaction scores and outputs the highest.
 * 
 * @param scoreGeneticAlgorithm Satisfaction score for Genetic Algorithm.
 * @param scoreBruteForce Satisfaction score for Brute Force.
 * @param scoreLinearProgramming Satisfaction score for Linear Programming.
 * 
 * This function compares the three provided satisfaction scores and prints the method
 * with the highest score along with the score value.
 */
void returnBestSatisfactionScores(double scoreGeneticAlgorithm, double scoreBruteForce, double scoreLinearProgramming);