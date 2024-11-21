#include <vector>
#include <string>
#include <map>
#include <set>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <ctime>
#include <numeric>
#include <limits>
#include <stdexcept>

// Structures to hold data
struct Nurse {
    int id;
    std::string name;
    std::string nurseType;
    std::string department;
    std::vector<int> preferences;

    Nurse(int _id, const std::string& _name, const std::string& _type, 
          const std::string& _dept, const std::vector<int>& _prefs)
        : id(_id), name(_name), nurseType(_type), department(_dept), preferences(_prefs) {}
};

struct ShiftConstraint {
    std::string department;
    int shift;
    int rnCount;
    int lpnCount;
    int naCount;

    ShiftConstraint(const std::string& _dept, int _shift, int _rn, int _lpn, int _na)
        : department(_dept), shift(_shift), rnCount(_rn), lpnCount(_lpn), naCount(_na) {}
};

class Schedule {
public:
    std::vector<Nurse>& nurses;
    std::map<int, std::map<std::string, std::vector<int>>> assignments; // shift -> department -> nurse IDs
    std::map<int, std::set<int>> nurseShiftAssignments;  // shift -> set of nurse IDs
    double fitness;
    int totalSatisfactionScore;  // Changed from satisfactionScore

    Schedule(std::vector<Nurse>& _nurses) 
        : nurses(_nurses), fitness(0.0), totalSatisfactionScore(0) {}
    
    Schedule(const Schedule& other) 
        : nurses(other.nurses), 
          assignments(other.assignments), 
          fitness(other.fitness), 
          totalSatisfactionScore(other.totalSatisfactionScore),
          nurseShiftAssignments(other.nurseShiftAssignments) {}
    
    Schedule& operator=(const Schedule& other) {
        if (this != &other) {
            assignments = other.assignments;
            fitness = other.fitness;
            totalSatisfactionScore = other.totalSatisfactionScore;
            nurseShiftAssignments = other.nurseShiftAssignments;
        }
        return *this;
    }

    bool isNurseAssignedToShift(int nurseId, int shift) const {
        auto it = nurseShiftAssignments.find(shift);
        return it != nurseShiftAssignments.end() && it->second.count(nurseId) > 0;
    }

    void addNurseAssignment(int nurseId, int shift) {
        nurseShiftAssignments[shift].insert(nurseId);
    }

    void clearAssignments() {
        nurseShiftAssignments.clear();
    }
};

class NurseScheduler {
private:
    std::vector<Nurse>& nurses;
    std::vector<ShiftConstraint>& constraints;
    int populationSize;
    int generations;
    std::map<std::string, std::vector<int>> nurseTypeMap;
    std::mt19937 rng;
    const double MUTATION_RATE = 0.1;
    const int TOURNAMENT_SIZE = 5;

    void createNurseTypeMap() {
        for (const auto& nurse : nurses) {
            nurseTypeMap[nurse.nurseType].push_back(nurse.id);
        }
    }

    double calculateFitness(Schedule* schedule) {
        double fitness = 0.0;
        int violations = 0;
        schedule->totalSatisfactionScore = 0;  // Reset total satisfaction score
        schedule->clearAssignments();

        for (int shift = 1; shift <= 42; shift++) {
            std::set<int> nursesInShift;
            
            for (const auto& constraint : constraints) {
                if (constraint.shift == shift) {
                    const auto& deptAssignments = schedule->assignments[shift][constraint.department];
                    
                    int rnCount = 0, lpnCount = 0, naCount = 0;
                    for (int nurseId : deptAssignments) {
                        if (nursesInShift.count(nurseId) > 0) {
                            violations += 1000;
                            continue;
                        }
                        nursesInShift.insert(nurseId);
                        schedule->addNurseAssignment(nurseId, shift);

                        const auto& nurse = nurses[nurseId-1];
                        if (nurse.nurseType == "RN") rnCount++;
                        else if (nurse.nurseType == "LPN") lpnCount++;
                        else if (nurse.nurseType == "NA") naCount++;

                        int preference = nurse.preferences[shift-1];
                        schedule->totalSatisfactionScore += preference;  // Direct preference addition
                        
                        // For fitness calculation (separate from satisfaction score)
                        switch(preference) {
                            case 0: fitness += 100.0; break;
                            case 1: fitness += 10.0; break;
                            case 2: fitness += 1.0; break;
                        }
                    }

                    violations += std::abs(rnCount - constraint.rnCount) * 500;
                    violations += std::abs(lpnCount - constraint.lpnCount) * 500;
                    violations += std::abs(naCount - constraint.naCount) * 500;
                }
            }
        }

        return fitness - violations;
    }

