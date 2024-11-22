#include "GeneticAlgorithm.h"
#include "NurseList.h"
#include "NurseFunctions.h"
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <iostream>

// Extern variables from NurseList.cpp
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Nurse>>> departmentNursesMap;
extern std::unordered_map<int, std::unordered_map<std::string, std::unordered_map<std::string, int>>> constraintsMap;
extern int satisfactionScoreGeneticAlgorithm;

// Function prototypes
ShiftSchedule generateInitialSchedule(std::mt19937& gen);
double evaluateFitness(const ShiftSchedule& schedule);
ShiftSchedule tournamentSelection(const std::vector<ShiftSchedule>& population, const std::vector<double>& fitnesses, std::mt19937& gen);
void crossover(const ShiftSchedule& parent1, const ShiftSchedule& parent2, ShiftSchedule& child1, ShiftSchedule& child2, std::mt19937& gen);
void mutate(ShiftSchedule& schedule, std::mt19937& gen);

ShiftSchedule geneticAlgorithm() {
    // Parameters
    const int populationSize = 50;
    const int numGenerations = 100;
    const double crossoverRate = 0.7;
    const double mutationRate = 0.1;

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate initial population
    std::vector<ShiftSchedule> population;
    for (int i = 0; i < populationSize; ++i) {
        ShiftSchedule schedule = generateInitialSchedule(gen);
        population.push_back(schedule);
    }

    // Main GA loop
    for (int generation = 0; generation < numGenerations; ++generation) {
        // Evaluate fitness
        std::vector<double> fitnesses;
        for (const auto& schedule : population) {
            double fitness = evaluateFitness(schedule);
            fitnesses.push_back(fitness);
        }

        // Selection and reproduction
        std::vector<ShiftSchedule> newPopulation;
        for (int i = 0; i < populationSize / 2; ++i) {
            // Select parents
            ShiftSchedule parent1 = tournamentSelection(population, fitnesses, gen);
            ShiftSchedule parent2 = tournamentSelection(population, fitnesses, gen);

            // Crossover
            ShiftSchedule child1 = parent1;
            ShiftSchedule child2 = parent2;
            std::uniform_real_distribution<> dis(0.0, 1.0);
            if (dis(gen) < crossoverRate) {
                crossover(parent1, parent2, child1, child2, gen);
            }

            // Mutation
            if (dis(gen) < mutationRate) {
                mutate(child1, gen);
            }
            if (dis(gen) < mutationRate) {
                mutate(child2, gen);
            }

            // Add children to new population
            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }

        // Replace old population with new population
        population = newPopulation;

        // Optional: Print best fitness
        double bestFitness = *std::max_element(fitnesses.begin(), fitnesses.end());
        std::cout << "Generation " << generation << ", Best Fitness: " << bestFitness << std::endl;
    }

    // After GA loop, return the best schedule
    // Evaluate final fitnesses
    std::vector<double> finalFitnesses;
    for (const auto& schedule : population) {
        double fitness = evaluateFitness(schedule);
        finalFitnesses.push_back(fitness);
    }

    // Find the schedule with the highest fitness
    auto maxIter = std::max_element(finalFitnesses.begin(), finalFitnesses.end());
    int bestIndex = std::distance(finalFitnesses.begin(), maxIter);
    satisfactionScoreGeneticAlgorithm = static_cast<int>(*maxIter);

    return population[bestIndex];
}

