#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>

/**
 * @brief Reads a CSV file and populates nurse data into the global departmentNursesMap.
 * 
 * @param fileName The name of the CSV file to read.
 */
void parseNursesCSV(const std::string& fileName);

/**
 * @brief Reads a CSV file and populates constraints into the global constraintsMap.
 * 
 * @param fileName The name of the CSV file to read.
 */
void parseConstraintsCSV(const std::string& fileName);

#endif // CSV_PARSER_H