    std::unique_ptr<Schedule> createValidSchedule() {
        auto schedule = std::make_unique<Schedule>(nurses);
        
        for (int shift = 1; shift <= 42; shift++) {
            for (auto& constraint : constraints) {
                if (constraint.shift == shift) {
                    std::vector<int> shiftNurses;
                    
                    // Helper function to get available nurses of a specific type
                    auto getAvailableNurses = [&](const std::string& nurseType, int count) {
                        std::vector<int> available;
                        std::vector<int> preferredNurses;
                        std::vector<int> neutralNurses;
                        std::vector<int> unwillingNurses;
                        
                        for (int nurseId : nurseTypeMap[nurseType]) {
                            if (nurses[nurseId-1].department == constraint.department && 
                                !schedule->isNurseAssignedToShift(nurseId, shift)) {
                                
                                int preference = nurses[nurseId-1].preferences[shift-1];
                                if (preference == 0)
                                    preferredNurses.push_back(nurseId);
                                else if (preference == 1)
                                    neutralNurses.push_back(nurseId);
                                else
                                    unwillingNurses.push_back(nurseId);
                            }
                        }
                        
                        std::shuffle(preferredNurses.begin(), preferredNurses.end(), rng);
                        std::shuffle(neutralNurses.begin(), neutralNurses.end(), rng);
                        std::shuffle(unwillingNurses.begin(), unwillingNurses.end(), rng);
                        
                        available.insert(available.end(), preferredNurses.begin(), preferredNurses.end());
                        available.insert(available.end(), neutralNurses.begin(), neutralNurses.end());
                        available.insert(available.end(), unwillingNurses.begin(), unwillingNurses.end());
                        
                        return available;
                    };

                    // Assign nurses based on type
                    auto assignNurses = [&](const std::string& nurseType, int count) {
                        auto available = getAvailableNurses(nurseType, count);
                        for (int i = 0; i < std::min(count, (int)available.size()); i++) {
                            int nurseId = available[i];
                            shiftNurses.push_back(nurseId);
                            schedule->addNurseAssignment(nurseId, shift);
                            int preference = nurses[nurseId-1].preferences[shift-1];
                            schedule->totalSatisfactionScore += preference;  // Add preference directly
                        }
                    };

                    assignNurses("RN", constraint.rnCount);
                    assignNurses("LPN", constraint.lpnCount);
                    assignNurses("NA", constraint.naCount);

                    schedule->assignments[shift][constraint.department] = shiftNurses;
                }
            }
        }
        
        return schedule;
    }

    std::vector<std::unique_ptr<Schedule>> createInitialPopulation() {
        std::vector<std::unique_ptr<Schedule>> population;
        for (int i = 0; i < populationSize; i++) {
            auto schedule = createValidSchedule();
            schedule->fitness = calculateFitness(schedule.get());
            population.push_back(std::move(schedule));
        }
        return population;
    }

