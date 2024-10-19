#include "neighbor.h"

using namespace std;

// Random seed
std::random_device rd;
std::mt19937 gen(rd());

// Helper function to get a random nurse
Nurse& getRandomNurse(const string& department, const string& type) {
    auto& nurses = departmentNursesMap[department][type];
    std::uniform_int_distribution<> dis(0, nurses.size() - 1);
    return nurses[dis(gen)];
}

// Helper function to get a random day
int getRandomDay() {
    std::uniform_int_distribution<> dis(0, 13); // Assuming 2 weeks, 14 days
    return dis(gen);
}

// Helper function to get a random shift on a random day
int getRandomShift() {
    std::uniform_int_distribution<> dis(0, 2); // Assuming 3 shifts per day
    return dis(gen);
}

/* To my knowledge, these structures only run on nurses of the same type in the same department.
In order to check if the change is feasible, it must improve nurse happiness and also not 
break any of the listed constraints. */

/* Funtion to check if a change is feasible ( the change does not reduce happiness )*/
bool feasible(int currPref, int newPref){
    return currPref >= newPref;
}

/* 
1. Select 2 nurses working back to back shifts
2. Check if a long shift is feasible
    a. If so, swap the nurses out for a long shift
*/

void structure1(){
    string department = "Oncology"; // Example department
    string nurseType = "RN"; // Example nurse type
    int day = getRandomDay();

    Nurse& nurse1 = getRandomNurse(department, nurseType);
    Nurse& nurse2 = getRandomNurse(department, nurseType);

    int shift1 = day * 3;
    int shift2 = day * 3 + 1;

    if (feasible(nurse1.shiftPreferences[shift1], 2) &&
        feasible(nurse2.shiftPreferences[shift2], 2)) {
        // Implement long shift for nurse1
        nurse1.shiftPreferences[shift1] = 2;
        nurse1.shiftPreferences[shift2] = 2;
        // Remove nurse2 from shift2
        nurse2.shiftPreferences[shift2] = 0;
        cout << "Long shift implemented for " << nurse1.fullName << endl;
    }
}

/*
1. Select a day (k) in some week (t)
2. Select 2 nurses
    a. 1 nurse works a back-to-back shift
    b. the other nurse is not scheduled
3. Check if splitting the back-to-back shift between the 2 nurses is feasible
    a. If so, split
*/
void structure2(){
    string department = "Pediatric"; // Example department
    string nurseType = "LPN"; // Example nurse type
    int day = getRandomDay();

    Nurse& nurse1 = getRandomNurse(department, nurseType);
    Nurse& nurse2 = getRandomNurse(department, nurseType);

    int shift1 = day * 3;
    int shift2 = day * 3 + 1;

    if (nurse1.shiftPreferences[shift1] == 2 && nurse1.shiftPreferences[shift2] == 2 &&
        nurse2.shiftPreferences[shift1] == 0 && nurse2.shiftPreferences[shift2] == 0) {
        // Split the back-to-back shift
        nurse1.shiftPreferences[shift2] = 0;
        nurse2.shiftPreferences[shift2] = 2;
        cout << "Back-to-back shift split between " << nurse1.fullName << " and " << nurse2.fullName << endl;
    }
}

/*
1. Randomly select 2 days and 2 nurses working on each day
2. Check if the shift swap between the nurses on different days if feasible
    a. If so, swap
*/
void structure3(){
    string department = "Surgery"; // Example department
    string nurseType = "NA"; // Example nurse type

    int day1 = getRandomDay();
    int day2 = getRandomDay();
    int shift = getRandomShift();

    Nurse& nurse1 = getRandomNurse(department, nurseType);
    Nurse& nurse2 = getRandomNurse(department, nurseType);

    int shiftIndex1 = day1 * 3 + shift;
    int shiftIndex2 = day2 * 3 + shift;

    if (feasible(nurse1.shiftPreferences[shiftIndex2], nurse1.shiftPreferences[shiftIndex1]) &&
        feasible(nurse2.shiftPreferences[shiftIndex1], nurse2.shiftPreferences[shiftIndex2])) {
        // Swap shifts
        swap(nurse1.shiftPreferences[shiftIndex1], nurse2.shiftPreferences[shiftIndex2]);
        cout << "Shifts swapped between " << nurse1.fullName << " and " << nurse2.fullName << endl;
    }
}

