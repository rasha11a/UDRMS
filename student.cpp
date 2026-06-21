#include "student.h"
#include <iostream>
using namespace std;

Student::Student()
    : id(""), fullName(""), academicYear(1),
    dormitoryName(""), roomNumber(-1) {}

Student::Student(const string& id, const string& fullName, int academicYear)
    : id(id), fullName(fullName), academicYear(academicYear),
    dormitoryName(""), roomNumber(-1) {}

string Student::getId() const { return id; }
string Student::getFullName() const { return fullName; }
int Student::getAcademicYear() const { return academicYear; }
string Student::getDormitoryName() const { return dormitoryName; }
int Student::getRoomNumber() const { return roomNumber; }
bool Student::isAssigned() const { return roomNumber != -1; }

void Student::setId(const string& id) { this->id = id; }
void Student::setFullName(const string& fullName) { this->fullName = fullName; }
void Student::setAcademicYear(int year) { this->academicYear = year; }

void Student::setAccommodation(const string& dormitoryName, int roomNumber) {
    this->dormitoryName = dormitoryName;
    this->roomNumber = roomNumber;
}

void Student::clearAccommodation() {
    dormitoryName = "";
    roomNumber = -1;
}

void Student::display() const {
    cout << "----------------------------" << endl;
    cout << "Student ID   : " << id << endl;
    cout << "Full Name    : " << fullName << endl;
    cout << "Academic Year: " << academicYear << endl;
    if (isAssigned()) {
        cout << "Dormitory    : " << dormitoryName << endl;
        cout << "Room Number  : " << roomNumber << endl;
    } else {
        cout << "Accommodation: Not assigned" << endl;
    }
    cout << "----------------------------" << endl;
}