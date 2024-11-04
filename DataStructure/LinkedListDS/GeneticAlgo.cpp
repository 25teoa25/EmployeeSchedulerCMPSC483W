#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

// Constants
const int POPULATION_SIZE = 100;
const int MAX_GENERATIONS = 500;
const double MUTATION_RATE = 0.05;
const int NUM_SHIFTS = 42;

// Data Structures
struct Nurse {
    std::string fullName;
    int nurseNumber;
    std::string nurseType;
    std::string department;
    std::vector<int> shiftPreferences; // 0: Wants to work, 1: Prefer not, 2: Doesn't want to work
};

struct ShiftConstraint {
    int requiredNurses;
    std::string nurseType;
    std::string department;
};

using constraintsMap = std::unordered_map<int, std::vector<ShiftConstraint>>;
using Chromosome = std::vector<std::vector<int>>; // Each shift has a list of nurse IDs

// Global Data
std::vector<Nurse> nurses;
constraintsMap constraints;

// Function to parse nurses from file
std::vector<Nurse> parseNurses(const std::string& filename) {
    std::vector<Nurse> nurses;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open nurse file");
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Nurse nurse;
        ss >> nurse.fullName >> nurse.nurseNumber >> nurse.nurseType >> nurse.department;
        for (int i = 0; i < NUM_SHIFTS; ++i) {
            int preference;
            ss >> preference;
            nurse.shiftPreferences.push_back(preference);
        }
        nurses.push_back(nurse);
    }

    file.close();
    return nurses;
}

// Function to parse constraints from file
constraintsMap parseConstraints(const std::string& filename) {
    constraintsMap constraints;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open constraints file");
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int shift;
        std::string department, nurseType;
        int requiredNurses;

        ss >> shift >> department >> nurseType >> requiredNurses;
        constraints[shift].push_back({requiredNurses, nurseType, department});
    }

    file.close();
    return constraints;
}

// Get nurses eligible for a shift based on type and department
std::vector<Nurse> getAvailableNurses(const std::string& type, const std::string& department, int shift) {
    std::vector<Nurse> eligibleNurses;
    for (const auto& nurse : nurses) {
        if (nurse.nurseType == type && nurse.department == department && nurse.shiftPreferences[shift] == 0) {
            eligibleNurses.push_back(nurse);
        }
    }
    return eligibleNurses;
}

// Initialize population of chromosomes, meeting constraints
void initialize_population(std::vector<Chromosome>& population) {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int p = 0; p < POPULATION_SIZE; ++p) {
        Chromosome chromosome(NUM_SHIFTS);

        for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
            const auto& shiftConstraints = constraints[shift];

            for (const auto& constraint : shiftConstraints) {
                std::vector<Nurse> eligibleNurses = getAvailableNurses(constraint.nurseType, constraint.department, shift);

                for (int i = 0; i < constraint.requiredNurses && !eligibleNurses.empty(); ++i) {
                    std::uniform_int_distribution<> distr(0, eligibleNurses.size() - 1);
                    int selectedNurseIndex = distr(gen);

                    chromosome[shift].push_back(eligibleNurses[selectedNurseIndex].nurseNumber);
                    eligibleNurses.erase(eligibleNurses.begin() + selectedNurseIndex); // Avoid reassignment
                }
            }
        }
        population.push_back(chromosome);
    }
}

// Calculate fitness of a chromosome based on nurse preferences and constraint satisfaction
int calculate_fitness(const Chromosome& chromosome) {
    int fitness = 0;

    for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
        const auto& shiftConstraints = constraints[shift];
        std::unordered_map<std::string, int> nurseTypeCount;
        std::unordered_map<std::string, int> departmentCount;

        for (int nurseID : chromosome[shift]) {
            const Nurse& nurse = nurses[nurseID];

            // Count nurse types and departments to check constraints
            nurseTypeCount[nurse.nurseType]++;
            departmentCount[nurse.department]++;

            // Add to fitness based on shift preference
            fitness -= nurse.shiftPreferences[shift];
        }

        // Apply penalties for constraint violations
        for (const auto& constraint : shiftConstraints) {
            if (nurseTypeCount[constraint.nurseType] < constraint.requiredNurses ||
                departmentCount[constraint.department] < constraint.requiredNurses) {
                fitness -= 10; // Penalty for each unmet constraint
            }
        }
    }
    return fitness;
}