    std::pair<Schedule*, Schedule*> selectParents(const std::vector<std::unique_ptr<Schedule>>& population) {
        std::vector<int> indices(population.size());
        std::iota(indices.begin(), indices.end(), 0);
        
        auto selectParent = [&]() -> Schedule* {
            std::shuffle(indices.begin(), indices.end(), rng);
            Schedule* parent = nullptr;
            double bestFitness = -std::numeric_limits<double>::infinity();
            
            for (int i = 0; i < TOURNAMENT_SIZE; i++) {
                if (population[indices[i]]->fitness > bestFitness) {
                    bestFitness = population[indices[i]]->fitness;
                    parent = population[indices[i]].get();
                }
            }
            return parent;
        };

        return {selectParent(), selectParent()};
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

    void mutate(Schedule* schedule) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        
        for (int shift = 1; shift <= 42; shift++) {
            for (auto& constraint : constraints) {
                if (constraint.shift == shift && dist(rng) < MUTATION_RATE) {
                    std::vector<std::string> nurseTypes = {"RN", "LPN", "NA"};
                    std::string selectedType = nurseTypes[std::uniform_int_distribution<int>(0, 2)(rng)];

                    std::vector<int> availableNurses;
                    for (int nurseId : nurseTypeMap[selectedType]) {
                        if (nurses[nurseId-1].department == constraint.department && 
                            !schedule->isNurseAssignedToShift(nurseId, shift)) {
                            availableNurses.push_back(nurseId);
                        }
                    }

                    if (!availableNurses.empty() && schedule->assignments[shift].count(constraint.department) > 0) {
                        auto& currentNurses = schedule->assignments[shift][constraint.department];
                        for (size_t i = 0; i < currentNurses.size(); i++) {
                            if (nurses[currentNurses[i]-1].nurseType == selectedType) {
                                int newNurse = availableNurses[std::uniform_int_distribution<int>(
                                    0, availableNurses.size()-1)(rng)];
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
                   int _populationSize = 200, int _generations = 200)
        : nurses(_nurses), constraints(_constraints),
          populationSize(_populationSize), generations(_generations) {
        rng.seed(std::time(nullptr));
        createNurseTypeMap();
    }

    Schedule* run() {
        auto population = createInitialPopulation();
        
        std::unique_ptr<Schedule> bestSchedule = std::make_unique<Schedule>(nurses);
        bestSchedule->fitness = -std::numeric_limits<double>::infinity();

        for (int gen = 0; gen < generations; gen++) {
            std::vector<std::unique_ptr<Schedule>> newPopulation;

            while (newPopulation.size() < static_cast<size_t>(populationSize)) {
                auto parents = selectParents(population);
                auto child = crossover(parents.first, parents.second);
                mutate(child.get());
                child->fitness = calculateFitness(child.get());

                if (child->fitness > bestSchedule->fitness) {
                    bestSchedule = std::make_unique<Schedule>(*child);
                }

                newPopulation.push_back(std::move(child));
            }

            population = std::move(newPopulation);

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

// Helper functions to parse CSV data
std::vector<Nurse> parseNurseData(const std::string& filename) {
    std::vector<Nurse> nurses;
    std::ifstream file(filename);
    std::string line;
    
    std::getline(file, line); // Skip header
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> row;
        
        while (std::getline(ss, item, ',')) {
            row.push_back(item);
        }
        
        std::vector<int> preferences;
        for (size_t i = 4; i < row.size(); i++) {
            preferences.push_back(std::stoi(row[i]));
        }
        
        nurses.emplace_back(std::stoi(row[1]), row[0], row[2], row[3], preferences);
    }
    
    return nurses;
}

std::vector<ShiftConstraint> parseConstraintData(const std::string& filename) {
    std::vector<ShiftConstraint> constraints;
    std::ifstream file(filename);
    std::string line;
    
    std::getline(file, line); // Skip header
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> row;
        
        while (std::getline(ss, item, ',')) {
            row.push_back(item);
        }
        
        constraints.emplace_back(row[0], std::stoi(row[1]), std::stoi(row[2]), 
                               std::stoi(row[3]), std::stoi(row[4]));
    }
    
    return constraints;
}

int main() {
    try {
        // Create output file
        std::ofstream outFile("schedule_output.txt");
        if (!outFile.is_open()) {
            throw std::runtime_error("Could not create output file");
        }

        // Helper function to write to both console and file
        auto log = [&outFile](const std::string& message) {
            std::cout << message << std::endl;
            outFile << message << std::endl;
        };

        log("Starting nurse scheduling program...");
        
        // Load data
        log("Loading nurse data...");
        auto nurses = parseNurseData("Nurse_List_Department_Included.csv");
        log("Loaded " + std::to_string(nurses.size()) + " nurses.");

        log("Loading constraint data...");
        auto constraints = parseConstraintData("NurseConstraints.csv");
        log("Loaded " + std::to_string(constraints.size()) + " constraints.");

        // Create and run scheduler
        log("Creating scheduler...");
        NurseScheduler scheduler(nurses, constraints, 200, 200);
        
        log("Running genetic algorithm...");
        Schedule* bestSchedule = scheduler.run();

        // Print results
        log("\nBest schedule fitness: " + std::to_string(bestSchedule->fitness));
        log("Total satisfaction score (lower is better): " + std::to_string(bestSchedule->totalSatisfactionScore));

        // Calculate detailed preference distribution
        int pref0 = 0, pref1 = 0, pref2 = 0;
        int totalAssignments = 0;
        
        // Print detailed schedule
        log("\n=== DETAILED SCHEDULE ===");
        for (int shift = 1; shift <= 42; shift++) {
            const auto& shiftAssignments = bestSchedule->assignments[shift];
            std::stringstream ss;
            ss << "\nShift " << shift << ":\n";
            
            for (const auto& deptAssignment : shiftAssignments) {
                const std::string& department = deptAssignment.first;
                const std::vector<int>& nurseIds = deptAssignment.second;
                
                ss << "Department: " << department << "\n";
                
                // Count nurse types in this department
                int rn = 0, lpn = 0, na = 0;
                for (int nurseId : nurseIds) {
                    const auto& nurse = nurses[nurseId-1];
                    if (nurse.nurseType == "RN") rn++;
                    else if (nurse.nurseType == "LPN") lpn++;
                    else if (nurse.nurseType == "NA") na++;

                    // Count preferences
                    int preference = nurse.preferences[shift-1];
                    if (preference == 0) pref0++;
                    else if (preference == 1) pref1++;
                    else pref2++;
                    totalAssignments++;
                }
                ss << "Nurse counts - RN: " << rn << ", LPN: " << lpn << ", NA: " << na << "\n";
                
                // List all nurses in this department
                ss << "Assigned nurses:\n";
                for (int nurseId : nurseIds) {
                    const auto& nurse = nurses[nurseId-1];
                    ss << "  - " << nurse.name << " (ID: " << nurse.id 
                       << ", Type: " << nurse.nurseType 
                       << ", Preference: " << nurse.preferences[shift-1] 
                       << ")\n";
                }
                ss << "\n";
            }
            log(ss.str());
        }

        // Print preference distribution
        log("\nPreference Distribution:");
        log("Preference 0 (Preferred): " + std::to_string(pref0) + " assignments (" + 
            std::to_string(static_cast<double>(pref0) * 100 / totalAssignments) + "%)");
        log("Preference 1 (Neutral): " + std::to_string(pref1) + " assignments (" + 
            std::to_string(static_cast<double>(pref1) * 100 / totalAssignments) + "%)");
        log("Preference 2 (Not Preferred): " + std::to_string(pref2) + " assignments (" + 
            std::to_string(static_cast<double>(pref2) * 100 / totalAssignments) + "%)");

        // Print department satisfaction scores
        std::map<std::string, std::pair<double, int>> departmentStats;
        for (const auto& shiftAssignment : bestSchedule->assignments) {
            for (const auto& deptAssignment : shiftAssignment.second) {
                for (int nurseId : deptAssignment.second) {
                    const auto& nurse = nurses[nurseId-1];
                    departmentStats[deptAssignment.first].first += nurse.preferences[shiftAssignment.first-1];
                    departmentStats[deptAssignment.first].second++;
                }
            }
        }

        log("\nDepartment Statistics:");
        for (const auto& dept : departmentStats) {
            double avgPreference = dept.second.first / dept.second.second;
            log(dept.first + ": Average preference = " + std::to_string(avgPreference) + 
                " (Total assignments: " + std::to_string(dept.second.second) + ")");
        }

        // Analyze nurse workload distribution
        std::map<int, int> nurseAssignmentCount;
        for (const auto& shiftAssignment : bestSchedule->assignments) {
            for (const auto& deptAssignment : shiftAssignment.second) {
                for (int nurseId : deptAssignment.second) {
                    nurseAssignmentCount[nurseId]++;
                }
            }
        }

        std::vector<int> assignmentCounts;
        for (const auto& count : nurseAssignmentCount) {
            assignmentCounts.push_back(count.second);
        }
        
        std::sort(assignmentCounts.begin(), assignmentCounts.end());
        double median = assignmentCounts[assignmentCounts.size() / 2];
        double mean = std::accumulate(assignmentCounts.begin(), assignmentCounts.end(), 0.0) / 
                     assignmentCounts.size();
        
        log("\nNurse Workload Distribution:");
        log("Minimum assignments: " + std::to_string(assignmentCounts.front()));
        log("Maximum assignments: " + std::to_string(assignmentCounts.back()));
        log("Median assignments: " + std::to_string(median));
        log("Mean assignments: " + std::to_string(mean));

        // Print final satisfaction score
        log("\nFINAL SCORES:");
        log("Total satisfaction score (lower is better): " + std::to_string(bestSchedule->totalSatisfactionScore));
        log("Average satisfaction per assignment: " + 
            std::to_string(static_cast<double>(bestSchedule->totalSatisfactionScore) / totalAssignments));

        // Clean up
        delete bestSchedule;
        outFile.close();
        
        log("\nProgram completed successfully. Results written to schedule_output.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
