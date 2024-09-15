// NurseList.h
#ifndef NURSE_LIST_H
#define NURSE_LIST_H

#include <iostream>
#include <vector>
#include <string>

// Nurse Information Class
struct Nurse {

    std::string nurseName;
    int nurseNumber;
    double nursePay;
    std::vector< std::string > nurseShifts;  // Store 42 shifts in a vector
    Nurse* next;
    Nurse* prev;

    Nurse( const std::string& nurseName , int number , double nursePay , const std::vector< std::string >& nurseShifts );

};

// Class for the doubly linked list
class NurseList {
public:

    NurseList();
    void addNurse( const std::string& nurseName , int number , double nursePay , const std::vector< std::string >& nurseShifts );
    void display() const;

private:

    Nurse* head;
    Nurse* tail;

};

#endif // NURSE_LIST_H
