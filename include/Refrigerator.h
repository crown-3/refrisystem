//
// Created by strik on 2023-05-21.
//

#ifndef REFRISYSTEM_REFRIGERATOR_H
#define REFRISYSTEM_REFRIGERATOR_H

#include <string>
#include <vector>
#include "Food.h"
#include "StorageType.h"

using namespace std;

class Refrigerator {
    bool isOn;
public:
    void turnOn();
    void turnOff();
    void addFood(Food food, double amount, StorageType storageType);
    // 없앨 food의 이름과 amount와 없ㅇ
    void removeFood(string foodName, double amount ,StorageType storageType);

    vector<Food> getSpoiledFoods();
};

#endif //REFRISYSTEM_REFRIGERATOR_H
