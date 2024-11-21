#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <ctime>
#include <stdexcept>
#include <numeric>
#include "CSVParser.h"
#include "NurseList.h"
#include "NurseFunctions.h"

// ShiftConstraint Structure
struct ShiftConstraint {
    std::string department;
    int shift;
    int rnCount;
    int lpnCount;
    int naCount;

    ShiftConstraint(const std::string& _dept, int _shift, int _rn, int _lpn, int _na)
        : department(_dept), shift(_shift), rnCount(_rn), lpnCount(_lpn), naCount(_na) {}
};

// Schedule Class
class Schedule {
public:
    std::vector<Nurse>& nurses;
    std::map<int, std::map<std::string, std::vector<int>>> assignments; // shift -> department -> nurse IDs
    double fitness;

    Schedule(std::vector<Nurse>& _nurses) : nurses(_nurses), fitness(0.0) {}

    // Copy Constructor
    Schedule(const Schedule& other)
        : nurses(other.nurses), assignments(other.assignments), fitness(other.fitness) {}

    // Assignment Operator
    Schedule& operator=(const Schedule& other) {
        if (this != &other) {
            assignments = other.assignments;
            fitness = other.fitness;
        }
        return *this;
    }
};

// NurseScheduler Class
class NurseScheduler {
private:
    std::vector<Nurse>& nurses;
    std::vector<ShiftConstraint>& constraints;
    int populationSize;
    int generations;
    std::map<std::string, std::vector<int>> nurseTypeMap;
    std::mt19937 rng;

    void createNurseTypeMap() {
        for (const auto& nurse : nurses) {
            nurseTypeMap[nurse.nurseType].push_back(nurse.nurseNumber);
        }
    }

    std::vector<std::unique_ptr<Schedule>> createInitialPopulation() {
        std::vector<std::unique_ptr<Schedule>> population;

        for (int i = 0; i < populationSize; i++) {
            auto schedule = std::make_unique<Schedule>(nurses);

            for (int shift = 1; shift <= 42; shift++) {
                for (auto& constraint : constraints) {
                    if (constraint.shift == shift) {
                        std::vector<int> shiftNurses;

                        // Assign RNs
                        std::vector<int> availableRNs;
                        for (int nurseId : nurseTypeMap["RN"]) {
                            if (nurses[nurseId - 1].department == constraint.department) {
                                availableRNs.push_back(nurseId);
                            }
                        }
                        std::shuffle(availableRNs.begin(), availableRNs.end(), rng);
                        shiftNurses.insert(shiftNurses.end(),
                                           availableRNs.begin(),
                                           availableRNs.begin() + std::min(constraint.rnCount, (int)availableRNs.size()));

                        // Assign LPNs
                        std::vector<int> availableLPNs;
                        for (int nurseId : nurseTypeMap["LPN"]) {
                            if (nurses[nurseId - 1].department == constraint.department) {
                                availableLPNs.push_back(nurseId);
                            }
                        }
                        std::shuffle(availableLPNs.begin(), availableLPNs.end(), rng);
                        shiftNurses.insert(shiftNurses.end(),
                                           availableLPNs.begin(),
                                           availableLPNs.begin() + std::min(constraint.lpnCount, (int)availableLPNs.size()));

                        // Assign NAs
                        std::vector<int> availableNAs;
                        for (int nurseId : nurseTypeMap["NA"]) {
                            if (nurses[nurseId - 1].department == constraint.department) {
                                availableNAs.push_back(nurseId);
                            }
                        }
                        std::shuffle(availableNAs.begin(), availableNAs.end(), rng);
                        shiftNurses.insert(shiftNurses.end(),
                                           availableNAs.begin(),
                                           availableNAs.begin() + std::min(constraint.naCount, (int)availableNAs.size()));

                        schedule->assignments[shift][constraint.department] = shiftNurses;
                    }
                }
            }
            population.push_back(std::move(schedule));
        }

        return population;
    }

    double calculateFitness(Schedule* schedule) {
        double fitness = 0.0;
        int violations = 0;

        for (int shift = 1; shift <= 42; shift++) {
            for (const auto& constraint : constraints) {
                if (constraint.shift == shift) {
                    const auto& deptAssignments = schedule->assignments[shift][constraint.department];

                    int rnCount = 0, lpnCount = 0, naCount = 0;
                    for (int nurseId : deptAssignments) {
                        const auto& nurseType = nurses[nurseId - 1].nurseType;
                        if (nurseType == "RN") rnCount++;
                        else if (nurseType == "LPN") lpnCount++;
                        else if (nurseType == "NA") naCount++;
                    }

                    violations += std::abs(rnCount - constraint.rnCount);
                    violations += std::abs(lpnCount - constraint.lpnCount);
                    violations += std::abs(naCount - constraint.naCount);

                    for (int nurseId : deptAssignments) {
                        int preference = nurses[nurseId - 1].shiftPreferences[shift - 1];
                        fitness += (2.0 - preference);
                    }
                }
            }
        }

        return fitness - (violations * 1000.0);
    }

