#include "FileManager.h"
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

static vector<string> split(const string& line, char delim) {
    vector<string> tokens;
    size_t start = 0, pos;
    while ((pos = line.find(delim, start)) != string::npos) {
        tokens.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    tokens.push_back(line.substr(start));
    return tokens;
}

bool FileManager::saveData(const University& uni, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open file for writing." << endl;
        return false;
    }

    for (Dormitory* dorm : uni.getDormitories()) {
        // Save dormitory
        file << "DORM|" << dorm->getName()
             << "|" << dorm->getCapacity() << "\n";

        // Save each room
        for (Room* room : dorm->getRooms()) {
            file << "ROOM|" << dorm->getName()
            << "|" << room->getRoomNumber()
            << "|" << room->getCapacity() << "\n";
        }

        // Save restaurant menu
        Meal b = dorm->getRestaurant().getBreakfast();
        file << "MENU|" << dorm->getName() << "|breakfast|"
             << b.mainDish << "|" << b.salad << "|"
             << b.fruit   << "|" << b.juice << "|"
             << b.soup    << "\n";

        Meal l = dorm->getRestaurant().getLunch();
        file << "MENU|" << dorm->getName() << "|lunch|"
             << l.mainDish << "|" << l.salad << "|"
             << l.fruit    << "|" << l.juice << "|"
             << l.soup     << "\n";

        Meal d = dorm->getRestaurant().getDinner();
        file << "MENU|" << dorm->getName() << "|dinner|"
             << d.mainDish << "|" << d.salad << "|"
             << d.fruit    << "|" << d.juice << "|"
             << d.soup     << "\n";

        // Save each student
        for (Student* s : dorm->getStudents()) {
            file << "STUDENT|" << s->getId()
            << "|" << s->getFullName()
            << "|" << s->getAcademicYear()
            << "|" << s->getDormitoryName()
            << "|" << s->getRoomNumber() << "\n";
        }
    }

    file.close();
    cout << "Data saved." << endl;
    return true;
}

bool FileManager::loadData(University& uni, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "No save file found. Starting fresh." << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> t = split(line, '|');
        if (t.empty()) continue;

        if (t[0] == "DORM" && t.size() == 3) {
            uni.addDormitory(t[1], stoi(t[2]));
        }
        else if (t[0] == "ROOM" && t.size() == 4) {
            Dormitory* d = uni.findDormitory(t[1]);
            if (d) d->addRoom(stoi(t[2]), stoi(t[3]));
        }
        else if (t[0] == "MENU" && t.size() == 8) {
            Dormitory* d = uni.findDormitory(t[1]);
            if (!d) continue;
            if      (t[2] == "breakfast")
                d->getRestaurant().setBreakfast(t[3],t[4],t[5],t[6],t[7]);
            else if (t[2] == "lunch")
                d->getRestaurant().setLunch(t[3],t[4],t[5],t[6],t[7]);
            else if (t[2] == "dinner")
                d->getRestaurant().setDinner(t[3],t[4],t[5],t[6],t[7]);
        }
        else if (t[0] == "STUDENT" && t.size() == 6) {
            uni.enrollStudent(t[1], t[2], stoi(t[3]), t[4], stoi(t[5]));
        }
    }

    file.close();
    cout << "Data loaded." << endl;
    return true;
}