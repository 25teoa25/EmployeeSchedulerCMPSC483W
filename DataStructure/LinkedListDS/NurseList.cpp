// NurseList.cpp
#include "NurseList.h"

/**
 * @brief Constructs a Nurse object with the given name, number, pay, and shift data.
 * 
 * @param nurseName The name of the nurse.
 * @param number The unique identifier for the nurse.
 * @param nursePay The pay of the nurse (hourly or annual).
 * @param nurseShifts The shifts assigned to the nurse in a vector of strings.
 */
Nurse::Nurse( const std::string& nurseName , int number , double nursePay , const std::vector< std::string >& nurseShifts )
    : nurseName( nurseName ) , nurseNumber( number ) , nursePay( nursePay ) , nurseShifts( nurseShifts ) , next( nullptr ) , prev( nullptr ) {}
    
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
void NurseList::addNurse( const std::string& nurseName , int number , double nursePay , const std::vector< std::string >& nurseShifts ) {

    Nurse* newNurse = new Nurse( nurseName , number , nursePay , nurseShifts );

    if ( !head ) {  // If the list is empty, set the new nurse as both head and tail.

        head = tail = newNurse;

    } else {  // Append to the end of the list if otherwise

        tail->next = newNurse;
        newNurse->prev = tail;
        tail = newNurse;

    }

}

// Display all nurses in the list
void NurseList::display() const {

    Nurse* current = head;

    while ( current ) {

        std::cout << "nurseName: " << current->nurseName << ", Number: " << current->nurseNumber
                  << ", nursePay: " << current->nursePay << ", Shifts: ";

        for ( const auto& shift : current->nurseShifts ) {

            std::cout << shift << " ";

        }

        std::cout << std::endl;
        current = current->next;

    }

}