    std::pair<Schedule*, Schedule*> selectParents(const std::vector<std::unique_ptr<Schedule>>& population) {
        const int tournamentSize = 5;
        std::vector<int> indices(population.size());
        std::iota(indices.begin(), indices.end(), 0);

        // Select first parent
        std::shuffle(indices.begin(), indices.end(), rng);
        Schedule* parent1 = nullptr;
        double bestFitness1 = -std::numeric_limits<double>::infinity();
        for (int i = 0; i < tournamentSize; i++) {
            if (population[indices[i]]->fitness > bestFitness1) {
                bestFitness1 = population[indices[i]]->fitness;
                parent1 = population[indices[i]].get();
            }
        }

        // Select second parent
        std::shuffle(indices.begin(), indices.end(), rng);
        Schedule* parent2 = nullptr;
        double bestFitness2 = -std::numeric_limits<double>::infinity();
        for (int i = 0; i < tournamentSize; i++) {
            if (population[indices[i]]->fitness > bestFitness2) {
                bestFitness2 = population[indices[i]]->fitness;
                parent2 = population[indices[i]].get();
            }
        }

        return {parent1, parent2};
    }

    std::unique_ptr<Schedule> crossover(Schedule* parent1, Schedule* parent2) {
        auto child = std::make_unique<Schedule>(nurses);
        std::uniform_int_distribution<int> dist(1, 42);
        int crossoverPoint = dist(rng);

        for (int shift = 1; shift <= crossoverPoint; shift++) {
            for (const auto& dept : parent1->assignments[shift]) {
                child->assignments[shift][dept.first] = dept.second;
            }
        }

        for (int shift = crossoverPoint + 1; shift <= 42; shift++) {
            for (const auto& dept : parent2->assignments[shift]) {
                child->assignments[shift][dept.first] = dept.second;
            }
        }

        return child;
    }

