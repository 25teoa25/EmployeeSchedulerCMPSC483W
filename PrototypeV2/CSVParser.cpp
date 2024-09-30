// CSVParser.cpp
#include "CSVParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

//function to load csv with all nurses into 3 linked lists: 1 for each: LPNs, RNS, NAs
void loadNurses( const std::string& filename , std::unordered_map<std::string , NurseList>& nurse_lists ) {

    // Open the CSV file
    std::ifstream file( filename );

    // Check if the file was successfully opened
    if ( !file.is_open() ) {

        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;

    }

    std::string line;
    getline( file , line );  // Skip header line

    // Read each line from the CSV
    while ( getline( file , line ) ) {

        std::stringstream ss( line ); // Create a stringstream to parse the line
        std::string name, type, dept;
        int number;
        std::vector< std::string > shifts( 42 );  // Initialize vector to hold 42 shifts

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
size_t std::hash<Key>::operator()(const Key& k) const {
    return hash<std::string>()(std::get<0>(k)) ^
           hash<int>()(std::get<1>(k)) ^
           hash<std::string>()(std::get<2>(k));
}

// Function to read the CSV of department constraints and store the data in NurseMap
// nurseMap format: (dept, shift #, nurse type) --> # needed
void loadConstraints(const std::string& filename, NurseMap& nurse_map) {
    std::ifstream file(filename);
    std::string line, dept, shift_str, rn_str, lpn_str, na_str;
    
    // Read the header line first (and ignore it)
    std::getline(file, line);

    // Read the file line by line
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        
        // Read the columns (assuming they are comma-separated)
        std::getline(ss, dept, ',');
        std::getline(ss, shift_str, ',');
        std::getline(ss, rn_str, ',');
        std::getline(ss, lpn_str, ',');
        std::getline(ss, na_str, ',');

        // Convert the strings to integers where necessary
        int shift = std::stoi(shift_str);
        int rn_count = std::stoi(rn_str);
        int lpn_count = std::stoi(lpn_str);
        int na_count = std::stoi(na_str);

        // Insert into the map for each nurse type
        nurse_map[std::make_tuple(dept, shift, "RN")] = rn_count;
        nurse_map[std::make_tuple(dept, shift, "LPN")] = lpn_count;
        nurse_map[std::make_tuple(dept, shift, "NA")] = na_count;
    }
}

// Function to display the NurseMap constraints
void displayNurseMap(const NurseMap& nurse_map) {
    std::cout << "Department | Shift # | Nurse Type | # Needed" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    
    for (const auto& entry : nurse_map) {
        // Unpack the tuple key
        const std::string& dept = std::get<0>(entry.first);
        int shift = std::get<1>(entry.first);
        const std::string& nurseType = std::get<2>(entry.first);
        int numberNeeded = entry.second;

        // Print the values
        std::cout << dept << " | " << shift << " | " << nurseType << " | " << numberNeeded << std::endl;
    }
}
