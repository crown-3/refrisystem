#ifndef REFRISYSTEM_INGREDIENT_H
#define REFRISYSTEM_INGREDIENT_H

#include <string>
#include "StorageType.h"
using namespace std;

struct Ingredient {
    string name;
    double quantity;
    int freshness;
    StorageType storageType;
};

#endif //REFRISYSTEM_INGREDIENT_H
