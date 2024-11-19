#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>
#include <unordered_map>
#include "NurseList.h"  // Include for departmentNursesMap access
#include "CSVParser.h"  // Include for parseNursesCSV

using namespace std;

// Constants for genetic algorithm configuration
const int NUM_SHIFTS = 42;
const int POPULATION_SIZE = 500;
const int MAX_GENERATIONS = 500;
const double MUTATION_RATE = 0.05;
const int PENALTY_WEIGHT_CONSECUTIVE = 20;
const int PENALTY_WEIGHT_SHIFT_COUNT = 30;
const int PENALTY_WEIGHT_SPECIAL = 15;
const int PENALTY_WEIGHT_DEPARTMENT = 25;

// Nurse and department structures
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Nurse>>> departmentNursesMap;
extern std::unordered_map<int, std::unordered_map<std::string, std::unordered_map<std::string, int>>> constraintsMap;

// Sample structure to represent a schedule (this might be more complex in reality)
struct Schedule {
    vector<int> shiftAssignments; // Array to represent nurse IDs assigned to each shift
    int fitness;

    Schedule() : shiftAssignments(NUM_SHIFTS, -1), fitness(0) {}
};
void applyConstraints(Schedule& schedule) {
    int total_penalty = 0;
    std::unordered_map<int, int> nurseShiftCount;  // Track shift count per nurse for 2-week period
    std::unordered_map<int, int> nurseConsecutiveShifts;  // Track consecutive shifts per nurse

    for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
        std::unordered_map<std::string, int> nurseTypeCounts; // Tracks nurse type counts for this shift

        // Count assigned nurse types for each shift and check individual constraints
        for (int nurseId : schedule.shiftAssignments) {
            if (nurseId != -1) { // Valid nurse assigned
                // Find nurse's department and type from departmentNursesMap
                std::string department;
                std::string type;
                bool found = false;

                // Loop through departmentNursesMap to find the department and type
                for (const auto& [dept, typeMap] : departmentNursesMap) {
                    for (const auto& [typ, nurses] : typeMap) {
                        // Check if nurseId is within range and assign department and type
                        if (nurseId < nurses.size()) {  // Updated line
                            department = dept;
                            type = typ;
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                }

                // Increment the count for this type in this shift's department
                nurseTypeCounts[type]++;
                nurseShiftCount[nurseId]++;  // Track total shifts per nurse

                // Check consecutive shifts
                if (nurseConsecutiveShifts[nurseId] > 2) {
                    total_penalty += PENALTY_WEIGHT_CONSECUTIVE; // Penalty for excess consecutive shifts
                }
                if (shift > 0 && schedule.shiftAssignments[shift - 1] == nurseId) {
                    nurseConsecutiveShifts[nurseId]++;
                } else {
                    nurseConsecutiveShifts[nurseId] = 1; // Reset if not consecutive
                }

                // Enforce department special conditions (weekend or daytime only)
                if (department == "Nurse Anesthetist" && (shift % 7 >= 5)) {  // Weekend restriction
                    total_penalty += PENALTY_WEIGHT_SPECIAL;  // Penalty if scheduled improperly
                }
                if ((department == "Nurse Educator" || department == "Health Informatics" || department == "Occupational Health Nursing") 
                    && (shift % 3 == 2)) {  // Overnight shift restriction (third shift each day)
                    total_penalty += PENALTY_WEIGHT_SPECIAL;
                }
            }
        }

        // Apply penalties based on department constraints
        for (const auto& [department, typeMap] : constraintsMap[shift]) {
            for (const auto& [type, requiredCount] : typeMap) {
                int actualCount = nurseTypeCounts[type];
                if (actualCount < requiredCount) {
                    int deficit = requiredCount - actualCount;
                    total_penalty += deficit * PENALTY_WEIGHT_DEPARTMENT; // Penalty for each missing nurse
                }
            }
        }
    }

    // Apply shift count penalty for any nurse over the 10-shift limit
    for (const auto& [nurseId, count] : nurseShiftCount) {
        if (count > 10) {
            total_penalty += (count - 10) * PENALTY_WEIGHT_SHIFT_COUNT;
        }
    }

    // Deduct total penalty from fitness
    schedule.fitness -= total_penalty;
}

int calculateFitness(const Schedule& schedule) {
    int fitness = 0;
    unordered_map<string, int> departmentUsage;  // Track usage by department

    for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
        int nurseId = schedule.shiftAssignments[shift];
        if (nurseId != -1) {
            for (auto& [department, typeMap] : departmentNursesMap) {
                for (auto& [type, nurses] : typeMap) {
                    if (nurseId < nurses.size()) {
                        departmentUsage[department]++;
                        int preference = nurses[nurseId].shiftPreferences[shift];
                        if (preference == 0) fitness += 100;  // Highest bonus for preference 0
                        else if (preference == 1) fitness -= 100;  // Moderate penalty for preference 1
                        else if (preference == 2) fitness -= 300;  // Severe penalty for preference 2
                    }
                }
            }
        } else {
            fitness -= 100; // Penalty for unfilled shift
        }
    }

    // Penalize overuse of a single department
    for (const auto& [department, usage] : departmentUsage) {
        if (usage > NUM_SHIFTS / departmentNursesMap.size()) {
            fitness -= (usage - NUM_SHIFTS / departmentNursesMap.size()) * 20;
        }
    }

    return fitness;
}


