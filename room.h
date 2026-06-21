#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <vector>
using namespace std;
#include "student.h"
class Room
{
private:
    int roomNumber;
    int capacity;
    vector<Student *> students;

public:
    // constructors
    Room();
    Room(int roomNumber, int capacity);
    // destructor
    ~Room();
    // getters
    int getRoomNumber() const;
    int getCapacity() const;
    int getOccupancy() const;
    bool isFull() const;
    bool isEmpty() const;

    // Student management
    bool addStudent(Student *s);
    bool removeStudent(const string &studentId);
    Student* findStudent(const string &studentId) const;

    // Display
    void display() const;
};
#endif