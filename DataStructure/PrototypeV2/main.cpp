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
    loadNurses( "/Users/rstack/Documents/CMPSC483W/EmployeeSchedulerCMPSC483W/DataStructure/PrototypeV2/nurse_data.csv" , nurse_lists );



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
    string filename = "/Users/rstack/Documents/CMPSC483W/EmployeeSchedulerCMPSC483W/DataStructure/PrototypeV2/5Day_Constraints.csv";

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

    // Array of departments
    std::vector<std::string> departments = { "Oncology", "Pediatrics", "Surgery" };

    // Array of nurse types
    std::vector<std::string> nurse_types = { "RN", "LPN", "NA" };

//nested for loops for brute force implementation
    // Iterate through 15 shifts
    for (int shift = 1; shift <= 15; ++shift) {
        // Iterate through each department
        for (const std::string& department : departments) {
            // Iterate through each nurse type
            for (const std::string& nurse_type : nurse_types) {
                // Build the key for nurse_map lookup
                Key key = std::make_tuple(department, shift, nurse_type);

                // Check if there are constraints for this department, shift, and nurse type
                if (nurse_map.find(key) != nurse_map.end()) {
                    int required_nurses = nurse_map.at(key);

                    // If there are required nurses, find and assign them from the corresponding NurseList
                    if (required_nurses > 0 && nurse_lists.find(nurse_type) != nurse_lists.end()) {
                        NurseList& nurseList = nurse_lists.at(nurse_type);  // Get the NurseList for this nurse type

                        //write code to iterate through nurseList and return all nurses with a "2" for that shift in their shifts array

                        // Vector to store selected nurses for the current shift
                        std::vector<Nurse*> selected_nurses;

                        // Pointer to the head of the NurseList
                        Nurse* current = nurseList.getHead();

                        // Iterate through the NurseList
                        while (current != nullptr) {
                            // Check if the nurse has a preference of "2" for the current shift
                            if (current->nurseShifts[shift - 1] == "2") {  // Shift array is 0-based
                                selected_nurses.push_back(current);  // Add the nurse to the selected list
                            }
                            current = current->next;  // Move to the next nurse in the list
                        }

                        // If the number of selected nurses exceeds the required nurses, truncate the vector
                        if (selected_nurses.size() > required_nurses) {
                            selected_nurses.resize(required_nurses);
                        }

                        // Assign the selected nurses to the shift (could involve updating their schedule, etc.)
                        for (Nurse* nurse : selected_nurses) {
                            std::cout << "Assigning Nurse " << nurse->nurseName << ", " << nurse->nurseType<< " to shift " << shift << " in " << department << std::endl;
                        }

                        // If there are still required nurses left, you could handle it by selecting nurses with preference "1"
                        if (selected_nurses.size() < required_nurses) {
                            int remaining_nurses = required_nurses - selected_nurses.size();
                            current = nurseList.getHead();  // Reset the pointer to the start of the list

                            // Now, assign nurses who can work the shift but don't prefer it (preference "1")
                            while (current != nullptr && remaining_nurses > 0) {
                                if (current->nurseShifts[shift - 1] == "1") {  // Can work but doesn't prefer
                                    selected_nurses.push_back(current);
                                    std::cout << "Assigning Nurse " << current->nurseName << " to shift " << shift << " in " << department << std::endl;
                                    --remaining_nurses;
                                }
                            current = current->next;
                            }
                        }
                    }
                }
            }
        }   
    }
    


    return 0;

}
