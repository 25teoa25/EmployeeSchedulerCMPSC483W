// CSVParser.cpp
#include "CSVParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

//function to load csv with all nurses into 3 linked lists: 1 for each: LPNs, RNS, NAs
void loadNurses( const string& filename , unordered_map<string , NurseList>& nurse_lists ) {

    // Open the CSV file
    ifstream file( filename );

    // Check if the file was successfully opened
    if ( !file.is_open() ) {

        cerr << "Error: Could not open file " << filename << endl;
        return;

    }

    string line;
    getline( file , line );  // Skip header line

    // Read each line from the CSV
    while ( getline( file , line ) ) {

        stringstream ss( line ); // Create a stringstream to parse the line
        string name, type, dept;
        int number;
        vector< string > shifts( 42 );  // Initialize vector to hold 42 shifts

        // Parse the nurse's name (up to the first comma)
        getline( ss , name , ',' );

        // Parse the nurse's number and skip the comma
        ss >> number;
        ss.ignore();

        // Parse the nurse's type (e.g., "RN", "LPN")
        getline( ss , type , ',' );

        // Parse the nurse's dept
        getline( ss , dept , ',' );

        // Read the 42 shifts
        for ( int i = 0; i < 42; ++i ) {

            getline( ss , shifts[i] , ',' );

        }

        // Add to the appropriate nurse list based on type
        nurse_lists[type].addNurse( name , number , type, dept , shifts );
    }

    file.close();
}

// Hash function for Key type
size_t hash<Key>::operator()(const Key& k) const {
    return hash<string>()(get<0>(k)) ^
           hash<int>()(get<1>(k)) ^
           hash<string>()(get<2>(k));
}

// Function to read the CSV of department constraints and store the data in NurseMap
// nurseMap format: (dept, shift #, nurse type) --> # needed
void loadConstraints(const string& filename, NurseMap& nurse_map) {
    ifstream file(filename);
    string line, dept, shift_str, rn_str, lpn_str, na_str;
    
    // Read the header line first (and ignore it)
    getline(file, line);

    // Read the file line by line
    while (getline(file, line)) {
        istringstream ss(line);
        
        // Read the columns (assuming they are comma-separated)
        getline(ss, dept, ',');
        getline(ss, shift_str, ',');
        getline(ss, rn_str, ',');
        getline(ss, lpn_str, ',');
        getline(ss, na_str, ',');

        // Convert the strings to integers where necessary
        int shift = stoi(shift_str);
        int rn_count = stoi(rn_str);
        int lpn_count = stoi(lpn_str);
        int na_count = stoi(na_str);

        // Insert into the map for each nurse type
        nurse_map[make_tuple(dept, shift, "RN")] = rn_count;
        nurse_map[make_tuple(dept, shift, "LPN")] = lpn_count;
        nurse_map[make_tuple(dept, shift, "NA")] = na_count;
    }
}

// Function to display the NurseMap constraints
void displayNurseMap(const NurseMap& nurse_map) {
    cout << "Department | Shift # | Nurse Type | # Needed" << endl;
    cout << "--------------------------------------------" << endl;
    
    for (const auto& entry : nurse_map) {
        // Unpack the tuple key
        const string& dept = get<0>(entry.first);
        int shift = get<1>(entry.first);
        const string& nurseType = get<2>(entry.first);
        int numberNeeded = entry.second;

        // Print the values
        cout << dept << " | " << shift << " | " << nurseType << " | " << numberNeeded << endl;
    }
}
