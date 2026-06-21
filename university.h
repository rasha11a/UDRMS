#ifndef UNIVERSITY_H
#define UNIVERSITY_H

#include <string>
#include <vector>
#include "Dormitory.h"
using namespace std;

class University {
private:
    string name;
    vector<Dormitory*> dormitories;

public:
    // Constructors
    University();
    University(const string& name);

    // Destructor
    ~University();

    // Getters
    string getName() const;
    int getTotalStudents() const;
    int getTotalRooms() const;
    const vector<Dormitory*>& getDormitories() const;

    // Dormitory management
    void addDormitory(const string& dormName, int capacity);
    Dormitory* findDormitory(const string& dormName) const;
    void displayDormitories() const;

    // Student management (university-wide)
    bool enrollStudent(const string& id,const string& fullName,int academicYear, const string& dormName,int roomNumber);

    bool removeStudent(const string& studentId, const string& dormName);

    Student* findStudent(const string& studentId) const;

    // Display
    void displayAll() const;
    void displaySummary() const;
};

#endif // UNIVERSITY_H