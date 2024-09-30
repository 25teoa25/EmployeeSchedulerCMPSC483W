#ifndef NURSE_FUNCTIONS_H
#define NURSE_FUNCTIONS_H

#include <unordered_map>
#include <string>
#include <vector>
#include "NurseList.h"

/**
 * @brief Selects the best nurses for a specific shift and department based on their preferences and the constraints.
 * 
 * @param nurse_lists A reference to an unordered_map where the key is the nurse type (e.g., "Registered Nurse", "Licensed Practical Nurse", etc.) and the value is a NurseList containing nurses of that type.
 * @param constraints A vector of vectors representing the constraints CSV (department, shift number, Registered Nurse, Licensed Practical Nurse, Emergency Nursing).
 * @param shift_number The shift number (1-42).
 * @param department The department for which nurses are being selected.
 * 
 * @return A vector of pointers to the best-selected nurses for the given shift and department.
 */
std::vector<Nurse*> selectBestNurses(const std::unordered_map<std::string, NurseList>& nurse_lists,
                                     const std::vector<std::vector<std::string>>& constraints,
                                     int shift_number, const std::string& department);

#endif // NURSE_FUNCTIONS_H
