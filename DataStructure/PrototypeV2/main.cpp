// main.cpp
#include <iostream>
#include <unordered_map>
#include "NurseList.h"
#include "CSVParser.h"
#include "NurseFunctions.h"
using namespace std;

// to run these files: g++ -std=c++11 main.cpp CSVParser.cpp NurseList.cpp -o Testfile
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
    loadConstraints(filename, nurse_map);
    displayNurseMap(nurse_map);

    //hard coded to fit five day schedule
    int shift_num = 1;
    for (int day = 1; day <= 5; day++) {
        for (int current_nurse = 1; current_nurse <= 100; current_nurse++) {
            for (shift_num = shift_num * day; shift_num <= 15; shift_num++) {
                //selectBestNurses(nurse_lists, )
            }
        }
    }

    return 0;

}
