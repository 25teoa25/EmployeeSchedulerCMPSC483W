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
int structure1(ShiftSchedule schedule, const string &department, int currPref, string nurseType);
int structure2(ShiftSchedule schedule, const string &department, int currPref, string nurseType);
int structure3(ShiftSchedule schedule, const string &department, int currPref, string nurseType);
int structure4(ShiftSchedule schedule, const string &department, int currPref, string nurseType);
int structure5(ShiftSchedule schedule, const string &department, int currPref, string nurseType);
int structure6(ShiftSchedule schedule, const string &department, int currPref, string nurseType);
int structure7(ShiftSchedule schedule, const string &department, int currPref, string nurseType);
int structure8(ShiftSchedule schedule, const string &department, int currPref, string nurseType);

#endif // NEIGHBOR_H