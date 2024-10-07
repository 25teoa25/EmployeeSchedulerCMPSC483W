// NurseList.h
#ifndef NURSE_LIST_H
#define NURSE_LIST_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

/**
 * @struct Nurse
 * @brief A structure that stores information about a nurse, including their name, ID number, pay, and work shifts.
 * 
 * The Nurse structure holds details about a nurse and is used in the doubly linked list for storing nurse records.
 */
struct Nurse {

    string nurseName; /**< Nurse's name. */
    int nurseNumber; /**< Unique nurse identifier (e.g., employee number). */
    string nurseType; /**< Nurse's type. */
    string nurseDepartment; /**< Nurse's dept. */
    vector< string > nurseShifts;  /**< A vector containing the nurse's assigned shifts (e.g., 42 shifts). */
    Nurse* next; /**< Pointer to the next nurse in the linked list. */
    Nurse* prev; /**< Pointer to the previous nurse in the linked list. */

    /**
     * @brief Constructs a Nurse object.
     * 
     * @param nurseName The nurse's name.
     * @param number The nurse's unique ID number.
     * @param nurseType 
     * @param nurseDept
     */
    Nurse( const string& nurseName , int number , const string& nurseType , const string& nurseDepartment, const vector< string >& nurseShifts );

};

/**
 * @class NurseList
 * @brief A doubly linked list class for storing and managing a collection of nurses.
 * 
 * The NurseList class allows adding nurses to a doubly linked list and displaying the list.
 */
class NurseList {
public:
    /**
     * @brief Constructs an empty NurseList object.
     * 
     * Initializes an empty doubly linked list where both head and tail pointers are set to nullptr.
     */
    NurseList();

    /**
     * @brief Adds a new nurse to the list.
     * 
     * @param nurseName The nurse's name.
     * @param number The nurse's unique ID number.
     * @param nurseType 
     * @param nurseDept
     * 
     * The new nurse is added to the end of the doubly linked list.
     */
    void addNurse( const string& nurseName , int number , const string& nurseType , const string& nurseDepartment, const vector< string >& nurseShifts );

    /**
     * @brief Displays all nurses in the list.
     * 
     * This function traverses the list and prints out details of each nurse.
     */
    void display() const;

    Nurse* getHead() const;

    void sortNursesByShift(int shiftIndex);

    void swap(Nurse* a, Nurse* b);

private:

    Nurse* head; /**< Pointer to the first nurse in the linked list. */
    Nurse* tail; /**< Pointer to the last nurse in the linked list. */

};

#endif // NURSE_LIST_H
