/*//#include "DataStructure/PrototypeV2/CSVParser.h"  
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Constants for simulation
const int NUM_NURSES = 1000;
const int NUM_SHIFTS = 42;
const int POPULATION_SIZE = 100;
const int MAX_GENERATIONS = 500;
const double MUTATION_RATE = 0.05;

// Nurse preference (0: can work, 1: prefer not, 2: doesn't want to work)
int nurse_preferences[NUM_NURSES][NUM_SHIFTS];

// Function to read data from CSV and fill nurse_preferences
void read_csv(const string& filename) {
    ifstream file(filename);
    string line;
    int nurse_index = 0;

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    // Skip the header line
    getline(file, line);

    // Process each line in the CSV
    while (getline(file, line) && nurse_index < NUM_NURSES) {
        stringstream ss(line);
        string value;
        int shift_index = 0;
        int column_index = 0;

        while (getline(ss, value, ',')) {
            if (column_index >= 4) {
                if (shift_index < NUM_SHIFTS) {
                    nurse_preferences[nurse_index][shift_index] = stoi(value);
                    shift_index++;
                }
            }
            column_index++;
        }

        if (shift_index != NUM_SHIFTS) {
            cerr << "Error: Row " << nurse_index + 1 << " does not have " << NUM_SHIFTS << " shift preferences.\n";
            exit(1);
        }

        nurse_index++;
    }

    file.close();
    cout << "CSV file successfully read and loaded into nurse_preferences array.\n";
}

// Chromosome structure representing a schedule
struct Chromosome {
    vector<int> shifts;
    int fitness;

    Chromosome() {
        shifts.resize(NUM_SHIFTS, -1);
        fitness = 0;
    }
};

// Helper function to initialize the population
vector<Chromosome> initialize_population() {
    vector<Chromosome> population(POPULATION_SIZE);
    random_device rd;
    mt19937 gen(rd());

    for (auto& chromosome : population) {
        for (int i = 0; i < NUM_SHIFTS; ++i) {
            vector<int> preferred_nurses;
            vector<int> fallback_nurses;

            // Group nurses based on their preferences for the current shift
            for (int nurse = 0; nurse < NUM_NURSES; ++nurse) {
                if (nurse_preferences[nurse][i] == 0) {
                    preferred_nurses.push_back(nurse);
                } else if (nurse_preferences[nurse][i] == 1) {
                    fallback_nurses.push_back(nurse);
                }
            }

            // Assign a nurse, prioritizing those with preference 0
            if (!preferred_nurses.empty()) {
                uniform_int_distribution<> distr(0, preferred_nurses.size() - 1);
                chromosome.shifts[i] = preferred_nurses[distr(gen)];
            } else if (!fallback_nurses.empty()) {
                uniform_int_distribution<> distr(0, fallback_nurses.size() - 1);
                chromosome.shifts[i] = fallback_nurses[distr(gen)];
            } else {
                chromosome.shifts[i] = gen() % NUM_NURSES;
            }
        }
    }
    return population;
}

// Calculate fitness based on preference
int calculate_fitness(const Chromosome& chromosome) {
    int fitness = 0;
    for (int i = 0; i < NUM_SHIFTS; ++i) {
        int nurse_id = chromosome.shifts[i];
        int preference = nurse_preferences[nurse_id][i];

        // Adjust fitness based on preference
        if (preference == 1) {
            fitness -= 1;
        } else if (preference == 2) {
            fitness -= 2;
        }
    }
    return fitness;
}

// Tournament selection
Chromosome select_parent(const vector<Chromosome>& population) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, POPULATION_SIZE - 1);

    Chromosome best = population[distr(gen)];
    for (int i = 0; i < 3; ++i) {
        Chromosome competitor = population[distr(gen)];
        if (competitor.fitness < best.fitness) {
            best = competitor;
        }
    }
    return best;
}

// Crossover function
Chromosome crossover(const Chromosome& parent1, const Chromosome& parent2) {
    Chromosome offspring;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, NUM_SHIFTS - 1);
    int crossover_point = distr(gen);

    for (int i = 0; i < NUM_SHIFTS; ++i) {
        if (i < crossover_point) {
            offspring.shifts[i] = parent1.shifts[i];
        } else {
            offspring.shifts[i] = parent2.shifts[i];
        }
    }
    return offspring;
}

// Mutation function
void mutate(Chromosome& chromosome) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distr(0.0, 1.0);
    uniform_int_distribution<> shift_distr(0, NUM_SHIFTS - 1);

    if (distr(gen) < MUTATION_RATE) {
        int shift = shift_distr(gen);
        vector<int> preferred_nurses;

        for (int nurse = 0; nurse < NUM_NURSES; ++nurse) {
            if (nurse_preferences[nurse][shift] == 0) {
                preferred_nurses.push_back(nurse);
            }
        }

        if (!preferred_nurses.empty()) {
            uniform_int_distribution<> nurse_distr(0, preferred_nurses.size() - 1);
            chromosome.shifts[shift] = preferred_nurses[nurse_distr(gen)];
        }
    }
}

// Main function to run the genetic algorithm
int main() {
    // Step 1: Read nurse preferences from the CSV file
    read_csv("Nurse_List_Department_Included.csv");

    // Step 2: Initialize the population with random schedules
    vector<Chromosome> population = initialize_population();

    // Step 3: Evaluate the initial fitness of each chromosome in the population
    for (auto& chromosome : population) {
        chromosome.fitness = calculate_fitness(chromosome);
    }

    // Step 4: Track the best chromosome across all generations
    Chromosome best_overall = *min_element(population.begin(), population.end(),
        [](const Chromosome& a, const Chromosome& b) {
            return a.fitness < b.fitness;
        });

    // Step 5: Evolution loop - generate new populations for each generation
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        vector<Chromosome> new_population;

        // Generate new population through selection, crossover, and mutation
        while (new_population.size() < POPULATION_SIZE) {
            Chromosome parent1 = select_parent(population);
            Chromosome parent2 = select_parent(population);
            Chromosome offspring = crossover(parent1, parent2);
            mutate(offspring);
            offspring.fitness = calculate_fitness(offspring);
            new_population.push_back(offspring);
        }

        population = std::move(new_population);

        auto best_chromosome_iter = min_element(population.begin(), population.end(),
            [](const Chromosome& a, const Chromosome& b) {
                return a.fitness < b.fitness;
            });

        if (best_chromosome_iter != population.end() && best_chromosome_iter->fitness < best_overall.fitness) {
            best_overall = *best_chromosome_iter;
            cout << "New best fitness found in generation " << generation << ": " << best_overall.fitness << endl;
        }
    }

    // Step 6: Output the best chromosome found
    cout << "\nBest Schedule Found:\n";
    cout << "Fitness: " << best_overall.fitness << "\n";
    cout << "Shift Assignments:\n";
    for (int i = 0; i < NUM_SHIFTS; ++i) {
        int nurse = best_overall.shifts[i];
        int preference = nurse_preferences[nurse][i];
        cout << "Shift " << (i + 1) << ": Nurse " << nurse + 1 << " (Preference: " << preference << ")\n";
    }

    return 0;
}
*/
#include "NurseList.h"
#include "CSVParser.h"
#include "NurseFunctions.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using json = nlohmann::json;

