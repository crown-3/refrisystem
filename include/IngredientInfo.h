#ifndef REFRISYSTEM_INGREDIENTINFO_H
#define REFRISYSTEM_INGREDIENTINFO_H

#include <string>
#include <vector>
#include <map>

using namespace std; // ?????

class IngredientInfo {
    string name;
    float amount;
    string storage;
public:
    vector<map<string, string>> parseData(string ingredientInfo);
};

#endif //REFRISYSTEM_INGREDIENTINFO_H