// Tournament selection
Chromosome select_parent(const std::vector<Chromosome>& population, const std::vector<int>& fitnessScores) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, POPULATION_SIZE - 1);

    int bestIndex = distr(gen);
    for (int i = 0; i < 3; ++i) {
        int competitor = distr(gen);
        if (fitnessScores[competitor] < fitnessScores[bestIndex]) {
            bestIndex = competitor;
        }
    }
    return population[bestIndex];
}

// Crossover function
Chromosome crossover(const Chromosome& parent1, const Chromosome& parent2) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, NUM_SHIFTS - 1);

    Chromosome offspring(NUM_SHIFTS);
    int crossover_point = distr(gen);

    for (int i = 0; i < NUM_SHIFTS; ++i) {
        if (i < crossover_point) {
            offspring[i] = parent1[i];
        } else {
            offspring[i] = parent2[i];
        }
    }
    return offspring;
}

// Mutation function
void mutate(Chromosome& chromosome) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> mutationChance(0.0, 1.0);

    for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
        if (mutationChance(gen) < MUTATION_RATE) {
            const auto& shiftConstraints = constraints[shift];

            for (const auto& constraint : shiftConstraints) {
                std::vector<Nurse> eligibleNurses = getAvailableNurses(constraint.nurseType, constraint.department, shift);
                if (!eligibleNurses.empty()) {
                    std::uniform_int_distribution<> distr(0, eligibleNurses.size() - 1);
                    int selectedNurseIndex = distr(gen);
                    chromosome[shift].push_back(eligibleNurses[selectedNurseIndex].nurseNumber);
                }
            }
        }
    }
}

// Main genetic algorithm
int main() {
    // Load nurses and constraints from structured data
    nurses = parseNurses("Nurse_List_Department_Included.csv");
    constraints = parseConstraints("Nurse Constraints Test.csv");

    // Step 1: Initialize population
    std::vector<Chromosome> population;
    initialize_population(population);

    // Step 2: Evaluate initial fitness
    std::vector<int> fitnessScores(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        fitnessScores[i] = calculate_fitness(population[i]);
    }

    // Track the best chromosome across generations
    Chromosome best_chromosome = population[0];
    int best_fitness = fitnessScores[0];

    // Evolution loop
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        std::vector<Chromosome> new_population;

        while (new_population.size() < POPULATION_SIZE) {
            Chromosome parent1 = select_parent(population, fitnessScores);
            Chromosome parent2 = select_parent(population, fitnessScores);
            Chromosome offspring = crossover(parent1, parent2);

            mutate(offspring);
            int offspring_fitness = calculate_fitness(offspring);

            new_population.push_back(offspring);
            if (offspring_fitness < best_fitness) {
                best_chromosome = offspring;
                best_fitness = offspring_fitness;
                std::cout << "New best fitness in generation " << generation << ": " << best_fitness << std::endl;
            }
        }

        population = std::move(new_population);
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            fitnessScores[i] = calculate_fitness(population[i]);
        }
    }

    // Output the best chromosome
    std::cout << "\nBest Schedule Found:\nFitness: " << best_fitness << "\n";
    for (int shift = 0; shift < NUM_SHIFTS; ++shift) {
        std::cout << "Shift " << (shift + 1) << ": ";
        for (int nurseID : best_chromosome[shift]) {
            std::cout << "Nurse " << nurseID << " ";
        }
        std::cout << "\n";
    }

    return 0;
}