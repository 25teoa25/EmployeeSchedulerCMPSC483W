#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

const int NUM_NURSES = 1000;
const int NUM_SHIFTS = 42;
const int POPULATION_SIZE = 100;
const int MAX_GENERATIONS = 500;
const double MUTATION_RATE = 0.05;

// Nurse preference (0: can work, 1: prefer not, 2: doesn't want to work)
int nurse_preferences[NUM_NURSES][NUM_SHIFTS]; // Filled from the CSV

// Declare debug_print_preferences
void debug_print_preferences(int num_nurses_to_print = 5);

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
        int column_index = 0; // Track column index to skip the first 4

        while (getline(ss, value, ',')) {
            if (column_index >= 4) {  // Start reading shift preferences after 4 columns
                if (shift_index < NUM_SHIFTS) {
                    // Convert value to integer and store in the array
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

    // Debug print call
    debug_print_preferences();
    cout << "CSV file successfully read and loaded into nurse_preferences array.\n";
}

void debug_print_preferences(int num_nurses_to_print) {
    cout << "Printing first " << num_nurses_to_print << " nurses' preferences:" << endl;
    for (int i = 0; i < num_nurses_to_print; i++) {
        cout << "Nurse " << i + 1 << ": ";
        for (int j = 0; j < NUM_SHIFTS; j++) {
            cout << nurse_preferences[i][j] << " ";
        }
        cout << endl;
    }
}

// Chromosome structure representing a schedule
struct Chromosome {
    vector<int> shifts; // 42 shifts, each element contains a nurse ID
    int fitness; // Fitness value of the chromosome

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
            vector<int> least_preferred_nurses;

            // Group nurses based on their preferences for the current shift
            for (int nurse = 0; nurse < NUM_NURSES; ++nurse) {
                if (nurse_preferences[nurse][i] == 0) {
                    preferred_nurses.push_back(nurse);
                }
                else if (nurse_preferences[nurse][i] == 1) {
                    fallback_nurses.push_back(nurse);
                }
                else if (nurse_preferences[nurse][i] == 2) {
                    least_preferred_nurses.push_back(nurse);
                }
            }

            // Randomly assign a nurse with some probability of choosing less optimal nurses
            double random_choice = (double)rand() / RAND_MAX;
            if (!preferred_nurses.empty() && random_choice < 0.6) {
                // 60% chance to pick from preferred nurses (Preference: 0)
                uniform_int_distribution<> distr(0, preferred_nurses.size() - 1);
                chromosome.shifts[i] = preferred_nurses[distr(gen)];
            }
            else if (!fallback_nurses.empty() && random_choice < 0.9) {
                // 30% chance to pick from fallback nurses (Preference: 1)
                uniform_int_distribution<> distr(0, fallback_nurses.size() - 1);
                chromosome.shifts[i] = fallback_nurses[distr(gen)];
            }
            else if (!least_preferred_nurses.empty()) {
                // 10% chance to pick from least preferred nurses (Preference: 2)
                uniform_int_distribution<> distr(0, least_preferred_nurses.size() - 1);
                chromosome.shifts[i] = least_preferred_nurses[distr(gen)];
            }
            else {
                chromosome.shifts[i] = gen() % NUM_NURSES; // Random fallback in case no preferences are available
            }

            // Debug output for each shift assignment
            cout << "Shift " << i + 1 << " assigned Nurse " << chromosome.shifts[i] + 1
                 << " with Preference " << nurse_preferences[chromosome.shifts[i]][i] << endl;
        }
    }
    return population;
}
// Updated calculate_fitness function to accept Chromosome parameter
int calculate_fitness(const Chromosome& chromosome) {
    int fitness = 0;

    for (int shift = 0; shift < NUM_SHIFTS; shift++) {
        int nurse_id = chromosome.shifts[shift];
        int preference = nurse_preferences[nurse_id][shift];

        // Adjust fitness based on preference
        if (preference == 1) {
            fitness -= 1; // Prefer not
        } else if (preference == 2) {
            fitness -= 2; // Doesn't want to work
        }

        // Debug output
        cout << "Shift " << shift + 1 << ": Nurse " << nurse_id + 1
             << " (Preference: " << preference << ") - Fitness impact: "
             << ((preference == 1) ? -1 : (preference == 2) ? -2 : 0) << endl;
    }

    return fitness;
}

// Selection function (Tournament selection)
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

