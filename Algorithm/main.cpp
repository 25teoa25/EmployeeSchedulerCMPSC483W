#include <iostream>
#include <unordered_map>
#include "../DataStructure/PrototypeV2/NurseFunctions.h"
#include "../DataStructure/PrototypeV2/CSVParser.h"
#include "../DataStructure/PrototypeV2/NurseList.h"
#include <random>
using namespace std;

//to run this file right now:
//g++ -std=c++11 main.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/PrototypeV2/CSVParser.cpp /Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/PrototypeV2/NurseList.cpp -o Testfile
//then: ./Testfile

int main() {

    unordered_map< string, NurseList > nurse_lists;  // Map to store linked lists for each nurse type

    // Read the CSV file and populate nurse lists
    loadNurses( "/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/PrototypeV2/nurse_data.csv" , nurse_lists );


    //display all nurses
    for (auto it = nurse_lists.begin(); it != nurse_lists.end(); ++it) {
        const string& nurseType = it->first;  // Key: nurse type
        const NurseList& nurseList = it->second;   // Value: NurseList object

        // Display nurse type (optional, for context)
        cout << "Displaying Nurse List for: " << nurseType << endl;

        // Call the display() method of NurseList
        nurseList.display();
    }

    NurseMap nurse_map;
    string filename = "/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/PrototypeV2/5Day_Constraints.csv";

    // Read the CSV and populate the data structure
    loadConstraints(filename, nurse_map); //populates nurse_map where key is a tuple (dept, shift num, ype of nurse) : how many needed
    displayNurseMap(nurse_map);

    // Shift specification algorithm for assigning the shifts
    // to the nurses during generating the initial feasible solution

    // Step 1: Generate a random number r from {1,2,3}
    random_device rd;
    mt19937 gen(rd()); 
    uniform_int_distribution<> distrib(1, 3);

    // Step 2: If r != 1, go to step 3

    // Step 3: If r != 2, go to step 4

    // Step 4: If the number of uncovered demands for night scheduling period = 0, go to step 1

}