// Global variables for nurse data and constraints
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Nurse>>> departmentNursesMap;
extern std::unordered_map<int, std::unordered_map<std::string, std::unordered_map<std::string, int>>> constraintsMap;


// Define a chromosome (schedule)
struct Schedule {
    std::vector<std::vector<Nurse>> shifts; // Shift assignments for nurses

    // Fitness function: Count satisfied nurses and apply constraints
    int fitness() const {
        int satisfiedCount = 0;
        int penalty = 0;

        // General constraints: No more than 10 shifts in 2 weeks
        std::unordered_map<int, int> shiftCount;

        // Check all nurses' preferences and apply constraints
        for (const auto& department : departmentNursesMap) {
            for (const auto& type : department.second) {
                for (const auto& nurse : type.second) {
                    bool foundViolation = false;
                    // Count shifts for each nurse and check for consecutive shift violations
                    int consecutiveShifts = 0;

                    for (int shift : nurse.shiftPreferences) {
                        if (shiftCount[nurse.nurseNumber] >= 10) {
                            penalty += 100; // Penalize if a nurse exceeds 10 shifts
                            foundViolation = true;
                        }
                        shiftCount[nurse.nurseNumber]++;
                        if (shift == 1) consecutiveShifts++;
                        if (consecutiveShifts > 2) {
                            penalty += 100; // Penalize if a nurse works more than 2 shifts in a row
                            foundViolation = true;
                        }
                    }

                    if (!foundViolation) {
                        satisfiedCount++;
                    }
                }
            }
        }

        // Department-specific nurse requirements (ensuring correct number of nurses in each department)
        for (const auto& department : departmentNursesMap) {
            for (const auto& type : department.second) {
                int requiredNurses = constraintsMap[0][department.first][type.first];
                int actualNursesAssigned = type.second.size(); // Count of nurses assigned to this type

                if (actualNursesAssigned != requiredNurses) {
                    penalty += 200; // Penalize for not meeting department nurse requirements
                }
            }
        }

        // Special conditions: Check for Nurse Anesthetist, Nurse Educator, etc.
        for (const auto& department : departmentNursesMap) {
            for (const auto& type : department.second) {
                if (type.first == "Nurse Anesthetist") {
                    // Ensure only 1 Nurse Anesthetist is assigned to weekends
                    int weekendCount = 0;
                    for (int shift : type.second[0].shiftPreferences) {
                        if (shift == 6 || shift == 7) weekendCount++; // Check for weekend shifts
                    }
                    if (weekendCount > 1) penalty += 100;
                } else if (type.first == "Nurse Educator" || type.first == "Health Informatics" || type.first == "Occupational Health Nursing") {
                    // Ensure these roles only work on weekdays (no weekend or overnight shifts)
                    for (int shift : type.second[0].shiftPreferences) {
                        if (shift == 6 || shift == 7) penalty += 100; // Penalize for working on weekends
                    }
                }
            }
        }

        return satisfiedCount - penalty; // Return total score, considering penalties
    }
};

