#ifndef REFRISYSTEM_INTERFACES_H
#define REFRISYSTEM_INTERFACES_H

#include <vector>
#include <string>

using namespace std;

// ------------------------------------------------------------------------------------------------------------
// This file is for interfaces like enums and structs.
// ------------------------------------------------------------------------------------------------------------

// StorageType
enum StorageType {
    FRIDGE,
    FREEZER,
    PANTRY
};

// IngredientItem
struct IngredientItem {
    int id;
    string name;
    double quantity;
    int freshness;
    StorageType storageType;
};

// RecipeItem
struct IngredientDetail;
struct RecipeItem {
    int id;
    string name;
    vector<string> tags;
    vector<string> steps;
    vector<IngredientDetail> ingredients;
};
struct IngredientDetail {
    string name;
    double amount;
};

#endif //REFRISYSTEM_INTERFACES_H