void printScheduleDebug(const Schedule& schedule) {
    cout << "Schedule Debugging Info:\n";
    unordered_map<string, int> departmentUsage;

    for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
        int nurseId = schedule.shiftAssignments[shift];

        if (nurseId != -1) {
            for (const auto& [department, typeMap] : departmentNursesMap) {
                for (const auto& [type, nurses] : typeMap) {
                    auto it = std::find_if(nurses.begin(), nurses.end(), 
                                           [&](const Nurse& nurse) { return nurse.nurseNumber == nurseId; });

                    if (it != nurses.end()) {
                        departmentUsage[department]++;
                        cout << "Shift " << shift + 1 
                             << ": Nurse ID " << it->nurseNumber 
                             << " (" << it->fullName << ")"
                             << ", Department: " << department 
                             << ", Type: " << type 
                             << ", Preference: " << it->shiftPreferences[shift] << endl;
                        break;
                    }
                }
            }
        } else {
            cout << "Shift " << shift + 1 << ": No nurse assigned.\n";
        }
    }

    cout << "\nDepartment Usage:\n";
    for (const auto& [department, count] : departmentUsage) {
        cout << department << ": " << count << " shifts\n";
    }
    cout << endl;
}



vector<Schedule> initializePopulation() {
    vector<Schedule> population(POPULATION_SIZE);
    random_device rd;
    mt19937 gen(rd());

    for (auto& schedule : population) {
        unordered_map<string, int> departmentUsage;  // Track department usage

        for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
            bool assigned = false;

            // Ensure balanced department selection
            for (auto& [department, typeMap] : departmentNursesMap) {
                if (departmentUsage[department] >= NUM_SHIFTS / departmentNursesMap.size()) {
                    continue;  // Skip if department is overused
                }

                for (auto& [type, nurses] : typeMap) {
                    vector<int> preferenceZeroNurses;

                    // Collect nurses with preference 0 for the current shift
                    for (int nurseId = 0; nurseId < nurses.size(); ++nurseId) {
                        if (nurses[nurseId].shiftPreferences[shift] == 0) {
                            preferenceZeroNurses.push_back(nurseId);
                        }
                    }

                    if (!preferenceZeroNurses.empty()) {
                        // Randomly select a nurse with preference 0 for the shift
                        uniform_int_distribution<> nurseDist(0, preferenceZeroNurses.size() - 1);
                        int chosenNurseId = preferenceZeroNurses[nurseDist(gen)];
                        schedule.shiftAssignments[shift] = nurses[chosenNurseId].nurseNumber;
                        departmentUsage[department]++;
                        assigned = true;
                        break;
                    }
                }
                if (assigned) break;
            }

            // Fallback: Assign any nurse if no preference 0 nurses found
            if (!assigned) {
                for (auto& [department, typeMap] : departmentNursesMap) {
                    for (auto& [type, nurses] : typeMap) {
                        vector<int> availableNurses;

                        for (int nurseId = 0; nurseId < nurses.size(); ++nurseId) {
                            if (nurses[nurseId].shiftPreferences[shift] != 2) {  // Avoid preference 2
                                availableNurses.push_back(nurseId);
                            }
                        }

                        if (!availableNurses.empty()) {
                            uniform_int_distribution<> nurseDist(0, availableNurses.size() - 1);
                            int chosenNurseId = availableNurses[nurseDist(gen)];
                            schedule.shiftAssignments[shift] = nurses[chosenNurseId].nurseNumber;
                            departmentUsage[department]++;
                            assigned = true;
                            break;
                        }
                    }
                    if (assigned) break;
                }
            }

            if (!assigned) {
                schedule.shiftAssignments[shift] = -1;  // No nurse assigned
            }
        }

        schedule.fitness = calculateFitness(schedule);
        applyConstraints(schedule); // Adjust fitness based on constraints
    }

    return population;
}



// Selection, crossover, and mutation functions (simplified, add as needed)
void mutate(Schedule& schedule) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
        if (dis(gen) < MUTATION_RATE) {
            int randomNurseIndex = gen() % NUM_SHIFTS;
            schedule.shiftAssignments[shift] = randomNurseIndex;
        }
    }
}

void geneticAlgorithm() {
    vector<Schedule> population = initializePopulation();

    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        for (auto& schedule : population) {
            schedule.fitness = calculateFitness(schedule);
        }

        // Sorting based on fitness for demonstration (selection would be more complex)
        sort(population.begin(), population.end(), [](const Schedule& a, const Schedule& b) {
            return a.fitness > b.fitness;
        });

        // Mutate the least fit half of the population
        for (size_t i = POPULATION_SIZE / 2; i < population.size(); ++i) {
            mutate(population[i]);
        }

        // Print debugging info for the best schedule of the generation
        cout << "Generation " << generation + 1 << " - Best Schedule Fitness: " << population[0].fitness << endl;
        printScheduleDebug(population[0]);  // Debugging output for the best schedule
    }

    cout << "Final Best Schedule Fitness: " << population[0].fitness << endl;
    printScheduleDebug(population[0]);  // Final debug output for the best schedule
}

int main() {
    parseNursesCSV("Nurse_List_Department_Included.csv");
    geneticAlgorithm();
    return 0;
}
