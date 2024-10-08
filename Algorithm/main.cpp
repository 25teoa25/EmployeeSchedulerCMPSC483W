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
    unordered_map<string, NurseList> nurse_lists;
    NurseMap nurse_map;
    
    // Load nurses and constraints
    loadNurses("/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/PrototypeV2/nurse_data.csv", nurse_lists);
    loadConstraints("/Users/saadyarao/EmployeeSchedulerCMPSC483W/DataStructure/PrototypeV2/5Day_Constraints.csv", nurse_map);
    
    vector<string> departments = {"Oncology", "Pediatric", "Surgery"};
    vector<string> nurse_types = {"RN", "LPN", "NA"};
    
    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 3);
    
    // Coded for 5 days (15 shifts)

    // Go through each day
    for (int day = 0; day < 5; day++) {
        cout << "\nScheduling Day: " << day + 1 << "\n";
        
        // Go through all departments
        for (const string& dept : departments) {
            cout << "\nDepartment: " << dept << "\n";
            
            // Go through each nurse type
            for (const string& nurse_type : nurse_types) {

                if (nurse_lists[nurse_type].getHead() == nullptr) {
                    continue;  // Skip if no nurses of this type
                }

                cout << "Nurse Type: " << nurse_type << "\n";
                
                // Initialize demand variables
                int dm = 0, de = 0, dn = 0;
                
                // Get demands from nurse_map
                for (int shift_offset = 1; shift_offset <= 3; shift_offset++) {
                    int shift_num = day * 3 + shift_offset;
                    Key key = make_tuple(dept, shift_num, nurse_type);
                    
                    if (nurse_map.find(key) != nurse_map.end()) {
                        int demand = nurse_map[key];
                        switch (shift_offset) {
                            case 1: dm = demand; break;
                            case 2: de = demand; break;
                            case 3: dn = demand; break;
                        }
                    }
                }
                
                // Skip if no demands for this combination
                if (dm == 0 && de == 0 && dn == 0) continue;
                
                cout << "Initial demands - Morning: " << dm 
                     << ", Evening: " << de 
                     << ", Night: " << dn << "\n";
                
                // Main scheduling loop
                while ((dm > 0 || de > 0 || dn > 0) && nurse_lists[nurse_type].getHead() != nullptr) {
                    string sh;
                    int r = dis(gen);
                    
                    // Shift specification algorithm
                    if (r == 1) {
                        if (dm == 0) continue;
                        if (de > 0) {
                            sh = "L";
                            dm--;
                            de--;
                        } else {
                            sh = "M";
                            dm--;
                        }
                    } else if (r == 2) {
                        if (de == 0) continue;
                        if (dm > 0) {
                            sh = "L";
                            dm--;
                            de--;
                        } else {
                            sh = "E";
                            de--;
                        }
                    } else { // r == 3
                        if (dn == 0) continue;
                        sh = "N";
                        dn--;
                    }
                    
                    // Assign nurse
                    Nurse* current = nurse_lists[nurse_type].getHead();
                    if (current != nullptr) {
                        int shift_num;
                        if (sh == "M") shift_num = day * 3 + 1;
                        else if (sh == "E" || sh == "L") shift_num = day * 3 + 2;
                        else shift_num = day * 3 + 3; // Night shift

                        cout << "Assigning " << nurse_type << " " << current->nurseName 
                             << " to " << sh << " shift (Shift #" << shift_num << ")\n";
                        
                        // Here you would update the nurse's schedule
                        // For example: current->assignedShifts.push_back(shift_num);
                    }
                }
            }
        }
    }
    
    return 0;
}
