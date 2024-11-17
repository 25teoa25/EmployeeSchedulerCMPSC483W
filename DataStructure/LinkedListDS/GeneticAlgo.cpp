#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>
#include "NurseList.h"  // Include for departmentNursesMap access
#include "CSVParser.h"  // Include for parseNursesCSV

using namespace std;

// Constants for genetic algorithm configuration
const int NUM_SHIFTS = 42;
const int POPULATION_SIZE = 500;
const int MAX_GENERATIONS = 500;
const double MUTATION_RATE = 0.05;

// Sample structure to represent a schedule (this might be more complex in reality)
struct Schedule {
    vector<int> shiftAssignments; // Array to represent nurse IDs assigned to each shift
    int fitness;

    Schedule() : shiftAssignments(NUM_SHIFTS, -1), fitness(0) {}
};

// Calculate fitness based on nurse preferences
// Calculate fitness with strong prioritization for preference 0
int calculateFitness(const Schedule& schedule) {
    int fitness = 0;
    for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
        int nurseId = schedule.shiftAssignments[shift];
        if (nurseId != -1) {
            bool validNurse = false;
            for (auto& [department, typeMap] : departmentNursesMap) {
                for (auto& [type, nurses] : typeMap) {
                    if (nurseId < nurses.size()) {
                        validNurse = true;
                        int preference = nurses[nurseId].shiftPreferences[shift];
                        if (preference == 0) fitness += 50;  // Higher bonus for preference 0
                        else if (preference == 1) fitness -= 100;  // Higher penalty for preference 1
                        else if (preference == 2) fitness -= 200;  // Stronger penalty for preference 2
                        break;
                    }
                }
                if (validNurse) break;
            }
        }
    }
    return fitness;
}

void printScheduleDebug(const Schedule& schedule) {
    cout << "Schedule Debugging Info:\n";
    for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
        int nurseId = schedule.shiftAssignments[shift];
        
        if (nurseId != -1) {
            bool nurseFound = false;
            for (const auto& [department, typeMap] : departmentNursesMap) {
                for (const auto& [type, nurses] : typeMap) {
                    // Search for nurse by nurseNumber
                    auto it = std::find_if(nurses.begin(), nurses.end(), 
                                           [&](const Nurse& nurse) { return nurse.nurseNumber == nurseId; });
                    
                    if (it != nurses.end()) {
                        cout << "Shift " << shift + 1 
                             << ": Nurse ID " << it->nurseNumber 
                             << " (" << it->fullName << ")"
                             << ", Department: " << department 
                             << ", Type: " << type 
                             << ", Preference: " << it->shiftPreferences[shift] << endl;
                        nurseFound = true;
                        break;
                    }
                }
                if (nurseFound) break;
            }
            if (!nurseFound) {
                cout << "Shift " << shift + 1 << ": Nurse with ID " << nurseId << " not found in departmentNursesMap.\n";
            }
        } else {
            cout << "Shift " << shift + 1 << ": No nurse assigned.\n";
        }
    }
    cout << endl;
}

vector<Schedule> initializePopulation() {
    vector<Schedule> population(POPULATION_SIZE);
    random_device rd;
    mt19937 gen(rd());

    for (auto& schedule : population) {
        for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
            bool assigned = false;

            // Prioritize nurses with preference 0 for each shift
            for (auto& [department, typeMap] : departmentNursesMap) {
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
                        schedule.shiftAssignments[shift] = nurses[chosenNurseId].nurseNumber;  // Use nurse number directly
                        assigned = true;
                        break;
                    }
                }
                if (assigned) break;
            }

            // Fallback: If no nurse with preference 0 is found, assign a random nurse
            if (!assigned) {
                for (auto& [department, typeMap] : departmentNursesMap) {
                    for (auto& [type, nurses] : typeMap) {
                        uniform_int_distribution<> nurseDist(0, nurses.size() - 1);
                        int randomNurseId = nurseDist(gen);
                        schedule.shiftAssignments[shift] = nurses[randomNurseId].nurseNumber;  // Ensure it's a valid nurse number
                        assigned = true;
                        break;
                    }
                    if (assigned) break;
                }
            }
        }
        // Calculate fitness after initializing the schedule
        schedule.fitness = calculateFitness(schedule);
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
