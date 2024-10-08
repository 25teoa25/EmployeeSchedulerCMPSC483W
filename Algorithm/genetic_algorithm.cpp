#include <iostream> // input/output operations (like std::cout for printing).
#include <vector> //std::vector, a dynamic array 
#include <algorithm> //std::sort
#include <random> //RANDOM NUMBER

// Constants
const int TOTAL_DEPARTMENTS = 21;       
const int SHIFTS_PER_DAY = 3;           //morning, afternoon, night
const int DAYS_PER_WEEK = 7;            
const int WEEKS = 2;                    // Scheduling period is 2 weeks
const int SHIFTS_IN_PERIOD = SHIFTS_PER_DAY * DAYS_PER_WEEK * WEEKS;  // Total shifts in 2 weeks
const int MAX_SHIFTS_PER_NURSE = 10;    // Maximum 10 shifts per nurse in 2 weeks
const int MAX_CONSECUTIVE_SHIFTS = 2;   // Maximum 2 consecutive shifts

//storing nurse info
struct Nurse {
    int id;
    int department;
    std::vector<int> preferences;  //0 = Not available, 1 = Available, 2 = Preferred
};

//storing shift info
struct Shift {
    int nurseId;  
    int day;      // 0-13 for 2 weeks
    int timeSlot; // 0 = Morning, 1 = Afternoon, 2 = Night
};
