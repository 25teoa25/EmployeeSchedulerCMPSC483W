// CSVParser.h
#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <unordered_map>
#include <string>
#include <tuple>
#include "NurseList.h"
using namespace std;

/**
 * @brief Reads a CSV file and populates nurse data into linked lists based on nurse type.
 * 
 * @param filename The name of the CSV file to read.
 * @param nurse_lists A reference to an unordered_map where the key is the nurse type (e.g., "RN", "LPN") and the value is a NurseList containing nurses of that type.
 * 
 * The function processes the CSV, where each line represents a nurse and their attributes. It parses the nurse's name, number, type, pay, and assigned shifts,
 * and adds the nurse to the appropriate linked list based on their type.
 */
void loadNurses( const string& filename , unordered_map<string , NurseList>& nurse_lists );


using Key = tuple<string, int, string>; // (dept, shift #, nurseType)
using NurseMap = unordered_map<Key, int>;         // Map to store # needed for each (dept, shift, nurseType)

// Hash function for tuple so it can be used in an unordered_map
namespace std {
    template <>
    struct hash<Key> {
        size_t operator()(const Key& k) const;
    };
}

/**
 * @brief Reads a CSV file and populates nurse constraints into nurse map. nurseMap format: (dept, shift #, nurse type) --> # needed
 * 
 * @param filename The name of the CSV file to read.
 * @param nurse_map A reference to a map of nurse constarints in the format: (dept, shift #, nurse type) --> # needed
 * 
 * The function processes the CSV, where each line represents a nurse and their attributes. It parses the nurse's name, number, type, pay, and assigned shifts,
 * and adds the nurse to the appropriate linked list based on their type.
 */
void loadConstraints(const string& filename, NurseMap& nurse_map);

/**
 * @brief displays nurse map (all constraints for departments in the4 hopital for each shift)
 * 
 * @param nurse_map A reference to a map of nurse constarints in the format: (dept, shift #, nurse type) --> # needed
 * 
 * The function processes the CSV, where each line represents a nurse and their attributes. It parses the nurse's name, number, type, pay, and assigned shifts,
 * and adds the nurse to the appropriate linked list based on their type.
 */
void displayNurseMap(const NurseMap& nurse_map);

#endif // CSV_PARSER_H