ShiftSchedule generateInitialSchedule(std::mt19937& gen) {
    ShiftSchedule schedule(42);

    // For each shift
    for (int shift = 1; shift <= 42; ++shift) {
        // For each department in constraintsMap[shift]
        for (const auto& deptPair : constraintsMap[shift]) {
            const std::string& department = deptPair.first;
            // For each nurse type in constraintsMap[shift][department]
            for (const auto& typePair : deptPair.second) {
                std::string nurseType = typePair.first;
                int nursesNeeded = typePair.second;

                // Process nurseType to match keys in departmentNursesMap
                std::string modifiedNurseType = nurseType;
                const std::string prefix = "Nurse Type: ";

                if (modifiedNurseType.rfind(prefix, 0) == 0) {
                    modifiedNurseType = modifiedNurseType.substr(prefix.length());
                }

                // Map full nurse type names to abbreviations
                if (modifiedNurseType == "Nurse Assistant"){
                    modifiedNurseType = "NA";
                }
                else if (modifiedNurseType == "Registered Nurse"){
                    modifiedNurseType = "RN";
                }
                else if (modifiedNurseType == "Licensed Practical Nurse"){
                    modifiedNurseType = "LPN";
                }

                // Get available nurses of this department and nurse type
                auto& availableNurses = departmentNursesMap[department][modifiedNurseType];

                // Sort the available nurses based on their preference for the shift
                std::vector<Nurse> sortedNurses = availableNurses;
                std::sort(sortedNurses.begin(), sortedNurses.end(),
                          [shift](const Nurse& a, const Nurse& b) {
                    // Changed to sort in descending order of preference
                    return a.shiftPreferences[shift - 1] > b.shiftPreferences[shift - 1];
                });

                // Assign nursesNeeded nurses
                for (int i = 0; i < nursesNeeded; ++i) {
                    if (i < sortedNurses.size()) {
                        Nurse nurse = sortedNurses[i];
                        schedule[shift - 1].push_back(nurse);
                    } else {
                        // Not enough nurses, add fake nurse
                        schedule[shift - 1].push_back(fakeNurse);
                    }
                }
            }
        }
    }
    return schedule;
}

double evaluateFitness(const ShiftSchedule& schedule) {
    double satisfactionScore = 0.0;

    // For each shift
    for (int shift = 1; shift <= 42; ++shift) {
        const auto& shiftNurses = schedule[shift - 1];
        std::unordered_set<int> nurseIDsInShift; // To check for duplicates
        // For each nurse in the shift
        for (const auto& nurse : shiftNurses) {
            if (nurse.nurseNumber != -1) { // Ignore fake nurses
                int preference = nurse.shiftPreferences[shift - 1];
                satisfactionScore += preference;

                // Check if nurse is assigned multiple times in the same shift
                if (nurseIDsInShift.find(nurse.nurseNumber) != nurseIDsInShift.end()) {
                    satisfactionScore -= 5; // Penalty for duplicate assignment
                } else {
                    nurseIDsInShift.insert(nurse.nurseNumber);
                }
            } else {
                // Penalty for fake nurse
                satisfactionScore -= 5;
            }
        }
    }

    return satisfactionScore;
}

ShiftSchedule tournamentSelection(const std::vector<ShiftSchedule>& population, const std::vector<double>& fitnesses, std::mt19937& gen) {
    const int tournamentSize = 3;
    std::uniform_int_distribution<> dis(0, population.size() - 1);

    int bestIndex = dis(gen);
    double bestFitness = fitnesses[bestIndex];

    for (int i = 1; i < tournamentSize; ++i) {
        int idx = dis(gen);
        if (fitnesses[idx] > bestFitness) {
            bestIndex = idx;
            bestFitness = fitnesses[idx];
        }
    }
    return population[bestIndex];
}

void crossover(const ShiftSchedule& parent1, const ShiftSchedule& parent2, ShiftSchedule& child1, ShiftSchedule& child2, std::mt19937& gen) {
    // One-point crossover
    std::uniform_int_distribution<> dis(1, 41); // Positions to crossover between 1 and 41
    int crossoverPoint = dis(gen);

    for (int i = 0; i < 42; ++i) {
        if (i < crossoverPoint) {
            child1[i] = parent1[i];
            child2[i] = parent2[i];
        } else {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }
    }
}

void mutate(ShiftSchedule& schedule, std::mt19937& gen) {
    // Randomly select a shift
    std::uniform_int_distribution<> shiftDis(0, 41);
    int shiftIndex = shiftDis(gen);

    auto& shiftNurses = schedule[shiftIndex];

    if (shiftNurses.empty()) return;

    // Randomly select a nurse in the shift
    std::uniform_int_distribution<> nurseDis(0, shiftNurses.size() - 1);
    int nurseIndex = nurseDis(gen);

    Nurse& nurse = shiftNurses[nurseIndex];

    // Remove the nurse
    shiftNurses.erase(shiftNurses.begin() + nurseIndex);

    // Get list of available nurses of the same department and nurse type
    std::string department = nurse.department;
    std::string nurseType = nurse.nurseType;

    auto& availableNurses = departmentNursesMap[department][nurseType];

    // Randomly select a new nurse
    std::uniform_int_distribution<> availDis(0, availableNurses.size() - 1);
    Nurse newNurse = availableNurses[availDis(gen)];

    // Add the new nurse to the shift
    shiftNurses.push_back(newNurse);
}
