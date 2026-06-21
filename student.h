#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

class Student {
private:
    string id;
    string fullName;
    int academicYear;
    string dormitoryName;  // "" if not assigned
    int roomNumber;        // -1 if not assigned

public:
    // Constructors
    Student();
    Student(const string& id, const string& fullName, int academicYear);

    // Getters
    string getId() const;
    string getFullName() const;
    int getAcademicYear() const;
    string getDormitoryName() const;
    int getRoomNumber() const;
    bool isAssigned() const;   // true if living in a dorm/room

    // Setters
    void setId(const string& id);
    void setFullName(const string& fullName);
    void setAcademicYear(int year);
    void setAccommodation(const string& dormitoryName, int roomNumber);
    void clearAccommodation();  // called when removed from a room

    // Display
    void display() const;
};

#endif // STUDENT_H