// Initialize population
std::vector<Schedule> initializePopulation(int populationSize) {
    std::vector<Schedule> population;

    for (int i = 0; i < populationSize; i++) {
        Schedule schedule;
        // Initialize random schedule (need add from req)
        // For example, randomly assign nurses to shifts in a valid way while respecting constraints
        population.push_back(schedule);
    }

    return population;
}

// Select two parents based on fitness
Schedule selectParent(const std::vector<Schedule>& population) {
    // Sort population by fitness in descending order
    std::vector<Schedule> sortedPopulation = population;
    std::sort(sortedPopulation.begin(), sortedPopulation.end(), [](const Schedule& a, const Schedule& b) {
        return a.fitness() > b.fitness();
    });

    // Select two best individuals
    return sortedPopulation[0]; // Return the best schedule
}

// Crossover (recombination) between two parents to create a child schedule
Schedule crossover(const Schedule& parent1, const Schedule& parent2) {
    Schedule child;
    // For simplicity, take half from parent1 and half from parent2
    // need improved maybe

    for (int shift = 0; shift < 42; shift++) { // 42 shifts
        if (rand() % 2 == 0) {
            child.shifts[shift] = parent1.shifts[shift];
        } else {
            child.shifts[shift] = parent2.shifts[shift];
        }
    }

    return child;
}

// Mutation: Randomly change one shift assignment in the schedule
void mutate(Schedule& schedule) {
    int shift = rand() % 42; // Randomly choose a shift
    int nurseIndex = rand() % schedule.shifts[shift].size(); // Randomly choose a nurse

    // Mutate: Reassign nurse to a different shift
    Nurse nurse = schedule.shifts[shift][nurseIndex];
    schedule.shifts[shift][nurseIndex] = schedule.shifts[rand() % 42][rand() % schedule.shifts[shift].size()];
    schedule.shifts[rand() % 42][rand() % schedule.shifts[shift].size()] = nurse;
}

// Genetic Algorithm
Schedule geneticAlgorithm(int populationSize, int generations, double mutationRate) {
    std::vector<Schedule> population = initializePopulation(populationSize);

    for (int generation = 0; generation < generations; generation++) {
        std::vector<Schedule> newPopulation;

        // Selection and Crossover
        while (newPopulation.size() < populationSize) {
            Schedule parent1 = selectParent(population);
            Schedule parent2 = selectParent(population);
            Schedule child = crossover(parent1, parent2);
            newPopulation.push_back(child);
        }

        // Mutation
        for (Schedule& schedule : newPopulation) {
            if (rand() % 100 < mutationRate * 100) {
                mutate(schedule);
            }
        }

        population = newPopulation; // Update population for next generation
    }

    // Return the best schedule from the final generation
    return *std::max_element(population.begin(), population.end(), [](const Schedule& a, const Schedule& b) {
        return a.fitness() < b.fitness();
    });
}

int main() {
    srand(time(0));

    parseNursesCSV("Nurse_List_Department_Included.csv");

    // Constraints for departments 
    constraintsMap[0]["Cardiac Nursing"]["Registered Nurse"] = 8;
    constraintsMap[0]["Surgical Nurse"]["Registered Nurse"] = 10;
    // Continue --> requirements doc

   
    int populationSize = 100;     //population size
    int generations = 1000;       //number of generations
    double mutationRate = 0.05;   //mutation rate

    Schedule bestSchedule = geneticAlgorithm(populationSize, generations, mutationRate);


    std::cout << "Best schedule fitness: " << bestSchedule.fitness() << std::endl;

    return 0; 
}
