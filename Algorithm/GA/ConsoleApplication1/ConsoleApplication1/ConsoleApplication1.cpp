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

    while (getline(file, line) && nurse_index < NUM_NURSES) {
        stringstream ss(line);
        string value;
        int shift_index = 0;

        // Skip the first 4 columns: Full Name, Nurse Number, Nurse Type, Nurse Department
        for (int i = 0; i < 4; ++i) {
            getline(ss, value, ',');
        }

        // Read shift preferences
        while (getline(ss, value, ',') && shift_index < NUM_SHIFTS) {
            nurse_preferences[nurse_index][shift_index] = stoi(value);
            shift_index++;
        }

        nurse_index++;
    }

    file.close();
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
            // Prefer assigning nurses with preference 0 first, then 1, and avoid 2 if possible
            vector<int> preferred_nurses;
            vector<int> fallback_nurses;

            for (int nurse = 0; nurse < NUM_NURSES; ++nurse) {
                if (nurse_preferences[nurse][i] == 0) {
                    preferred_nurses.push_back(nurse);
                }
                else if (nurse_preferences[nurse][i] == 1) {
                    fallback_nurses.push_back(nurse);
                }
            }

            if (!preferred_nurses.empty()) {
                uniform_int_distribution<> distr(0, preferred_nurses.size() - 1);
                chromosome.shifts[i] = preferred_nurses[distr(gen)];
            }
            else if (!fallback_nurses.empty()) {
                uniform_int_distribution<> distr(0, fallback_nurses.size() - 1);
                chromosome.shifts[i] = fallback_nurses[distr(gen)];
            }
            else {
                chromosome.shifts[i] = gen() % NUM_NURSES;
            }
        }
    }
    return population;
}

// Fitness function to evaluate each chromosome
int calculate_fitness(const Chromosome& chromosome) {
    int fitness = 0;

    // Evaluate shift preferences
    for (int i = 0; i < NUM_SHIFTS; ++i) {
        int nurse = chromosome.shifts[i];
        int preference = nurse_preferences[nurse][i];

        if (preference == 2) {
            fitness += 200;  // Much larger penalty for "doesn't want to work"
        }
        else if (preference == 1) {
            fitness += 50;  // Moderate penalty for "prefer not"
        }
        else {
            fitness -= 10;  // Small bonus for "can work"
        }
    }

    // Penalize for consecutive shifts beyond 2 in a row
    for (int i = 0; i < NUM_SHIFTS - 2; ++i) {
        if (chromosome.shifts[i] == chromosome.shifts[i + 1] && chromosome.shifts[i + 1] == chromosome.shifts[i + 2]) {
            fitness += 100;  // Strong penalty for more than 2 consecutive shifts
        }
    }

    // Penalize for nurses assigned to too many shifts
    vector<int> nurse_shift_count(NUM_NURSES, 0);
    for (int i = 0; i < NUM_SHIFTS; ++i) {
        int nurse = chromosome.shifts[i];
        nurse_shift_count[nurse]++;
    }

    for (int count : nurse_shift_count) {
        if (count > 2) {
            fitness += (count - 2) * 10; // Penalty for each additional shift beyond 2
        }
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
    // Read nurse preferences from the CSV file
    read_csv("Nurse_List_Department_Included.csv");

    // Initialize population
    vector<Chromosome> population = initialize_population();

    // Evaluate initial fitness
    for (auto& chromosome : population) {
        chromosome.fitness = calculate_fitness(chromosome);
    }

    // Track the best chromosome overall
    Chromosome best_overall = *min_element(population.begin(), population.end(),
        [](const Chromosome& a, const Chromosome& b) {
            return a.fitness < b.fitness;
        });

    // Evolution loop
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        vector<Chromosome> new_population;

        // Create new generation
        while (new_population.size() < POPULATION_SIZE) {
            Chromosome parent1 = select_parent(population);
            Chromosome parent2 = select_parent(population);
            Chromosome offspring = crossover(parent1, parent2);
            mutate(offspring);
            offspring.fitness = calculate_fitness(offspring);
            new_population.push_back(offspring);
        }

        // Replace old population with new population
        population = std::move(new_population);

        // Find the best chromosome in the current generation
        auto best_chromosome_iter = min_element(population.begin(), population.end(),
            [](const Chromosome& a, const Chromosome& b) {
                return a.fitness < b.fitness;
            });

        if (best_chromosome_iter != population.end() && best_chromosome_iter->fitness < best_overall.fitness) {
            best_overall = *best_chromosome_iter;
        }

        cout << "Generation " << generation << " - Best Fitness: " << best_chromosome_iter->fitness << endl;
    }

    // Print the best chromosome overall
    cout << "\nBest Schedule Found:\n";
    cout << "Fitness: " << best_overall.fitness << "\n";
    cout << "Shift Assignments:\n";
    for (int i = 0; i < NUM_SHIFTS; ++i) {
        int nurse = best_overall.shifts[i];
        int preference = nurse_preferences[nurse][i];
        cout << "Shift " << (i + 1) << ": Nurse " << nurse << " (Preference: " << preference << ")\n";
    }

    // Save the best chromosome to a file
    ofstream output_file("best_schedule.txt");
    if (output_file.is_open()) {
        output_file << "Best Schedule Found:\n";
        output_file << "Fitness: " << best_overall.fitness << "\n";
        output_file << "Shift Assignments:\n";
        for (int i = 0; i < NUM_SHIFTS; ++i) {
            int nurse = best_overall.shifts[i];
            int preference = nurse_preferences[nurse][i];
            output_file << "Shift " << (i + 1) << ": Nurse " << nurse << " (Preference: " << preference << ")\n";
        }
        output_file.close();
        cout << "\nBest schedule saved to 'best_schedule.txt'.\n";
    }
    else {
        cerr << "Error: Could not open file to save the best schedule.\n";
    }

    return 0;
}
