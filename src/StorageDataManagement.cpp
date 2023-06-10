#include "../include/StorageDataManagement.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>

using namespace std;
using namespace nlohmann;

StorageDataManagement::StorageDataManagement(string data_path) {
    RawJSON_path = data_path;
    loadData();
}

StorageDataManagement::~StorageDataManagement() {
    saveData();
}

void StorageDataManagement::loadData() {
    // load json file with data_path
    ifstream jsonFile(RawJSON_path);
    if (!jsonFile.is_open()) {
        throw runtime_error("Storage.json file could not be opened. Check if the file exists and its permissions.");
    }
    json j;
    jsonFile >> j;
    jsonFile.close();

    int idCounter = 1; // Counter for assigning newIngredient.id

    // assign json items to each RecipeRow row
    for (const auto& element : j) {
        IngredientItem newIngredient;

        // Assign values to RecipeRow members
        newIngredient.id = idCounter++;
        newIngredient.name = element["name"];
        newIngredient.quantity = element["amount"];
        newIngredient.freshness = element["freshness"];
        newIngredient.storageType = element["storage"] == "fridge" ? StorageType::FRIDGE :
                                    element["storage"] == "freezer" ? StorageType::FREEZER :
                                    StorageType::PANTRY;

        // Add the newIngredient to data vector
        data.push_back(newIngredient);
    }
}

void StorageDataManagement::saveData() {
    ofstream jsonFile(RawJSON_path);
    if (!jsonFile.is_open()) {
        throw runtime_error("Storage.json file could not be opened for writing. Check if the file exists and its permissions.");
    }

    json j;
    for (auto& element : data) {
        json sub;
        sub["name"] = element.name;
        sub["amount"] = element.quantity;
        sub["freshness"] = element.freshness;
        sub["storage"] = element.storageType == StorageType::FRIDGE ? "fridge" :
                         element.storageType == StorageType::FREEZER ? "freezer" : "pantry";
        j.push_back(sub);
    }

    jsonFile << j.dump(4) << endl;
    jsonFile.close();
}

// Getter
vector<IngredientItem> StorageDataManagement::getData() {
    return data;
}

// Add Ingredient
void StorageDataManagement::addData(IngredientItem ingredient) {
    StorageDataManagement::data.push_back(ingredient);
}

// Remove Ingredient
void StorageDataManagement::removeData(std::string ingredientName, double amount) {
    // Collect all ingredients with the given name
    vector<int> ingredientIndices;
    for (int i = 0; i < data.size(); i++) {
        if (data[i].name == ingredientName) {
            ingredientIndices.push_back(i);
        }
    }

    if (ingredientIndices.empty()) {
        throw runtime_error("Ingredient not found in the refrigerator.");
    }

    // Sort ingredients by freshness in ascending order
    sort(ingredientIndices.begin(), ingredientIndices.end(), [&](int a, int b) {
        return data[a].freshness < data[b].freshness;
    });

    // Subtract the requested amount from the freshness-sorted ingredients
    for (auto index : ingredientIndices) {
        if (amount <= 0) {
            break;
        }
        if (amount >= data[index].quantity) {
            amount -= data[index].quantity;
            data[index].quantity = 0;
        } else {
            data[index].quantity -= amount;
            amount = 0;
        }
    }

    // Remove ingredients with zero quantity
    data.erase(remove_if(data.begin(), data.end(),
                                   [](const IngredientItem& ing) { return ing.quantity <= 0; }), data.end());
}

// Clear Freshness of an Ingredient
void StorageDataManagement::clearFreshness(string ingredientName, int init, int criteria) {
    for (auto& ing : data) {
        if (ing.name == ingredientName && ing.freshness <= criteria) {
            ing.freshness = init;
        }
    }
}
