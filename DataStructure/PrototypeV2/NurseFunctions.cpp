#include "NurseFunctions.h"
#include <algorithm>
#include <iostream>
using namespace std;

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
vector<Nurse*> selectBestNurses(const unordered_map<string, NurseList>& nurse_lists,
                                     const vector<vector<string>>& constraints,
                                     int shift_number, const string& department) {

    // Variables to store required nurses for the shift
    int required_rn = 0, required_lpn = 0, required_emergency_nurses = 0;
    bool department_found = false;

    // Search for the department and shift constraints in the CSV data
    for (const auto& row : constraints) {
        if (row[0] == department && stoi(row[1]) == shift_number) {
            required_rn = stoi(row[2]);   // Number of Registered Nurses required
            required_lpn = stoi(row[3]);  // Number of Licensed Practical Nurses required
            required_emergency_nurses = stoi(row[4]);   // Number of Emergency Nurses required
            department_found = true;
            break;
        }
    }

    if (!department_found) {
        cerr << "No constraints found for department " << department << " and shift " << shift_number << endl;
        return {};
    }

    vector<Nurse*> selected_nurses;

    // Helper function to filter nurses by preference for the shift
    auto filterNursesByPreference = [shift_number](const NurseList& nurse_list, int required_count) -> vector<Nurse*> {
        vector<Nurse*> type_selected_nurses;

        Nurse* current = nurse_list.getHead();  // Use the getter method here

        while (current != nullptr) {
            if (current->nurseShifts[shift_number - 1] == "2") {
                type_selected_nurses.push_back(current);  // Nurse prefers this shift
            } else if (current->nurseShifts[shift_number - 1] == "1") {
                type_selected_nurses.push_back(current);  // Nurse can work but doesn't prefer it
            }
            current = current->next;
        }

        // Sort nurses based on preference (preferred nurses first)
        sort(type_selected_nurses.begin(), type_selected_nurses.end(), 
            [shift_number](Nurse* a, Nurse* b) {
                return a->nurseShifts[shift_number - 1] > b->nurseShifts[shift_number - 1];
            });

        // Return only the required number of nurses
        if (type_selected_nurses.size() > required_count) {
            type_selected_nurses.resize(required_count);
        }

        return type_selected_nurses;
    };

    // Select the required Registered Nurses based on preference
    if (nurse_lists.find("Registered Nurse") != nurse_lists.end()) {
        vector<Nurse*> rn_nurses = filterNursesByPreference(nurse_lists.at("Registered Nurse"), required_rn);
        selected_nurses.insert(selected_nurses.end(), rn_nurses.begin(), rn_nurses.end());
    }

    // Select the required Licensed Practical Nurses based on preference
    if (nurse_lists.find("Licensed Practical Nurse") != nurse_lists.end()) {
        vector<Nurse*> lpn_nurses = filterNursesByPreference(nurse_lists.at("Licensed Practical Nurse"), required_lpn);
        selected_nurses.insert(selected_nurses.end(), lpn_nurses.begin(), lpn_nurses.end());
    }

    // Select the required Emergency Nurses based on preference
    if (nurse_lists.find("Emergency Nursing") != nurse_lists.end()) {
        vector<Nurse*> emergency_nurses = filterNursesByPreference(nurse_lists.at("Emergency Nursing"), required_emergency_nurses);
        selected_nurses.insert(selected_nurses.end(), emergency_nurses.begin(), emergency_nurses.end());
    }

    return selected_nurses;
}

/**
 * @brief Calculates the average satisfaction score of nurses assigned to a specific shift.
 * 
 * @param assigned_nurses A vector of pointers to Nurse objects representing the nurses assigned to the shift.
 * @param shift The shift number (1-based) for which the average satisfaction score is to be calculated.
 *               Note: The `shift` parameter is 1-based, while the `nurseShifts` vector is 0-based.
 * 
 * @return The average satisfaction score for the assigned nurses for the specified shift.
 *         Returns 0.0 if no nurses are assigned to avoid division by zero.
 */
double calculateAverageSatisfaction(const std::vector<Nurse*>& assigned_nurses, int shift) {
    int total_satisfaction = 0;

    // Sum the satisfaction scores for each nurse for the given shift
    for (const Nurse* nurse : assigned_nurses) {
        total_satisfaction += std::stoi(nurse->nurseShifts[shift - 1]);  // nurseShifts is 0-based, shift is 1-based
    }

    // Return the average satisfaction score
    return assigned_nurses.empty() ? 0.0 : static_cast<double>(total_satisfaction) / assigned_nurses.size();
}
