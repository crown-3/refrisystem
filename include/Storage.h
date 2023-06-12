#ifndef REFRISYSTEM_STORAGE_H
#define REFRISYSTEM_STORAGE_H

#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "DataManagement.h"
#include "Interfaces.h"
#include "StorageDataManagement.h"

using namespace nlohmann;
using namespace std;

enum SpoiledIngredientsBehavior {
    DELETE, CLEAR, NONE
};

class Storage {
private:
    string RawJSON_path;
    StorageDataManagement *dataManager;

    enum FreshnessCriteria {
        ROTTEN = 0, DANGER = 10, PERISHABLE = 20, FRESH = 100
    };

    // the original helper functions
    bool isSpoiledEmpty(int criteria);
    string spoiledIngredientsBehavior(int criteria, SpoiledIngredientsBehavior behavior);

public:
    Storage(string data_path);

    ~Storage();

    void printStorage();

    bool isInStorage(string ingredientName);
    bool isDangerEmpty();
    bool isRottenEmpty();

    string dangerIngredientsBehavior(SpoiledIngredientsBehavior behavior);
    string rottenIngredientsBehavior(SpoiledIngredientsBehavior behavior);

    double checkUsableIngredientAmount(string ingredientName);
    bool checkIngredientPerishable(string ingredientName);

    void addIngredientSequence();
    void removeIngredientSequence();

    vector<IngredientItem> getIngredientsFreshnessLowerThan(int criteria);
    double checkIngredientAmount(string ingredientName, int freshThreshold);
    double checkIngredientLowestFreshness(string ingredientName);
};

#endif //REFRISYSTEM_STORAGE_H
