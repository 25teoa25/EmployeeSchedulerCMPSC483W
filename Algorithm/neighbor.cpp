#include "neighbor.h"

using namespace std;

// Random seed
std::random_device rd;
std::mt19937 gen(rd());

// Helper function to get a random day
// 1 - Indexed
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
    int ranDay = getRandomDay();
    int ranShift = getRandomShift();
    int shift = ranDay * 3 + ranShift;

    Nurse& nurse1 = schedule[shift].getRandomNurse(nurseType);      // Get a randomly scheduled nurse
    Nurse& nurse2 = getRandomNurse(nurseType);                      // Get a random nurse from the list

    int newPref1 = currPref - nurse1.shiftPreferences[shift + 1] + nurse2.shiftPreferences[shift + 1];
    int newPref2 = currPref - nurse1.shiftPreferences[shift] + nurse2.shiftPreferences[shift];
    if (feasible(currPref, newPref1)) {
        schedule.add(shift + 1, nurse2);
        schedule.remove(shift + 1, nurse1);
    }

    else if (feasible(currPref, newPref2)) {
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
    int ranShift1 = getRandomShift();
    int ranShift2 = getRandomShift();

    int shift1 = day1 * 3 + ranShift1;
    int shift2 = day1 * 3 + ranShift2;
    int shift3 = day2 * 3 + ranShift1;
    int shift4 = day2 * 3 + ranShift2;

    Nurse& nurse1 = schedule[shift1].getRandomNurse(nurseType);
    Nurse& nurse2 = schedule[shift2].getRandomNurse(nurseType);
    Nurse& nurse3 = schedule[shift3].getRandomNurse(nurseType);
    Nurse& nurse4 = schedule[shift4].getRandomNurse(nurseType);


    int newPref = currPref 
                - nurse1.shiftPreferences[shift1] + nurse1.shiftPreferences[shift3] 
                - nurse2.shiftPreferences[shift2] + nures2.shiftPreferences[shift4]
                - nurse3.shiftPreferences[shift3] + nurse3.shiftPreferences[shift1]
                - nurse4.shiftPreferences[shift4] + nurse4.shiftPreferences[shift2];
    
    if (feasible(currPref, newPref)) {
        // Swap shifts
        schedule.add(shift3, nurse1);
        schedule.remove(shift1, nurse1);
        schedule.add(shift4, nurse2);
        schedule.remove(shift2, nurse2);
        schedule.add(shift1, nurse3);
        schedule.remove(shift3, nurse3);
        schedule.add(shift2, nurse4);
        schedule.remove(shift4, nurse4);
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
    int ranShift1 = getRandomShift();
    int ranShift2 = getRandomShift();

    int shift1 = day1 * 3 + ranShift1;
    int shift2 = day1 * 3 + ranShift2;
    int shift3 = day2 * 3 + ranShift1;
    int shift4 = day2 * 3 + ranShift2;

    Nurse& nurse1 = schedule[shift1].getRandomNurse(nurseType);
    Nurse& nurse2 = schedule[shift2].getRandomNurse(nurseType);
    Nurse& nurse3 = schedule[shift3].getRandomNurse(nurseType);
    Nurse& nurse4 = schedule[shift4].getRandomNurse(nurseType);

    int newPref = currPref 
                - nurse1.shiftPreferences[shift1] + nurse1.shiftPreferences[shift2] 
                - nurse2.shiftPreferences[shift2] + nures2.shiftPreferences[shift1]
                - nurse3.shiftPreferences[shift3] + nurse3.shiftPreferences[shift4]
                - nurse4.shiftPreferences[shift4] + nurse4.shiftPreferences[shift3];


    if (feasible(currPref, newPref)) {
        // Swap shifts
        schedule.add(shift2, nurse1);
        schedule.remove(shift1, nurse1);
        schedule.add(shift1, nurse2);
        schedule.remove(shift2, nurse2);
        schedule.add(shift4, nurse3);
        schedule.remove(shift3, nurse3);
        schedule.add(shift3, nurse4);
        schedule.remove(shift4, nurse4);
    }
}

/*
1. Randomly choose 2 nurses that work on different days
2. Check if swapping shifts is feasible
    a. If so, swap
*/
void structure5(int currPref, string nurseType){
    structure3(currPref, nurseType);
}

/*
1. Select 2 days and 2 nurses per day (might be same nurse might not idk)
2. Check if a clockwise rotation of shifts is feasible
    a. If so, do
*/
void structure6(int currPref, string nurseType){
    int day1 = getRandomDay();
    int day2 = getRandomDay();
    int ranShift1 = getRandomShift();
    int ranShift2 = getRandomShift();

    int shift1 = day1 * 3 + ranShift1;
    int shift2 = day1 * 3 + ranShift2;
    int shift3 = day2 * 3 + ranShift1;
    int shift4 = day2 * 3 + ranShift2;

    Nurse& nurse1 = schedule[shift1].getRandomNurse(nurseType);
    Nurse& nurse2 = schedule[shift2].getRandomNurse(nurseType);
    Nurse& nurse3 = schedule[shift3].getRandomNurse(nurseType);
    Nurse& nurse4 = schedule[shift4].getRandomNurse(nurseType);

    int newPref = currPref 
                - nurse1.shiftPreferences[shift1] + nurse1.shiftPreferences[shift3] 
                - nurse2.shiftPreferences[shift2] + nures2.shiftPreferences[shift1]
                - nurse3.shiftPreferences[shift3] + nurse3.shiftPreferences[shift4]
                - nurse4.shiftPreferences[shift4] + nurse4.shiftPreferences[shift2];

    if (feasible(currPref, newPref)) {
        // Perform clockwise rotation
        schedule.add(shift3, nurse1);
        schedule.remove(shift1, nurse1);
        schedule.add(shift1, nurse2);
        schedule.remove(shift2, nurse2);
        schedule.add(shift4, nurse3);
        schedule.remove(shift3, nurse3);
        schedule.add(shift2, nurse4);
        schedule.remove(shift4, nurse4);
    }
}

/* 
1. Choose a day (k), a shift (i) and a nurse (j) 
2. If the nurse can work that shift, schedule it
*/
void structure7(int currPref, string nurseType){
    int day = getRandomDay();
    int shift = getRandomShift();
    Nurse& nurse = getRandomNurse(nurseType);

    int shiftIndex = day * 3 + shift;
    int newPref = currPref + nurse.shiftPreferences[shiftIndex];

    if (feasible(currPref, newPref)) {                              // I dont think this newPref check is needed, cause it will always be same or greater
        schedule.add(shiftIndex, nurse); // Schedule the nurse
    }
}

/*
1. Select a day (k) that has 3 nurses working different shifts on that day
2. Check if cycling the shifts is feasible
    a. If so, do
*/
void structure8(int currPref, string nurseType){
    int day = getRandomDay();
    
    int shift1 = day * 3;
    int shift2 = day * 3 + 1;
    int shift3 = day * 3 + 2;

    Nurse& nurse1 = schedule[shift1].getRandomNurse(nurseType);
    Nurse& nurse2 = schedule[shift2]getRandomNurse(nurseType);
    Nurse& nurse3 = schedule[shift3]getRandomNurse(nurseType);

    int newPref = currPref
                - nurse1.shiftPreferences[shift1] + nurse1.shiftPreferences[shift2]
                - nurse2.shiftPreferences[shift2] + nurse2.shiftPreferences[shift3]
                - nurse3.shiftPreferences[shift3] + nurse3.shiftPreferences[shift1];
    
    if (feasible(currPref, newPref)) {
        // Cycle shifts
        schedule.add(shift2, nurse1);
        schedule.remove(nurse1, nurse1);
        schedule.add(shift3, nurse2);
        schedule.remove(nurse2, nurse2);
        schedule.add(shift1, nurse3);
        schedule.remove(shift3, nurse3);
    }
}