// Crossover function to create offspring
Chromosome crossover(const Chromosome& parent1, const Chromosome& parent2) {
    Chromosome offspring;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, NUM_SHIFTS - 1);
    int crossover_point = distr(gen);

    for (int i = 0; i < NUM_SHIFTS; ++i) {
        if (i < crossover_point) {
            offspring.shifts[i] = parent1.shifts[i];
        }
        else {
            offspring.shifts[i] = parent2.shifts[i];
        }
    }
    return offspring;
}

// Mutation function to introduce variability
void mutate(Chromosome& chromosome) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distr(0.0, 1.0);
    uniform_int_distribution<> nurse_distr(0, NUM_NURSES - 1);
    uniform_int_distribution<> shift_distr(0, NUM_SHIFTS - 1);

    if (distr(gen) < MUTATION_RATE) {
        int shift = shift_distr(gen);

        // Find the best available nurse (avoid preference 2 if possible)
        vector<int> preferred_nurses;
        vector<int> fallback_nurses;

        for (int nurse = 0; nurse < NUM_NURSES; ++nurse) {
            if (nurse_preferences[nurse][shift] == 0) {
                preferred_nurses.push_back(nurse);
            }
            else if (nurse_preferences[nurse][shift] == 1) {
                fallback_nurses.push_back(nurse);
            }
        }

        if (!preferred_nurses.empty()) {
            uniform_int_distribution<> nurse_distr(0, preferred_nurses.size() - 1);
            chromosome.shifts[shift] = preferred_nurses[nurse_distr(gen)];
        }
        else if (!fallback_nurses.empty()) {
            uniform_int_distribution<> nurse_distr(0, fallback_nurses.size() - 1);
            chromosome.shifts[shift] = fallback_nurses[nurse_distr(gen)];
        }
    }
}

// Main function to run the genetic algorithm
int main() {
    // Step 1: Read nurse preferences from the CSV file
    cout << "Loading nurse preferences from CSV...\n";
    read_csv("Nurse_List_Department_Included.csv");

    // Step 2: Initialize the population with random schedules
    cout << "Initializing population...\n";
    vector<Chromosome> population = initialize_population();

    // Step 3: Evaluate the initial fitness of each chromosome in the population
    cout << "Evaluating initial fitness for each chromosome...\n";
    for (auto& chromosome : population) {
        chromosome.fitness = calculate_fitness(chromosome);
    }

    // Step 4: Track the best chromosome across all generations
    Chromosome best_overall = *min_element(population.begin(), population.end(),
        [](const Chromosome& a, const Chromosome& b) {
            return a.fitness < b.fitness;
        });
    cout << "Initial best fitness: " << best_overall.fitness << "\n";

    // Step 5: Evolution loop - generate new populations for each generation
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        vector<Chromosome> new_population;

        // Generate new population through selection, crossover, and mutation
        while (new_population.size() < POPULATION_SIZE) {
            Chromosome parent1 = select_parent(population);
            Chromosome parent2 = select_parent(population);
            Chromosome offspring = crossover(parent1, parent2);
            mutate(offspring);
            offspring.fitness = calculate_fitness(offspring);  // Evaluate fitness of offspring
            new_population.push_back(offspring);
        }

        // Replace the old population with the new one
        population = std::move(new_population);

        // Identify the best chromosome in the current generation
        auto best_chromosome_iter = min_element(population.begin(), population.end(),
            [](const Chromosome& a, const Chromosome& b) {
                return a.fitness < b.fitness;
            });

        // If the current generation's best is better than the overall best, update it
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
        cout << "DEBUG: Confirming Nurse " << nurse + 1 << " assigned to Shift " << (i + 1) << " with Preference " << preference << endl;
    }

    // Optional: Save the best schedule to a file
    ofstream output_file("best_schedule.txt");
    if (output_file.is_open()) {
        output_file << "Best Schedule Found:\n";
        output_file << "Fitness: " << best_overall.fitness << "\n";
        output_file << "Shift Assignments:\n";
        for (int i = 0; i < NUM_SHIFTS; ++i) {
            int nurse = best_overall.shifts[i];
            int preference = nurse_preferences[nurse][i];
            output_file << "Shift " << (i + 1) << ": Nurse " << nurse + 1 << " (Preference: " << preference << ")\n";
        }
        output_file.close();
        cout << "\nBest schedule saved to 'best_schedule.txt'.\n";
    } else {
        cerr << "Error: Could not open file to save the best schedule.\n";
    }

    return 0;
}
