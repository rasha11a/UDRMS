#include "Room.h"
#include <iostream>
using namespace std;

// Default constructor
Room::Room() : roomNumber(0), capacity(2) {}

// Parameterized constructor
Room::Room(int roomNumber, int capacity)
    : roomNumber(roomNumber), capacity(capacity) {}

// Destructor — we do NOT delete students here
// because Dormitory owns the students, not the Room
Room::~Room()
{
    students.clear();
}

// Getters
int Room::getRoomNumber() const { return roomNumber; }
int Room::getCapacity() const { return capacity; }
int Room::getOccupancy() const { return students.size(); }
bool Room::isFull() const { return (int)students.size() >= capacity; }
bool Room::isEmpty() const { return students.empty(); }

// Add a student to this room
bool Room::addStudent(Student *s)
{
    if (isFull())
    {
        cout << "Room " << roomNumber << " is full!" << endl;
        return false;
    }
    students.push_back(s);
    return true;
}

// Remove a student by ID
bool Room::removeStudent(const string &studentId)
{
    for (auto it = students.begin(); it != students.end(); ++it)
    {
        if ((*it)->getId() == studentId)
        {
            students.erase(it);
            return true;
        }
    }
    cout << "Student " << studentId << " not found in room " << roomNumber << endl;
    return false;
}

// Find a student by ID
Student *Room::findStudent(const string &studentId) const
{
    for (Student *s : students)
    {
        if (s->getId() == studentId)
            return s;
    }
    return nullptr;
}

// Display room info and its occupants
void Room::display() const
{
    cout << "=============================" << endl;
    cout << "Room Number : " << roomNumber << endl;
    cout << "Capacity    : " << capacity << endl;
    cout << "Occupancy   : " << students.size() << "/" << capacity << endl;

    if (students.empty())
    {
        cout << "Status      : Available" << endl;
    }
    else
    {
        cout << "Occupants   :" << endl;
        for (Student *s : students)
        {
            cout << "  - [" << s->getId() << "] " << s->getFullName()
            << " (Year " << s->getAcademicYear() << ")" << endl;
        }
    }
    cout << "=============================" << endl;
}