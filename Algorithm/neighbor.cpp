#include "neighbor.h"

using namespace std;

// Random seed
std::random_device rd;
std::mt19937 gen(rd());

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

// Helper function to get a random nurse
Nurse& getRandomNurse(const string& department, const string& type) {
    auto& nurses = departmentNursesMap[department][type];
    std::uniform_int_distribution<> dis(0, nurses.size() - 1);
    return nurses[dis(gen)];
}

// Helper function to get a random assigned nurse
Nurse& getRandomAssignedNurse() {
    int shift = getRandomShift();
                                        //UPdate
    return;
}

/* To my knowledge, these structures only run on nurses of the same type in the same department.
In order to check if the change is feasible, it must improve nurse happiness and also not 
break any of the listed constraints. */

/* Funtion to check if a change is feasible ( the change does not reduce happiness )*/
bool feasible(int currPref, int newPref){       // UPDATE to include constraints, ie can work or not
    if (newPref < currPref){
        return false;
    }
    
    return true;
}


/* 
1. Select 2 nurses working back to back shifts
2. Check if a long shift is feasible
    a. If so, swap the nurses out for a long shift
*/

void structure1(int currPref, string nurseType){
    int day = getRandomDay();
    int shift = getRandomShift();

    Nurse& nurse1 = schedule[shift].getRandomNurse();       // Get the nurse of the selected shift
    Nurse& nurse2 = schedule[shift + 1].getRandomNurse();   // Get the nurse after the selected Shift       UPDATE

    if (nurse1.nurseType != nurseType || nurse2.nurseType != nurseType){
        return;
    }

    if (feasible(currPref, currPref - nurse2.shiftPreferences[shift2] 
    + nurse1.shiftPreferences[shift2])) {                   // UPDATE Check if new schedule is feasible
        // Implement long shift for nurse1
        schedule.add(shift2, nurse1);                       // UPDATE function to add a nurse to a shift
        schedule.remove(shift2, nurse2);                    // Update function to remove nurse from a shift
        //schedule.swap(shift1, nurse 1, shift2, nurse2);   // Update function to swap 2 nurses, might not be needed
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
void structure2(int currPref, string nurseType){
    int shift = getRandomShift();
    int newPref = 0;

    Nurse& nurse1 = schedule[shift].getRandomNurse(nurseType);      // Get a randomly scheduled nurse
    Nurse& nurse2 = getRandomNurse(nurseType);                      // Get a random nurse from the list

    newPref = currPref - nurse1.shiftPreferences[shift + 1] + nurse2.shiftPreferences[shift + 1];
    if (feasible(currPref, newPref)) {
        schedule.add(shift + 1, nurse2);
        schedule.remove(shift + 1, nurse1);
    }

    newPref = currPref - nurse1.shiftPreferences[shift] + nurse2.shiftPreferences[shift];
    else if (feasible(currPref, newPref)) {
        schedule.add(shift, nurse2);
        schedule.remove(shift, nurse1);
    }

    return;
}

/*
1. Randomly select 2 days and 2 nurses working on each day
2. Check if the shift swap between the nurses on different days if feasible
    a. If so, swap
*/
void structure3(int currPref, string nurseType){
    int day1 = getRandomDay();
    int day2 = getRandomDay();
    int shift1 = getRandomShift();
    int shift2 = getRandomShift();

    Nurse& nurse1 = schedule[shift1].getRandomNurse(nurseType);
    Nurse& nurse2 = schedule[shift2].getRandomNurse(nurseType);

    int newPref = currPref - nurse1.shiftPreferences[shift1] + nurse1.shiftPreferences[shift2] 
                  - nurse2.shiftPreferences[shift2] + nures2.shiftPreferences[shift1];
    if (feasible(currPref, newPref)) {
        // Swap shifts
        schedule.add(shift1, nurse2);
        schedule.add(shift2, nurse1);
        schedule.remove(shift1, nurse1);
        schedule.remove(shift2, nurse2);
    }
}

/*
1. Select 2 days and 2 nurses
2. Check if swapping the shifts on the same day is feasible
    a. If so, swap
*/
void structure4(int currPref, string nurseType){
    int day1 = getRandomDay();
    int day2 = getRandomDay();
    int shift1 = getRandomShift();
    int shift2 = (shift1 + 1) % 3;

    Nurse& nurse1 = getRandomNurse(nurseType);
    Nurse& nurse2 = getRandomNurse(nurseType);

    int shiftIndex1 = day * 3 + shift1;
    int shiftIndex2 = day * 3 + shift2;

    if (feasible(nurse1.shiftPreferences[shiftIndex2], nurse1.shiftPreferences[shiftIndex1]) &&
        feasible(nurse2.shiftPreferences[shiftIndex1], nurse2.shiftPreferences[shiftIndex2])) {
        // Swap shifts
        swap(nurse1.shiftPreferences[shiftIndex1], nurse2.shiftPreferences[shiftIndex2]);
    }
}

/*
1. Randomly choose 2 nurses that work on different days
2. Check if swapping shifts is feasible
    a. If so, swap
*/
void structure5(int currPref, string nurseType){
    structure3(int currPref, string nurseType);
}

/*
1. Select 2 days and 2 nurses per day (might be same nurse might not idk)
2. Check if a clockwise rotation of shifts is feasible
    a. If so, do
*/
void structure6(int currPref, string nurseType){
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
    }
}

/* 
1. Choose a day (k), a shift (i) and a nurse (j) 
2. If the nurse can work that shift, schedule it
*/
void structure7(int currPref, string nurseType){
    string department = "Surgery"; // Example department
    string nurseType = "NA"; // Example nurse type

    int day = getRandomDay();
    int shift = getRandomShift();
    Nurse& nurse = getRandomNurse(department, nurseType);

    int shiftIndex = day * 3 + shift;

    if (nurse.shiftPreferences[shiftIndex] == 0) {
        nurse.shiftPreferences[shiftIndex] = 1; // Schedule the nurse
    }
}

/*
1. Select a day (k) that has 3 nurses working different shifts on that day
2. Check if cycling the shifts is feasible
    a. If so, do
*/
void structure8(int currPref, string nurseType){
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
    }
}