// NurseList.cpp
#include "NurseList.h"

// Constructor for Nurse struct
Nurse::Nurse( const std::string& nurseName , int number , double nursePay , const std::vector< std::string >& nurseShifts )
    : nurseName( nurseName ) , nurseNumber( number ) , nursePay( nursePay ) , nurseShifts( nurseShifts ) , next( nullptr ) , prev( nullptr ) {}

// Constructor for NurseList class
NurseList::NurseList() : head( nullptr ) , tail( nullptr ) {}

// Add a nurse to the linked list
void NurseList::addNurse( const std::string& nurseName , int number , double nursePay , const std::vector< std::string >& nurseShifts ) {

    Nurse* newNurse = new Nurse( nurseName , number , nursePay , nurseShifts );

    if ( !head ) {  // If the list is empty

        head = tail = newNurse;

    } else {  // Append to the end of the list

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
