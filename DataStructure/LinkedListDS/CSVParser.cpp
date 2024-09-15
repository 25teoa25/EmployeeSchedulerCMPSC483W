// CSVParser.cpp
#include "CSVParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Function to read CSV and populate linked lists for each nurse type
void readCSV( const std::string& filename , std::unordered_map<std::string , NurseList>& nurse_lists ) {

    std::ifstream file( filename );

    if ( !file.is_open() ) {

        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;

    }

    std::string line;
    getline( file , line );  // Skip header line

    // Read each line from the CSV
    while ( getline( file , line ) ) {

        std::stringstream ss( line );
        std::string name, type;
        int number;
        double pay;
        std::vector< std::string > shifts( 42 );  // Initialize vector to hold 42 shifts

        // Parse each field
        getline( ss , name , ',' );
        ss >> number;
        ss.ignore();  // Ignore the comma
        getline( ss , type , ',' );
        ss >> pay;
        ss.ignore();  // Ignore the comma

        // Read the 42 shifts
        for ( int i = 0; i < 42; ++i ) {

            getline( ss , shifts[i] , ',' );

        }

        // Add to the appropriate nurse list based on type
        nurse_lists[type].addNurse( name , number , pay , shifts );
    }

    file.close();
}
