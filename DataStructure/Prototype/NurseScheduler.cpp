#include "NurseScheduler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// Define the loadFromCSV function
void NurseScheduler::loadFromCSV(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header line

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;

        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 46) continue;

        std::string fullName = tokens[0];
        std::string nurseNumber = tokens[1];
        std::string nurseType = tokens[2];

        Employee employee(nurseNumber, fullName, nurseType);
        for (int shift = 1; shift <= 42; ++shift) {
            int availability = std::stoi(tokens[3 + shift]);
            employee.setAvailability(shift, static_cast<Availability>(availability));
        }

        schedule.addEmployee(employee);
    }

    file.close();
}
