#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <iostream>
#include <unordered_map>
#include "../DataStructure/LinkedListDS/NurseFunctions.h"
#include "../DataStructure/LinkedListDS/CSVParser.h"
#include "../DataStructure/LinkedListDS/NurseList.h"
#include <random>
#include <algorithm>

// Declare external reference to departmentNursesMap
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Nurse>>> departmentNursesMap;

// Function to check if a change is feasible
bool feasible(int currPref, int newPref);

// Neighborhood structures
void structure1();
void structure2();
void structure3();
void structure4();
void structure5();
void structure6();
void structure7();
void structure8();

#endif // NEIGHBOR_H