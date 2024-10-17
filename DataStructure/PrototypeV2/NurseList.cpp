// NurseList.cpp
#include "NurseList.h"
#include <iostream>
#include "json.hpp"  // Include the JSON library
using namespace std;
using json = nlohmann::json;

/**
 * @brief Constructs a Nurse object with the given name, number, pay, and shift data.
 * 
 * @param nurseName The name of the nurse.
 * @param number The unique identifier for the nurse.
 * @param nursePay The pay of the nurse (hourly or annual).
 * @param nurseShifts The shifts assigned to the nurse in a vector of strings.
 */
Nurse::Nurse( const string& nurseName , int number , const string& nurseType, const string& nurseDepartment , const vector< string >& nurseShifts )
    : nurseName( nurseName ) , nurseNumber( number ) , nurseType( nurseType ) ,nurseDepartment( nurseDepartment ), nurseShifts( nurseShifts ) , next( nullptr ) , prev( nullptr ) {}
    
Nurse* NurseList::getHead() const {
    return head;
}

/**
 * @brief Constructs an empty NurseList object.
 * 
 * The constructor initializes the head and tail pointers to nullptr, indicating that the list is empty.
 */
NurseList::NurseList() : head( nullptr ) , tail( nullptr ) {}

/**
 * @brief Adds a new nurse to the end of the list.
 * 
 * @param nurseName The nurse's name.
 * @param number The nurse's unique ID number.
 * @param nursePay The nurse's hourly or annual pay.
 * @param nurseShifts A vector of shifts assigned to the nurse.
 * 
 * This method dynamically allocates memory for a new nurse and appends the nurse to the end of the list. 
 * If the list is empty, the new nurse becomes both the head and the tail of the list. Otherwise, the new nurse is appended to the tail.
 */
void NurseList::addNurse( const string& nurseName , int number , const string& nurseType, const string& nurseDepartment , const vector< string >& nurseShifts ) {

    Nurse* newNurse = new Nurse( nurseName , number , nurseType, nurseDepartment , nurseShifts );

    if ( !head ) {  // If the list is empty, set the new nurse as both head and tail.

        head = tail = newNurse;

    } else {  // Append to the end of the list if otherwise

        tail->next = newNurse;
        newNurse->prev = tail;
        tail = newNurse;

    }

}

/**
 * @brief Displays all nurses in the list
 * 
 * 
 * Iterates through the list starting at the head and outputs the information for each nurse
 * 
 */
void NurseList::display() const {

    Nurse* current = head;

    while ( current ) {

        cout << "nurseName: " << current->nurseName << ", Number: " << current->nurseNumber
                  << ", nurseType: " << current->nurseType << ", nurseDept: " << current->nurseDepartment << ", Shifts: ";

        for ( const auto& shift : current->nurseShifts ) {

            cout << shift << " ";

        }

        cout << endl;
        current = current->next;

    }
}

void NurseList::displayJSON() const {
    Nurse* current = head;
    
    json nursesList = json::array();  // Create a JSON array to hold nurse data
    
    while (current) {
        // Create a JSON object for each nurse
        json nurseJson;
        nurseJson["nurseName"] = current->nurseName;
        nurseJson["nurseNumber"] = current->nurseNumber;
        nurseJson["nurseType"] = current->nurseType;
        nurseJson["nurseDept"] = current->nurseDepartment;
        
        // Convert shifts to JSON array
        nurseJson["Shifts"] = json::array();
        for (const auto& shift : current->nurseShifts) {
            nurseJson["Shifts"].push_back(shift);
        }
        
        // Add nurse JSON object to the list
        nursesList.push_back(nurseJson);
        
        current = current->next;
    }
    
    // Output the JSON
    cout << nursesList.dump(4) << endl;  // Pretty-print with indentation of 4 spaces
}
/**
 * @brief Sorts nurses by satisfaction number for a given shift
 * 
 * @param shiftIndex Shift number to sort satisfaction by
 * 
 *  This function sorts nurses from most willing to least willing to work a given shift, which allows us to pick nurses
 * with a higher satisfaction score
 */
void NurseList::sortNursesByShift(int shiftIndex) {
    if (!head) return;  // If the list is empty, no need to sort.

    bool swapped;
    do {
        swapped = false;
        Nurse* current = head;

        while (current && current->next) {
            // Ensure we are not accessing an out-of-bounds shiftIndex
            if (shiftIndex < current->nurseShifts.size() && shiftIndex < current->next->nurseShifts.size()) {
                int currentShift = stoi(current->nurseShifts[shiftIndex]);
                int nextShift = stoi(current->next->nurseShifts[shiftIndex]);

                // Compare shifts and swap if the current shift is less than the next
                if (currentShift < nextShift) {
                    // Swap nurse details (we're swapping data, not the nodes themselves)
                    swap(current, current->next);
                    swapped = true;
                }
            }
            current = current->next;
        }
    } while (swapped);  // Continue until no swaps are made
}



// Utility function to swap two nurses (swaps their data, not the nodes themselves)
void NurseList::swap(Nurse* a, Nurse* b) {
    std::swap(a->nurseName, b->nurseName);
    std::swap(a->nurseNumber, b->nurseNumber);
    std::swap(a->nurseType, b->nurseType);
    std::swap(a->nurseDepartment, b->nurseDepartment);
    std::swap(a->nurseShifts, b->nurseShifts);
}


