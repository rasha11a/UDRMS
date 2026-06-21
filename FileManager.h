#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "university.h"
using namespace std;

class FileManager {
public:
    static bool saveData(const University& uni, const string& filename);
    static bool loadData(University& uni, const string& filename);
};

#endif // FILEMANAGER_H