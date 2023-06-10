#ifndef REFRISYSTEM_DATAMANAGEMENT_H
#define REFRISYSTEM_DATAMANAGEMENT_H

#include <string>
using namespace std;

// Using pure virtual function to make this class abstract
class DataManagement {
public:
    virtual ~DataManagement()=0;

    // Save to JSON file
    virtual void saveData()=0;

    // Load from JSON file, and save to member variables
    virtual void loadData()=0;
};

#endif //REFRISYSTEM_DATAMANAGEMENT_H
