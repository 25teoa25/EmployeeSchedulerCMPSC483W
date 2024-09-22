// CSVParser.cpp
#include "CSVParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Reads a CSV file and populates nurse data into linked lists for each nurse type.
 * 
 * @param filename The name of the CSV file to read.
 * @param nurse_lists A reference to an unordered_map where the key is the nurse type (e.g., "RN", "LPN") and the value is a NurseList containing nurses of that type.
 * 
 * This function opens the CSV file, skips the header line, and processes each subsequent line to extract nurse details such as name, number, type, pay, and assigned shifts.
 * The parsed data is then added to the appropriate NurseList in the unordered_map based on the nurse type.
 */
void readCSV( const std::string& filename , std::unordered_map<std::string , NurseList>& nurse_lists ) {

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
        std::string name, type;
        int number;
        double pay;
        std::vector< std::string > shifts( 42 );  // Initialize vector to hold 42 shifts

        // Parse the nurse's name (up to the first comma)
        getline( ss , name , ',' );

        // Parse the nurse's number and skip the comma
        ss >> number;
        ss.ignore();

        // Parse the nurse's type (e.g., "RN", "LPN")
        getline( ss , type , ',' );

        // Parse the nurse's pay and skip the comma
        ss >> pay;
        ss.ignore();

        // Read the 42 shifts
        for ( int i = 0; i < 42; ++i ) {

            getline( ss , shifts[i] , ',' );

        }

        // Add to the appropriate nurse list based on type
        nurse_lists[type].addNurse( name , number , pay , shifts );
    }

    file.close();
}
