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
void structure1(int currPref, string nurseType);
void structure2(int currPref, string nurseType);
void structure3(int currPref, string nurseType);
void structure4(int currPref, string nurseType);
void structure5(int currPref, string nurseType);
void structure6(int currPref, string nurseType);
void structure7(int currPref, string nurseType);
void structure8(int currPref, string nurseType);

#endif // NEIGHBOR_H