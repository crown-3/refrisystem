#ifndef REFRISYSTEM_TABLE_H
#define REFRISYSTEM_TABLE_H

#include <vector>
#include <string>
using namespace std;

// Struct that represents the row
struct Row {
    vector<string> values;
};

// Function that prints the table
void table(const vector<string>& titles, const vector<Row>& rows);

#endif //REFRISYSTEM_TABLE_H
