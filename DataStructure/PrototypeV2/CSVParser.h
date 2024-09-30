// CSVParser.h
#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <unordered_map>
#include <string>
#include <tuple>
#include "NurseList.h"

/**
 * @brief Reads a CSV file and populates nurse data into linked lists based on nurse type.
 * 
 * @param filename The name of the CSV file to read.
 * @param nurse_lists A reference to an unordered_map where the key is the nurse type (e.g., "RN", "LPN") and the value is a NurseList containing nurses of that type.
 * 
 * The function processes the CSV, where each line represents a nurse and their attributes. It parses the nurse's name, number, type, pay, and assigned shifts,
 * and adds the nurse to the appropriate linked list based on their type.
 */
void loadNurses( const std::string& filename , std::unordered_map<std::string , NurseList>& nurse_lists );


using Key = std::tuple<std::string, int, std::string>; // (dept, shift #, nurseType)
using NurseMap = std::unordered_map<Key, int>;         // Map to store # needed for each (dept, shift, nurseType)

// Hash function for tuple so it can be used in an unordered_map
namespace std {
    template <>
    struct hash<Key> {
        size_t operator()(const Key& k) const;
    };
}

void loadConstraints(const std::string& filename, NurseMap& nurse_map);
void displayNurseMap(const NurseMap& nurse_map);

#endif // CSV_PARSER_H
