#include "university.h"
#include <iostream>
using namespace std;

// Default constructor
University::University() : name("ENSIA") {}

// Parameterized constructor
University::University(const string& name) : name(name) {}

// Destructor — university owns dormitories
University::~University() {
    for (Dormitory* d : dormitories) delete d;
    dormitories.clear();
}

// Getters
string University::getName() const { return name; }

int University::getTotalStudents() const {
    int total = 0;
    for (Dormitory* d : dormitories)
        total += d->getTotalOccupancy();
    return total;
}

int University::getTotalRooms() const {
    // We count via dormitory display; kept simple here
    return (int)dormitories.size();
}

const vector<Dormitory*>& University::getDormitories() const {
    return dormitories;
}

// ── Dormitory management ─────────────────────────────────

void University::addDormitory(const string& dormName, int capacity) {
    if (findDormitory(dormName)) {
        cout << "Dormitory '" << dormName << "' already exists." << endl;
        return;
    }
    dormitories.push_back(new Dormitory(dormName, capacity));
    cout << "Dormitory '" << dormName << "' added to " << name << endl;
}

Dormitory* University::findDormitory(const string& dormName) const {
    for (Dormitory* d : dormitories)
        if (d->getName() == dormName) return d;
    return nullptr;
}

void University::displayDormitories() const {
    cout << "\n=== Dormitories of " << name << " ===" << endl;
    if (dormitories.empty()) {
        cout << "No dormitories added yet." << endl;
        return;
    }
    for (Dormitory* d : dormitories) d->display();
}

// ── Student management ───────────────────────────────────

bool University::enrollStudent(const string& id,
                               const string& fullName,
                               int academicYear,
                               const string& dormName,
                               int roomNumber) {
    Dormitory* dorm = findDormitory(dormName);
    if (!dorm) {
        cout << "Dormitory '" << dormName << "' not found." << endl;
        return false;
    }
    // Create the student on the heap — dormitory will own and delete it
    Student* s = new Student(id, fullName, academicYear);
    if (!dorm->addStudent(s, roomNumber)) {
        delete s;   // failed to assign — clean up
        return false;
    }
    return true;
}

bool University::removeStudent(const string& studentId,
                               const string& dormName) {
    Dormitory* dorm = findDormitory(dormName);
    if (!dorm) {
        cout << "Dormitory '" << dormName << "' not found." << endl;
        return false;
    }
    return dorm->removeStudent(studentId);
}

Student* University::findStudent(const string& studentId) const {
    for (Dormitory* d : dormitories) {
        Student* s = d->findStudent(studentId);
        if (s) return s;
    }
    return nullptr;
}

// ── Display ──────────────────────────────────────────────

void University::displaySummary() const {
    cout << "\n************************************" << endl;
    cout << "  " << name << " — System Summary" << endl;
    cout << "************************************" << endl;
    cout << "  Dormitories    : " << dormitories.size() << endl;
    cout << "  Total students : " << getTotalStudents() << endl;
    cout << "************************************" << endl;
}

void University::displayAll() const {
    displaySummary();
    for (Dormitory* d : dormitories) {
        d->display();
        d->displayRooms();
        d->displayStudents();
    }
}