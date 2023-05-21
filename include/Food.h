//
// Created by strik on 2023-05-21.
//

#ifndef REFRISYSTEM_FOOD_H
#define REFRISYSTEM_FOOD_H

#include <string>
#include "StorageType.h"
using namespace std;

class Food {
    string name;
    double quantity;
    int freshness;
    StorageType storageType;
    int expiryDate;
public:
    void updateFreshness();
    bool isSpoiled();
};

#endif //REFRISYSTEM_FOOD_H