    void mutate(Schedule* schedule, double mutationRate = 0.1) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int shift = 1; shift <= 42; shift++) {
            for (auto& constraint : constraints) {
                if (constraint.shift == shift && dist(rng) < mutationRate) {
                    std::vector<std::string> nurseTypes = {"RN", "LPN", "NA"};
                    std::string selectedType = nurseTypes[std::uniform_int_distribution<int>(0, 2)(rng)];

                    std::vector<int> availableNurses;
                    for (int nurseId : nurseTypeMap[selectedType]) {
                        if (nurses[nurseId - 1].department == constraint.department) {
                            availableNurses.push_back(nurseId);
                        }
                    }

                    if (!availableNurses.empty() && schedule->assignments[shift].count(constraint.department) > 0) {
                        auto& currentNurses = schedule->assignments[shift][constraint.department];
                        for (size_t i = 0; i < currentNurses.size(); i++) {
                            if (nurses[currentNurses[i] - 1].nurseType == selectedType) {
                                int newNurse = availableNurses[std::uniform_int_distribution<int>(
                                    0, availableNurses.size() - 1)(rng)];
                                currentNurses[i] = newNurse;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

public:
    NurseScheduler(std::vector<Nurse>& _nurses, std::vector<ShiftConstraint>& _constraints,
                   int _populationSize = 100, int _generations = 100)
        : nurses(_nurses), constraints(_constraints),
          populationSize(_populationSize), generations(_generations) {
        rng.seed(std::time(nullptr));
        createNurseTypeMap();
    }

    Schedule* run() {
        // Create initial population
        auto population = createInitialPopulation();

        // Calculate initial fitness
        for (auto& schedule : population) {
            schedule->fitness = calculateFitness(schedule.get());
        }

        // Best schedule found so far
        std::unique_ptr<Schedule> bestSchedule = std::make_unique<Schedule>(nurses);
        bestSchedule->fitness = -std::numeric_limits<double>::infinity();

        // Main evolution loop
        for (int gen = 0; gen < generations; gen++) {
            std::vector<std::unique_ptr<Schedule>> newPopulation;

            // Create new generation
            while (newPopulation.size() < static_cast<size_t>(populationSize)) {
                auto [parent1, parent2] = selectParents(population);

                auto child = crossover(parent1, parent2);
                mutate(child.get());
                child->fitness = calculateFitness(child.get());

                if (child->fitness > bestSchedule->fitness) {
                    bestSchedule = std::make_unique<Schedule>(*child);
                }

                newPopulation.push_back(std::move(child));
            }

            population = std::move(newPopulation);

            // Print progress
            if (gen % 10 == 0) {
                double bestFitness = -std::numeric_limits<double>::infinity();
                for (const auto& schedule : population) {
                    bestFitness = std::max(bestFitness, schedule->fitness);
                }
                std::cout << "Generation " << gen << ": Best Fitness = " << bestFitness << std::endl;
            }
        }

        return bestSchedule.release();
    }
};
int main() {
    try {
        // Open output file for logging results
        std::ofstream outFile("schedule_output.txt");
        if (!outFile.is_open()) {
            throw std::runtime_error("Could not create output file");
        }

        auto log = [&outFile](const std::string& message) {
            std::cout << message << std::endl;
            outFile << message << std::endl;
        };

        log("Starting nurse scheduling program...");

        // Parse nurse data
        log("Parsing nurse data from Nurse_List_Department_Included.csv...");
        parseNursesCSV("Nurse_List_Department_Included.csv");
        log("Nurse data parsing complete.");

        // Parse constraint data
        log("Parsing constraints data from NurseContraints.csv...");
        parseConstraintsCSV("NurseContraints.csv");
        log("Constraint data parsing complete.");

        // Populate local vectors from global maps
        std::vector<Nurse> nurses;

        // Convert departmentNursesMap to a vector of Nurse objects
        for (const auto& [dept, typeMap] : departmentNursesMap) {
            for (const auto& [type, nurseList] : typeMap) {
                for (const auto& nurse : nurseList) {
                    Nurse newNurse;
                    newNurse.fullName = nurse.fullName;
                    newNurse.nurseNumber = nurse.nurseNumber;
                    newNurse.nurseType = nurse.nurseType;
                    newNurse.department = nurse.department;
                    newNurse.shiftPreferences = nurse.shiftPreferences;
                    newNurse.scheduledShifts = nurse.scheduledShifts;

                    nurses.push_back(newNurse); // Add the new Nurse object
                }
            }
        }

        // Populate constraints from constraintsMap
        std::vector<ShiftConstraint> constraints;
        for (const auto& [shift, deptMap] : constraintsMap) {
            for (const auto& [department, typeMap] : deptMap) {
                int rnCount = typeMap.count("RN") ? typeMap.at("RN") : 0;
                int lpnCount = typeMap.count("LPN") ? typeMap.at("LPN") : 0;
                int naCount = typeMap.count("NA") ? typeMap.at("NA") : 0;

                constraints.emplace_back(department, shift, rnCount, lpnCount, naCount);
            }
        }

        log("Creating scheduler...");
        NurseScheduler scheduler(nurses, constraints, 100, 100);

        log("Running genetic algorithm...");
        Schedule* bestSchedule = scheduler.run();

        log("\nBest schedule fitness: " + std::to_string(bestSchedule->fitness));

        // Print detailed schedule
        log("\n=== DETAILED SCHEDULE ===");
        for (int shift = 1; shift <= 42; ++shift) {
            const auto& shiftAssignments = bestSchedule->assignments[shift];
            std::stringstream ss;
            ss << "\nShift " << shift << ":\n";

            for (const auto& [department, nurseIds] : shiftAssignments) {
                ss << "Department: " << department << "\n";

                int rn = 0, lpn = 0, na = 0;
                for (int nurseId : nurseIds) {
                    const auto& nurse = nurses[nurseId - 1];
                    if (nurse.nurseType == "RN") rn++;
                    else if (nurse.nurseType == "LPN") lpn++;
                    else if (nurse.nurseType == "NA") na++;
                }
                ss << "Nurse counts - RN: " << rn << ", LPN: " << lpn << ", NA: " << na << "\n";

                ss << "Assigned nurses:\n";
                for (int nurseId : nurseIds) {
                    const auto& nurse = nurses[nurseId - 1];
                    int preference = nurse.shiftPreferences[shift - 1]; 
                    ss << "  - " << nurse.fullName << " (ID: " << nurse.nurseNumber 
                       << ", Type: " << nurse.nurseType << ", Preference: " << preference << ")\n";
                }
                ss << "\n";
            }
            log(ss.str());
        }

        delete bestSchedule;
        outFile.close();
        log("Program completed successfully.");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