/*
1. Select 2 days and 2 nurses
2. Check if swapping the shifts on the same day is feasible
    a. If so, swap
*/
void structure4(){
    string department = "Oncology"; // Example department
    string nurseType = "RN"; // Example nurse type

    int day = getRandomDay();
    int shift1 = getRandomShift();
    int shift2 = (shift1 + 1) % 3;

    Nurse& nurse1 = getRandomNurse(department, nurseType);
    Nurse& nurse2 = getRandomNurse(department, nurseType);

    int shiftIndex1 = day * 3 + shift1;
    int shiftIndex2 = day * 3 + shift2;

    if (feasible(nurse1.shiftPreferences[shiftIndex2], nurse1.shiftPreferences[shiftIndex1]) &&
        feasible(nurse2.shiftPreferences[shiftIndex1], nurse2.shiftPreferences[shiftIndex2])) {
        // Swap shifts
        swap(nurse1.shiftPreferences[shiftIndex1], nurse2.shiftPreferences[shiftIndex2]);
        cout << "Shifts swapped on the same day between " << nurse1.fullName << " and " << nurse2.fullName << endl;
    }
}

/*
1. Randomly choose 2 nurses that work on different days
2. Check if swapping shifts is feasible
    a. If so, swap
*/
void structure5(){
    structure3();
}

/*
1. Select 2 days and 2 nurses per day (might be same nurse might not idk)
2. Check if a clockwise rotation of shifts is feasible
    a. If so, do
*/
void structure6(){
    string department = "Pediatric"; // Example department
    string nurseType = "LPN"; // Example nurse type

    int day1 = getRandomDay();
    int day2 = getRandomDay();

    Nurse& nurse1 = getRandomNurse(department, nurseType);
    Nurse& nurse2 = getRandomNurse(department, nurseType);
    Nurse& nurse3 = getRandomNurse(department, nurseType);
    Nurse& nurse4 = getRandomNurse(department, nurseType);

    int shiftIndex1 = day1 * 3;
    int shiftIndex2 = day1 * 3 + 1;
    int shiftIndex3 = day2 * 3;
    int shiftIndex4 = day2 * 3 + 1;

    if (feasible(nurse2.shiftPreferences[shiftIndex1], nurse1.shiftPreferences[shiftIndex1]) &&
        feasible(nurse3.shiftPreferences[shiftIndex2], nurse2.shiftPreferences[shiftIndex2]) &&
        feasible(nurse4.shiftPreferences[shiftIndex3], nurse3.shiftPreferences[shiftIndex3]) &&
        feasible(nurse1.shiftPreferences[shiftIndex4], nurse4.shiftPreferences[shiftIndex4])) {
        // Perform clockwise rotation
        swap(nurse1.shiftPreferences[shiftIndex1], nurse2.shiftPreferences[shiftIndex2]);
        swap(nurse2.shiftPreferences[shiftIndex2], nurse3.shiftPreferences[shiftIndex3]);
        swap(nurse3.shiftPreferences[shiftIndex3], nurse4.shiftPreferences[shiftIndex4]);
        swap(nurse4.shiftPreferences[shiftIndex4], nurse1.shiftPreferences[shiftIndex1]);
        cout << "Clockwise rotation of shifts performed for 4 nurses" << endl;
    }
}

/* 
1. Choose a day (k), a shift (i) and a nurse (j) 
2. If the nurse can work that shift, schedule it
*/
void structure7(){
    string department = "Surgery"; // Example department
    string nurseType = "NA"; // Example nurse type

    int day = getRandomDay();
    int shift = getRandomShift();
    Nurse& nurse = getRandomNurse(department, nurseType);

    int shiftIndex = day * 3 + shift;

    if (nurse.shiftPreferences[shiftIndex] == 0) {
        nurse.shiftPreferences[shiftIndex] = 1; // Schedule the nurse
        cout << nurse.fullName << " scheduled for shift " << shift << " on day " << day << endl;
    }
}

/*
1. Select a day (k) that has 3 nurses working different shifts on that day
2. Check if cycling the shifts is feasible
    a. If so, do
*/
void structure8(){
        string department = "Oncology"; // Example department
    string nurseType = "RN"; // Example nurse type

    int day = getRandomDay();

    Nurse& nurse1 = getRandomNurse(department, nurseType);
    Nurse& nurse2 = getRandomNurse(department, nurseType);
    Nurse& nurse3 = getRandomNurse(department, nurseType);

    int shiftIndex1 = day * 3;
    int shiftIndex2 = day * 3 + 1;
    int shiftIndex3 = day * 3 + 2;

    if (feasible(nurse2.shiftPreferences[shiftIndex1], nurse1.shiftPreferences[shiftIndex1]) &&
        feasible(nurse3.shiftPreferences[shiftIndex2], nurse2.shiftPreferences[shiftIndex2]) &&
        feasible(nurse1.shiftPreferences[shiftIndex3], nurse3.shiftPreferences[shiftIndex3])) {
        // Cycle shifts
        int temp = nurse1.shiftPreferences[shiftIndex1];
        nurse1.shiftPreferences[shiftIndex1] = nurse3.shiftPreferences[shiftIndex3];
        nurse3.shiftPreferences[shiftIndex3] = nurse2.shiftPreferences[shiftIndex2];
        nurse2.shiftPreferences[shiftIndex2] = temp;
        cout << "Shifts cycled for 3 nurses on day " << day << endl;
}