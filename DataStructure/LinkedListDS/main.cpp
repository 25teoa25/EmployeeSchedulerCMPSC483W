// main.cpp
#include <iostream>
#include <unordered_map>
#include "NurseList.h"
#include "CSVParser.h"
#include "NurseFunctions.h"

int main() {

    std::unordered_map< std::string, NurseList > nurse_lists;  // Map to store linked lists for each nurse type

    // Read the CSV file and populate nurse lists
    readCSV( "/Users/alexteo/EmployeeSchedulerCMPSC483W/DataStructure/Nurse_List_Final.csv" , nurse_lists );

    std::vector<std::vector<std::string>> constraints = {
        {"Oncology", "5", "2", "1", "1"}  // Example constraints
        // Add more constraint rows as needed
    };

    // Select the best nurses for shift 5 in the Oncology department
    std::vector<Nurse*> selected_nurses = selectBestNurses(nurse_lists, constraints, 5, "Oncology");

    // Print selected nurses
    for (Nurse* nurse : selected_nurses) {
        std::cout << "Selected Nurse: " << nurse->nurseName << std::endl;
    }

    return 0;

}
