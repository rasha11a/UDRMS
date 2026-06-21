#ifndef DORMITORY_H
#define DORMITORY_H

#include <string>
#include <vector>
#include "room.h"
#include "student.h"
#include "restaurant.h"
using namespace std;

class Dormitory {
private:
    string name;
    int capacity;
    vector<Room*> rooms;
    vector<Student*> students;  // all students in this dormitory
    Restaurant restaurant;       // composition: each dorm has exactly one

public:
    // Constructors
    Dormitory();
    Dormitory(const string& name, int capacity);

    // Destructor
    ~Dormitory();

    // Getters
    string getName() const;
    int getCapacity() const;
    int getTotalOccupancy() const;
    Restaurant& getRestaurant();
    const vector<Room*>& getRooms() const;
    const vector<Student*>& getStudents() const;

    // Room management
    void addRoom(int roomNumber, int roomCapacity);
    Room* findRoom(int roomNumber) const;
    void displayRooms() const;
    void displayAvailableRooms() const;

    // Student management
    bool addStudent(Student* s, int roomNumber);
    bool removeStudent(const string& studentId);
    Student* findStudent(const string& studentId) const;
    void displayStudents() const;

    // Display
    void display() const;
};

#endif // DORMITORY_H