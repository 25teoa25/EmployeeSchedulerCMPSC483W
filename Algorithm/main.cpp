#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include "../DataStructure/LinkedListDS/NurseFunctions.h"
#include "../DataStructure/LinkedListDS/CSVParser.h"
#include "../DataStructure/LinkedListDS/NurseList.h"

using namespace std;

// Define shift types
enum ShiftType { MORNING = 'M', EVENING = 'E', NIGHT = 'N' };

// Global map declaration (moved from NurseList.cpp)
extern unordered_map<string, unordered_map<string, vector<Nurse>>> departmentNursesMap;

// Structure to represent a nurse assignment
struct NurseAssignment {
    int nurseNumber;
    int day;
    ShiftType shift;

    bool operator<(const NurseAssignment& other) const {
        return tie(nurseNumber, day, shift) < tie(other.nurseNumber, other.day, other.shift);
    }
};

// Set to keep track of nurse assignments
set<NurseAssignment> assignedNurses;

// Function to implement the shift specification algorithm
ShiftType specifyShift(int& dm, int& de, int& dn) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 3);
    
    int r = dis(gen);
    
    if (r == 1) {
        if (dm == 0) return specifyShift(dm, de, dn);
        if (de > 0) {
            dm--;
            de--;
            return MORNING;
        } else {
            dm--;
            return MORNING;
        }
    } else if (r == 2) {
        if (de == 0) return specifyShift(dm, de, dn);
        if (dm > 0) {
            dm--;
            de--;
            return EVENING;
        } else {
            de--;
            return EVENING;
        }
    } else {
        if (dn == 0) return specifyShift(dm, de, dn);
        dn--;
        return NIGHT;
    }
}

// Function to check if a nurse is available for a shift
bool isNurseAvailable(const Nurse& nurse, int day, ShiftType shift) {
    NurseAssignment assignment{nurse.nurseNumber, day, shift};
    return assignedNurses.find(assignment) == assignedNurses.end();
}

// Function to assign nurses based on the specified shift
void assignNurses(vector<Nurse>& nurses, ShiftType shift, int required_nurses, int day, const string& department) {
    vector<Nurse*> selected_nurses;
    int shift_index = (day - 1) * 3 + static_cast<int>(shift) - static_cast<int>(MORNING);

    // First pass: select nurses with preference "2"
    for (auto& nurse : nurses) {
        if (selected_nurses.size() >= required_nurses) break;
        if (nurse.shiftPreferences[shift_index] == 2 && isNurseAvailable(nurse, day, shift)) {
            selected_nurses.push_back(&nurse);
        }
    }

    // Second pass: select nurses with preference "1" if needed
    if (selected_nurses.size() < required_nurses) {
        for (auto& nurse : nurses) {
            if (selected_nurses.size() >= required_nurses) break;
            if (nurse.shiftPreferences[shift_index] == 1 && 
                isNurseAvailable(nurse, day, shift) &&
                find(selected_nurses.begin(), selected_nurses.end(), &nurse) == selected_nurses.end()) {
                selected_nurses.push_back(&nurse);
            }
        }
    }

    // Assign the selected nurses
    for (Nurse* nurse : selected_nurses) {
        cout << "Assigning Nurse " << nurse->fullName << ", " << nurse->nurseType 
             << " to " << static_cast<char>(shift) << " shift on day " << day 
             << " in " << department << endl;
        
        // Add the assignment to the set
        assignedNurses.insert({nurse->nurseNumber, day, shift});
    }
}

int main() {
    // Load nurses from CSV
    parseNursesCSV("/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/LinkedListDS/Nurse_List_Department_Included.csv");

    vector<string> departments = { "Oncology", "Pediatric", "Surgery" };
    vector<string> nurse_types = { "RN", "LPN", "NA" };

    int total_satisfaction_score = 0;
    int total_assigned_nurses = 0;

    // 5-day schedule
    for (int day = 1; day <= 14; ++day) {
        for (const string& department : departments) {
            // Initialize demand for each shift type
            int dm = 5, de = 7, dn = 3; // Example values, adjust as needed

            for (const string& nurse_type : nurse_types) {
                while (dm > 0 || de > 0 || dn > 0) {
                    ShiftType shift = specifyShift(dm, de, dn);
                    int shift_num = (day - 1) * 3 + static_cast<int>(shift) - static_cast<int>(MORNING) + 1;
                    
                    // Check if we have nurses of this type in this department
                    if (departmentNursesMap.count(department) && 
                        departmentNursesMap[department].count(nurse_type)) {
                        
                        vector<Nurse>& nurses = departmentNursesMap[department][nurse_type];
                        int required_nurses = 1; // Adjust this based on your requirements

                        assignNurses(nurses, shift, required_nurses, day, department);
                        
                        // Update satisfaction score (simplified)
                        total_satisfaction_score += required_nurses * 2; // Assuming all assignments are satisfactory
                        total_assigned_nurses += required_nurses;
                    }
                }
            }
        }
    }

    double overall_average_satisfaction = total_assigned_nurses > 0 ? 
        static_cast<double>(total_satisfaction_score) / total_assigned_nurses : 0.0;
    cout << "Overall average satisfaction score: " << overall_average_satisfaction << endl;

    return 0;
}