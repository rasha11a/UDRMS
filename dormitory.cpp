#include "dormitory.h"
#include <iostream>
using namespace std;

// Default constructor
Dormitory::Dormitory()
    : name("Unknown Dormitory"), capacity(0),
    restaurant("University Restaurant") {}

// Parameterized constructor
Dormitory::Dormitory(const string& name, int capacity)
    : name(name), capacity(capacity),
    restaurant(name + " Restaurant") {}

// Destructor — dormitory owns rooms and students, so we delete them
Dormitory::~Dormitory() {
    for (Room* r : rooms)       delete r;
    for (Student* s : students) delete s;
    rooms.clear();
    students.clear();
}

// Getters
string Dormitory::getName() const { return name; }
int Dormitory::getCapacity() const { return capacity; }
Restaurant& Dormitory::getRestaurant() { return restaurant; }

int Dormitory::getTotalOccupancy() const {
    int count = 0;
    for (Room* r : rooms) count += r->getOccupancy();
    return count;
}

const vector<Room*>& Dormitory::getRooms() const { return rooms; }
const vector<Student*>& Dormitory::getStudents() const { return students; }

// ── Room management

void Dormitory::addRoom(int roomNumber, int roomCapacity) {
    // Prevent duplicate room numbers
    if (findRoom(roomNumber)) {
        cout << "Room " << roomNumber << " already exists in " << name << endl;
        return;
    }
    rooms.push_back(new Room(roomNumber, roomCapacity));
    cout << "Room " << roomNumber << " added to " << name << endl;
}

Room* Dormitory::findRoom(int roomNumber) const {
    for (Room* r : rooms)
        if (r->getRoomNumber() == roomNumber) return r;
    return nullptr;
}

void Dormitory::displayRooms() const {
    cout << "\n=== Rooms in " << name << " ===" << endl;
    if (rooms.empty()) { cout << "No rooms added yet." << endl; return; }
    for (Room* r : rooms) r->display();
}

void Dormitory::displayAvailableRooms() const {
    cout << "\n=== Available Rooms in " << name << " ===" << endl;
    bool found = false;
    for (Room* r : rooms) {
        if (!r->isFull()) {
            cout << "  Room " << r->getRoomNumber()
            << "  (" << r->getOccupancy() << "/"
            << r->getCapacity() << " occupied)" << endl;
            found = true;
        }
    }
    if (!found) cout << "  No available rooms." << endl;
}

//Student management

bool Dormitory::addStudent(Student* s, int roomNumber) {
    // Check dormitory capacity
    if (getTotalOccupancy() >= capacity) {
        cout << "Dormitory " << name << " is at full capacity!" << endl;
        return false;
    }
    // Find the target room
    Room* room = findRoom(roomNumber);
    if (!room) {
        cout << "Room " << roomNumber << " does not exist in " << name << endl;
        return false;
    }
    // Try to add to room
    if (!room->addStudent(s)) return false;

    s->setAccommodation(name, roomNumber);  // keep student in sync
    students.push_back(s);
    cout << s->getFullName() << " assigned to room "
         << roomNumber << " in " << name << endl;
    return true;
}

bool Dormitory::removeStudent(const string& studentId) {
    // Remove from whichever room holds this student
    for (Room* r : rooms) {
        if (r->findStudent(studentId)) {
            r->removeStudent(studentId);
            break;
        }
    }
    // Remove from dormitory student list
    for (auto it = students.begin(); it != students.end(); ++it) {
        if ((*it)->getId() == studentId) {
            cout << (*it)->getFullName() << " removed from " << name << endl;
            (*it)->clearAccommodation();  // keep student in sync
            students.erase(it);
            return true;
        }
    }
    cout << "Student " << studentId << " not found in " << name << endl;
    return false;
}

Student* Dormitory::findStudent(const string& studentId) const {
    for (Student* s : students)
        if (s->getId() == studentId) return s;
    return nullptr;
}

void Dormitory::displayStudents() const {
    cout << "\n=== Students in " << name << " ===" << endl;
    if (students.empty()) { cout << "No students yet." << endl; return; }
    for (Student* s : students) s->display();
}

//display

void Dormitory::display() const {
    cout << "\n##############################" << endl;
    cout << "  Dormitory : " << name << endl;
    cout << "  Capacity  : " << capacity << endl;
    cout << "  Occupancy : " << getTotalOccupancy()
         << "/" << capacity << endl;
    cout << "  Rooms     : " << rooms.size() << endl;
    cout << "  Students  : " << students.size() << endl;
    cout << "##############################" << endl;
    restaurant.displayMenu();
}
