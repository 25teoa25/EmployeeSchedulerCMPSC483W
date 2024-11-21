#ifndef LINEARPGHELPER_H
#define LINEARPGHELPER_H

#include "CSVParser.h"
#include "NurseFunctions.h"
#include "NurseList.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

int generateRandomShift();

void simpleAssignment(ShiftSchedule& schedule, const string &department, const string &nurseType, int shift, int &demandRemaining);

void optimizePreferenceAssignment(ShiftSchedule& schedule, const string &department, const string &nurseType, int shift, int &demandRemaining);

#endif // LINEARPGHELPER_H