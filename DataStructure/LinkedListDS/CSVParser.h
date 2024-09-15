// CSVParser.h
#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <unordered_map>
#include <string>
#include "NurseList.h"

// Function to read CSV and populate linked lists for each nurse type
void readCSV( const std::string& filename , std::unordered_map<std::string , NurseList>& nurse_lists );

#endif // CSV_PARSER_H
