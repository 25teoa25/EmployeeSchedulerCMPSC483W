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
