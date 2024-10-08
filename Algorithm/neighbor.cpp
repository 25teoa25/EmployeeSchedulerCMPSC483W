#include <iostream>
#include <unordered_map>
#include "../DataStructure/PrototypeV2/NurseFunctions.h"
#include "../DataStructure/PrototypeV2/CSVParser.h"
#include "../DataStructure/PrototypeV2/NurseList.h"
#include <random>
using namespace std;

/*
To my knowledge, these structures only run on nurses of the same type in the same department.
In order to check if the change is feasible, it must improve nurse happiness and also not 
break any of the listed constraints.
*/

/* 
1. Select 2 nurses working back to back shifts
2. Check if a long shift is feasible
    a. If so, swap the nurses out for a long shift
*/
void structure1(){

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

}

/*
1. Randomly select 2 days and 2 nurses working on each day
2. Check if the shift swap between the nurses on different days if feasible
    a. If so, swap
*/
void structure3(){

}

/*
1. Select 2 days and 2 nurses
2. Check if swapping the shifts on the same day is feasible
    a. If so, swap
*/
void structure4(){
    
}

/*
1. Randomly choose 2 nurses that work on different days
2. Check if swapping shifts is feasible
    a. If so, swap
*/
void structure5(){
    
}

/*
1. Select 2 days and 2 nurses per day (might be same nurse might not idk)
2. Check if a clockwise rotation of shifts is feasible
    a. If so, do
*/
void structure6(){
    
}

/* 
1. Choose a day (k), a shift (i) and a nurse (j) 
2. If the nurse can work that shift, schedule it
*/
void structure7(){
    
}

/*
1. Select a day (k) that has 3 nurses working different shifts on that day
2. Check if cycling the shifts is feasible
    a. If so, do
*/
void structure8(){
    
}