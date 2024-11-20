#include <vector>
#include <string>
#include <map>
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <ctime>
#include <numeric>  // Add this for std::iota

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
    double fitness;

    Schedule(std::vector<Nurse>& _nurses) : nurses(_nurses), fitness(0.0) {}
    
    // Add copy constructor
    Schedule(const Schedule& other) : nurses(other.nurses), assignments(other.assignments), fitness(other.fitness) {}
    
    // Add assignment operator
    Schedule& operator=(const Schedule& other) {
        if (this != &other) {
            assignments = other.assignments;
            fitness = other.fitness;
        }
        return *this;
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

    void createNurseTypeMap() {
        for (const auto& nurse : nurses) {
            nurseTypeMap[nurse.nurseType].push_back(nurse.id);
        }
    }

std::vector<std::unique_ptr<Schedule>> createInitialPopulation() {
        std::vector<std::unique_ptr<Schedule>> population;
        
        for (int i = 0; i < populationSize; i++) {
            auto schedule = std::make_unique<Schedule>(nurses);
            
            // Process one shift at a time
            for (int shift = 1; shift <= 42; shift++) {
                // Find all constraints for this shift number
                std::vector<ShiftConstraint*> shiftConstraints;
                for (auto& constraint : constraints) {
                    if (constraint.shift == shift) {
                        shiftConstraints.push_back(&constraint);
                    }
                }

                // For each shift, process each department's constraints
                for (auto* constraint : shiftConstraints) {
                    std::vector<int> shiftNurses;
                    
                    // Assign RNs for this department
                    std::vector<int> availableRNs;
                    for (int nurseId : nurseTypeMap["RN"]) {
                        if (nurses[nurseId-1].department == constraint->department) {
                            availableRNs.push_back(nurseId);
                        }
                    }
                    std::shuffle(availableRNs.begin(), availableRNs.end(), rng);
                    for (int j = 0; j < std::min(constraint->rnCount, (int)availableRNs.size()); j++) {
                        shiftNurses.push_back(availableRNs[j]);
                    }

                    // Assign LPNs for this department
                    std::vector<int> availableLPNs;
                    for (int nurseId : nurseTypeMap["LPN"]) {
                        if (nurses[nurseId-1].department == constraint->department) {
                            availableLPNs.push_back(nurseId);
                        }
                    }
                    std::shuffle(availableLPNs.begin(), availableLPNs.end(), rng);
                    for (int j = 0; j < std::min(constraint->lpnCount, (int)availableLPNs.size()); j++) {
                        shiftNurses.push_back(availableLPNs[j]);
                    }

                    // Assign NAs for this department
                    std::vector<int> availableNAs;
                    for (int nurseId : nurseTypeMap["NA"]) {
                        if (nurses[nurseId-1].department == constraint->department) {
                            availableNAs.push_back(nurseId);
                        }
                    }
                    std::shuffle(availableNAs.begin(), availableNAs.end(), rng);
                    for (int j = 0; j < std::min(constraint->naCount, (int)availableNAs.size()); j++) {
                        shiftNurses.push_back(availableNAs[j]);
                    }

                    // Store assignments using a compound key of shift and department
                    std::string key = std::to_string(shift) + "_" + constraint->department;
                    schedule->assignments[shift][constraint->department] = shiftNurses;
                }
            }
            
            population.push_back(std::move(schedule));
        }
        
        return population;
    }

double calculateFitness(Schedule* schedule) {
    double fitness = 0.0;
    int violations = 0;

    // Check constraints for each shift and department
    for (int shift = 1; shift <= 42; shift++) {
        for (const auto& constraint : constraints) {
            if (constraint.shift == shift) {
                const auto& deptAssignments = schedule->assignments[shift][constraint.department];
                
                // Count nurse types for this department
                int rnCount = 0, lpnCount = 0, naCount = 0;
                for (int nurseId : deptAssignments) {
                    const auto& nurseType = nurses[nurseId-1].nurseType;
                    if (nurseType == "RN") rnCount++;
                    else if (nurseType == "LPN") lpnCount++;
                    else if (nurseType == "NA") naCount++;
                }

                // Add violations for this department
                violations += std::abs(rnCount - constraint.rnCount);
                violations += std::abs(lpnCount - constraint.lpnCount);
                violations += std::abs(naCount - constraint.naCount);

                // Calculate preference score for this department
                for (int nurseId : deptAssignments) {
                    int preference = nurses[nurseId-1].preferences[shift-1];
                    fitness += (2.0 - preference);
                }
            }
        }
    }

    // Combine scores
    return fitness - (violations * 1000.0);
}

    std::pair<Schedule*, Schedule*> selectParents(const std::vector<std::unique_ptr<Schedule>>& population) {
        const int tournamentSize = 5;
        std::vector<int> indices(population.size());
        std::iota(indices.begin(), indices.end(), 0);
        
        // First tournament
        std::shuffle(indices.begin(), indices.end(), rng);
        Schedule* parent1 = nullptr;
        double bestFitness1 = -std::numeric_limits<double>::infinity();
        
        for (int i = 0; i < tournamentSize; i++) {
            if (population[indices[i]]->fitness > bestFitness1) {
                bestFitness1 = population[indices[i]]->fitness;
                parent1 = population[indices[i]].get();
            }
        }

        // Second tournament
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

    // Take first part from parent1
    for (int shift = 1; shift <= crossoverPoint; shift++) {
        for (const auto& dept : parent1->assignments[shift]) {
            child->assignments[shift][dept.first] = dept.second;
        }
    }

    // Take second part from parent2
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

                // Get available nurses for this department and type
                std::vector<int> availableNurses;
                for (int nurseId : nurseTypeMap[selectedType]) {
                    if (nurses[nurseId-1].department == constraint.department) {
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
            // Use std::pair instead of structured binding
            std::pair<Schedule*, Schedule*> parents = selectParents(population);
            Schedule* parent1 = parents.first;
            Schedule* parent2 = parents.second;
            
            auto child = crossover(parent1, parent2);
            mutate(child.get());
            child->fitness = calculateFitness(child.get());

            // Update best schedule if necessary
            if (child->fitness > bestSchedule->fitness) {
                bestSchedule = std::make_unique<Schedule>(*child);
            }

            newPopulation.push_back(std::move(child));
        }

        // Replace old population
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

// Helper functions to parse CSV data
std::vector<Nurse> parseNurseData(const std::string& filename) {
    std::vector<Nurse> nurses;
    std::ifstream file(filename);
    std::string line;
    
    // Skip header
    std::getline(file, line);
    
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
    
    // Skip header
    std::getline(file, line);
    
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

// Example usage
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
        auto constraints = parseConstraintData("NurseContraints.csv");
        log("Loaded " + std::to_string(constraints.size()) + " constraints.");

        // Create and run scheduler
        log("Creating scheduler...");
        NurseScheduler scheduler(nurses, constraints, 100, 100);
        
        log("Running genetic algorithm...");
        Schedule* bestSchedule = scheduler.run();

        // Print results
        log("\nBest schedule fitness: " + std::to_string(bestSchedule->fitness));

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
        }
        ss << "Nurse counts - RN: " << rn << ", LPN: " << lpn << ", NA: " << na << "\n";
        
        // List all nurses in this department
        ss << "Assigned nurses:\n";
        for (int nurseId : nurseIds) {
            const auto& nurse = nurses[nurseId-1];
            ss << "  - " << nurse.name << " (ID: " << nurse.id 
               << ", Type: " << nurse.nurseType 
               << ", Preference: " << nurse.preferences[shift-1] << ")\n";
        }
        ss << "\n";
    }
    log(ss.str());
}

log("\n=== SCHEDULE SUMMARY ===");
double totalPreference = 0;
int totalAssignments = 0;
std::map<std::string, int> typeAssignments;

for (const auto& shiftAssignment : bestSchedule->assignments) {
    for (const auto& deptAssignment : shiftAssignment.second) {
        for (int nurseId : deptAssignment.second) {  // Access the vector of nurse IDs
            totalPreference += nurses[nurseId-1].preferences[shiftAssignment.first-1];
            totalAssignments++;
            typeAssignments[nurses[nurseId-1].nurseType]++;
        }
    }
}

log("Average preference score: " + 
    std::to_string(totalPreference / totalAssignments));
log("Total assignments: " + std::to_string(totalAssignments));
log("Assignments by nurse type:");
for (const auto& type : typeAssignments) {
    log("  " + type.first + ": " + std::to_string(type.second));
}

        // Clean up
        delete bestSchedule;
        outFile.close();
        
        log("\nProgram completed successfully. Results written to schedule_output.txt");
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
