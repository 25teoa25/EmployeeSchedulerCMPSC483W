// main.cpp
#include <iostream>
#include <unordered_map>
#include "NurseList.h"
#include "CSVParser.h"

int main() {

    std::unordered_map< std::string, NurseList > nurse_lists;  // Map to store linked lists for each nurse type

    // Read the CSV file and populate nurse lists
    readCSV( "/Users/alexteo/EmployeeSchedulerCMPSC483W/DataStructure/Nurse_List_Final.csv" , nurse_lists );

    // Display all nurse lists
    for ( const auto& pair : nurse_lists ) {

        std::cout << "Nurse Type: " << pair.first << std::endl;
        pair.second.display();
        std::cout << "---------------------------------" << std::endl;

    }

    return 0;

}
