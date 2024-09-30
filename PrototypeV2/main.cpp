// main.cpp
#include <iostream>
#include <unordered_map>
#include "NurseList.h"
#include "CSVParser.h"

// to run these files: g++ -std=c++11 main.cpp CSVParser.cpp NurseList.cpp -o Testfile
//then: ./Testfile
int main() {

    std::unordered_map< std::string, NurseList > nurse_lists;  // Map to store linked lists for each nurse type

    // Read the CSV file and populate nurse lists
    loadNurses( "/Users/rstack/Documents/nurse_data.csv" , nurse_lists );

    //display all nurses
    for (auto it = nurse_lists.begin(); it != nurse_lists.end(); ++it) {
        const std::string& nurseType = it->first;  // Key: nurse type
        const NurseList& nurseList = it->second;   // Value: NurseList object

        // Display nurse type (optional, for context)
        std::cout << "Displaying Nurse List for: " << nurseType << std::endl;

        // Call the display() method of NurseList
        nurseList.display();
    }

    NurseMap nurse_map;
    std::string filename = "/Users/rstack/Documents/5Day_Constraints.csv";

    // Read the CSV and populate the data structure
    loadConstraints(filename, nurse_map);
    displayNurseMap(nurse_map);

    return 0;

}
