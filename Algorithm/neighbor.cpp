#include "neighbor.h"
#include "../DataStructure/LinkedListDS/NurseFunctions.h"

using namespace std;

/* Things to fix:

BIGGEST PROBLEM: THERE IS NO WAY OF CHOOSING A NURSE OF AA SPECIFIC TYPE THAT HAS ALREADY BEEN SCHEUDLED

1. There is no way of choosing a random nurse of a specific type working a shift
2. Need to be able to
3. Actively update the new preference of the scheudle
4. chnge feasible to check if they are legally allowed to work*/

// Random seed
std::random_device rd;
std::mt19937 gen(rd());

// Helper function to get a random day
// 1 - Indexed
int getRandomDay()
{
    std::uniform_int_distribution<> dis(0, 13); // Assuming 2 weeks, 14 days
    return dis(gen);
}

// Helper function to get a random shift on a random day
int getRandomShift()
{
    std::uniform_int_distribution<> dis(1, 3); // Assuming 3 shifts per day
    return dis(gen);
}

// Helper function to get a random nurse
Nurse &getRandomNurse(const string &department, const string &type)
{
    auto &nurses = departmentNursesMap[department][type];
    std::uniform_int_distribution<> dis(0, nurses.size() - 1);
    return nurses[dis(gen)];
}

// Helper function to get a random assigned nurse
Nurse &getRandomAssignedNurse(ShiftSchedule schedule, int shift)
{
    std::uniform_int_distribution<> dis(0, schedule[shift].size() - 1);
    // Not sure how schedule stores nurses
    // UPDATE? : to find nurses of a specific type?
    return schedule[shift][dis(gen)];
}

/* To my knowledge, these structures only run on nurses of the same type in the same department.
In order to check if the change is feasible, it must improve nurse happiness and also not
break any of the listed constraints. */

/* Funtion to check if a change is feasible ( the change does not reduce happiness )*/
bool feasible(int currPref, int newPref)
{
    // UPDATE to include constraints, ie can work or not
    if (newPref < currPref)
    {
        return false;
    }

    return true;
}

/*
1. Select 2 nurses working back to back shifts
2. Check if a long shift is feasible
    a. If so, swap the nurses out for a long shift
*/

