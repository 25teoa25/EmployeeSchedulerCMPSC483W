#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>

/**
 * @brief Reads a CSV file and populates nurse data into the global departmentNursesMap.
 * 
 * @param fileName The name of the CSV file to read.
 * 
 * This function reads and parses the CSV file to populate the global map of nurses 
 * categorized by their department and type.
 */
void parseNursesCSV(const std::string& fileName);

#endif // CSV_PARSER_H
