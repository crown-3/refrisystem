#ifndef REFRISYSTEM_STORAGE_H
#define REFRISYSTEM_STORAGE_H

#include <vector>
#include <string>
#include "../hooks/Table.h"
using namespace std;

class Storage{
public:
    vector<Row> loadStorage();
    void showStorage();
    void removeItem();
    void depositAllRotten();
    void sortItems();
};

#endif //REFRISYSTEM_STORAGE_H