int structure1(ShiftSchedule schedule, const string &department, int currPref, const string nurseType)
{
    // Generate a random shift
    int day = getRandomDay();
    int shift = getRandomShift();
    int shift1 = day * 3 + shift;
    int shift2 = shift1 + 1;

    // Generate 1 random nurse per shift
    Nurse nurse1 = getRandomNurseFromShift(schedule, shift1, nurseType);
    Nurse nurse2 = getRandomNurseFromShift(schedule, shift2, nurseType);

    // If the selected nurse is not of the correct type return
    if (nurse1.nurseType != nurseType || nurse2.nurseType != nurseType)
    {
        return;
    }

    // If nurse1 can take the long shift
    if (feasible(currPref, currPref - nurse2.shiftPreferences[shift2] + nurse1.shiftPreferences[shift2]))
    {
        add(schedule, shift2, nurse1);
        remove(schedule, shift2, nurse2);
        return currPref - nurse2.shiftPreferences[shift2] + nurse1.shiftPreferences[shift2];
    }

    // If nurse2 can take the long shift
    else if (feasible(currPref, currPref - nurse1.shiftPreferences[shift1] + nurse2.shiftPreferences[shift1]))
    {
        add(schedule, shift1, nurse2);
        remove(schedule, shift1, nurse1);
        return currPref - nurse1.shiftPreferences[shift1] + nurse2.shiftPreferences[shift1];
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
int structure2(ShiftSchedule schedule, const string &department, int currPref, string nurseType)
{
    // Generate a random shift
    int ranDay = getRandomDay();
    int ranShift = getRandomShift();
    int shift = ranDay * 3 + ranShift;

    // Generate 2 nurses, 1) works a long shift 2) does not work either shift
    Nurse nurse1 = getRandomNurseFromShift(schedule, shift, nurseType);
    Nurse &nurse2 = getRandomNurse(department, nurseType);

    if (nurse1.nurseType != nurse2.nurseType)
    {
        // Update to verify nurse1 works a long shift
        return;
    }

    int newPref1 = currPref - nurse1.shiftPreferences[shift + 1] + nurse2.shiftPreferences[shift + 1];
    int newPref2 = currPref - nurse1.shiftPreferences[shift] + nurse2.shiftPreferences[shift];

    // If splitting the shift is feasible, do it
    if (feasible(currPref, newPref1))
    {
        add(schedule, shift, nurse2);
        remove(schedule, shift, nurse1);
        return newPref1;
    }

    else if (feasible(currPref, newPref2))
    {
        add(schedule, shift, nurse2);
        remove(schedule, shift, nurse1);
        return newPref2;
    }

    return;
}

/*
1. Randomly select 2 days and 2 nurses working on each day
2. Check if the shift swap between the nurses on different days if feasible
    a. If so, swap
*/
int structure3(ShiftSchedule schedule, const string &department, int currPref, string nurseType)
{
    int day1 = getRandomDay();
    int day2 = getRandomDay();
    int ranShift1 = getRandomShift();
    int ranShift2 = getRandomShift();

    int shift1 = day1 * 3 + ranShift1;
    int shift2 = day1 * 3 + ranShift2;
    int shift3 = day2 * 3 + ranShift1;
    int shift4 = day2 * 3 + ranShift2;

    Nurse nurse1 = getRandomNurseFromShift(schedule, shift1, nurseType);
    Nurse nurse2 = getRandomNurseFromShift(schedule, shift2, nurseType);
    Nurse nurse3 = getRandomNurseFromShift(schedule, shift3, nurseType);
    Nurse nurse4 = getRandomNurseFromShift(schedule, shift4, nurseType);

    // UPDATE: Need to ensure all nurses are of the saame type

    int newPref = currPref - nurse1.shiftPreferences[shift1] + nurse1.shiftPreferences[shift3] -
                  nurse2.shiftPreferences[shift2] + nurse2.shiftPreferences[shift4] - nurse3.shiftPreferences[shift3] + nurse3.shiftPreferences[shift1] - nurse4.shiftPreferences[shift4] + nurse4.shiftPreferences[shift2];

    if (feasible(currPref, newPref))
    {
        // Swap shifts
        add(schedule, shift3, nurse1);
        remove(schedule, shift1, nurse1);
        add(schedule, shift4, nurse2);
        remove(schedule, shift2, nurse2);
        add(schedule, shift1, nurse3);
        remove(schedule, shift3, nurse3);
        add(schedule, shift2, nurse4);
        remove(schedule, shift4, nurse4);
        return newPref;
    }
}

/*
1. Select 2 days and 2 nurses
2. Check if swapping the shifts on the same day is feasible
    a. If so, swap
*/
int structure4(ShiftSchedule schedule, const string &department, int currPref, string nurseType)
{
    int day1 = getRandomDay();
    int day2 = getRandomDay();
    int ranShift1 = getRandomShift();
    int ranShift2 = getRandomShift();

    int shift1 = day1 * 3 + ranShift1;
    int shift2 = day1 * 3 + ranShift2;
    int shift3 = day2 * 3 + ranShift1;
    int shift4 = day2 * 3 + ranShift2;

    Nurse nurse1 = getRandomNurseFromShift(schedule, shift1, nurseType);
    Nurse nurse2 = getRandomNurseFromShift(schedule, shift2, nurseType);
    Nurse nurse3 = getRandomNurseFromShift(schedule, shift3, nurseType);
    Nurse nurse4 = getRandomNurseFromShift(schedule, shift4, nurseType);

    int newPref = currPref - nurse1.shiftPreferences[shift1] + nurse1.shiftPreferences[shift2] - nurse2.shiftPreferences[shift2] + nurse2.shiftPreferences[shift1] - nurse3.shiftPreferences[shift3] + nurse3.shiftPreferences[shift4] - nurse4.shiftPreferences[shift4] + nurse4.shiftPreferences[shift3];

    if (feasible(currPref, newPref))
    {
        // Swap shifts
        add(schedule, shift2, nurse1);
        remove(schedule, shift1, nurse1);
        add(schedule, shift1, nurse2);
        remove(schedule, shift2, nurse2);
        add(schedule, shift4, nurse3);
        remove(schedule, shift3, nurse3);
        add(schedule, shift3, nurse4);
        remove(schedule, shift4, nurse4);
        return newPref;
    }
}

/*
1. Randomly choose 2 nurses that work on different days
2. Check if swapping shifts is feasible
    a. If so, swap
*/
int structure5(ShiftSchedule schedule, const string &department, int currPref, string nurseType)
{
    return structure3(schedule, department, currPref, nurseType);
}

/*
1. Select 2 days and 2 nurses per day (might be same nurse might not idk)
2. Check if a clockwise rotation of shifts is feasible
    a. If so, do
*/
int structure6(ShiftSchedule schedule, const string &department, int currPref, string nurseType)
{
    int day1 = getRandomDay();
    int day2 = getRandomDay();
    int ranShift1 = getRandomShift();
    int ranShift2 = getRandomShift();

    int shift1 = day1 * 3 + ranShift1;
    int shift2 = day1 * 3 + ranShift2;
    int shift3 = day2 * 3 + ranShift1;
    int shift4 = day2 * 3 + ranShift2;

    Nurse nurse1 = getRandomNurseFromShift(schedule, shift1, nurseType);
    Nurse nurse2 = getRandomNurseFromShift(schedule, shift2, nurseType);
    Nurse nurse3 = getRandomNurseFromShift(schedule, shift3, nurseType);
    Nurse nurse4 = getRandomNurseFromShift(schedule, shift4, nurseType);

    int newPref = currPref - nurse1.shiftPreferences[shift1] + nurse1.shiftPreferences[shift3] - nurse2.shiftPreferences[shift2] + nurse2.shiftPreferences[shift1] - nurse3.shiftPreferences[shift3] + nurse3.shiftPreferences[shift4] - nurse4.shiftPreferences[shift4] + nurse4.shiftPreferences[shift2];

    if (feasible(currPref, newPref))
    {
        // Perform clockwise rotation
        add(schedule, shift3, nurse1);
        remove(schedule, shift1, nurse1);
        add(schedule, shift1, nurse2);
        remove(schedule, shift2, nurse2);
        add(schedule, shift4, nurse3);
        remove(schedule, shift3, nurse3);
        add(schedule, shift2, nurse4);
        remove(schedule, shift4, nurse4);
        return newPref;
    }
}

/*
1. Choose a day (k), a shift (i) and a nurse (j)
2. If the nurse can work that shift, schedule it
*/
int structure7(ShiftSchedule schedule, const string &department, int currPref, string nurseType)
{
    int day = getRandomDay();
    int shift = getRandomShift();
    Nurse &nurse = getRandomNurse(department, nurseType);

    int shiftIndex = day * 3 + shift;
    int newPref = currPref + nurse.shiftPreferences[shiftIndex];

    if (feasible(currPref, newPref))
    {                                     // I dont think this newPref check is needed, cause it will always be same or greater
        add(schedule, shiftIndex, nurse); // Schedule the nurse
        return newPref;
    }
}

/*
1. Select a day (k) that has 3 nurses working different shifts on that day
2. Check if cycling the shifts is feasible
    a. If so, do
*/
int structure8(ShiftSchedule schedule, const string &department, int currPref, string nurseType)
{
    int day = getRandomDay();

    int shift1 = day * 3;
    int shift2 = day * 3 + 1;
    int shift3 = day * 3 + 2;

    Nurse nurse1 = getRandomNurseFromShift(schedule, shift1, nurseType);
    Nurse nurse2 = getRandomNurseFromShift(schedule, shift2, nurseType);
    Nurse nurse3 = getRandomNurseFromShift(schedule, shift3, nurseType);

    int newPref = currPref - nurse1.shiftPreferences[shift1] + nurse1.shiftPreferences[shift2] - nurse2.shiftPreferences[shift2] + nurse2.shiftPreferences[shift3] - nurse3.shiftPreferences[shift3] + nurse3.shiftPreferences[shift1];

    if (feasible(currPref, newPref))
    {
        // Cycle shifts
        add(schedule, shift2, nurse1);
        remove(schedule, shift1, nurse1);
        add(schedule, shift3, nurse2);
        remove(schedule, shift2, nurse2);
        add(schedule, shift1, nurse3);
        remove(schedule, shift3, nurse3);
        return newPref;
    }
}