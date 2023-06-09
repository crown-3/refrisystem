#ifndef REFRISYSTEM_DATAMANAGEMENT_H
#define REFRISYSTEM_DATAMANAGEMENT_H

#include <string>
using namespace std;

// Using pure virtual function to make this class abstract
class DataManagement {
public:
    // Save to JSON file
    virtual void saveData(string DataPath)=0;

    // Load from JSON file, and save to member variables
    virtual void loadData(string DataPath)=0;

    virtual ~DataManagement()=0;

    virtual void printData()=0;

    virtual void addData()=0;

    virtual void removeData()=0;
};

#endif //REFRISYSTEM_DATAMANAGEMENT_H
