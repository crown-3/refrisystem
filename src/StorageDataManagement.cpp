#include "../include/StorageDataManagement.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>

using namespace std;
using namespace nlohmann;


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

        // Add the newIngredient to ingredientData vector
        ingredientData.push_back(newIngredient);
    }
}

StorageDataManagement::~StorageDataManagement() {
    saveData();
}

void StorageDataManagement::saveData() {
    ofstream jsonFile(RawJSON_path);
    if (!jsonFile.is_open()) {
        throw runtime_error("Storage.json file could not be opened for writing. Check if the file exists and its permissions.");
    }

    json j;
    for (auto& element : ingredientData) {
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

// Add Ingredient
void StorageDataManagement::addData(IngredientItem ingredient) {
    StorageDataManagement::ingredientData.push_back(ingredient);
}

// Remove Ingredient
void StorageDataManagement::removeData(std::string ingredientName, double amount) {
    // Collect all ingredients with the given name
    vector<int> ingredientIndices;
    for (int i = 0; i < ingredientData.size(); i++) {
        if (ingredientData[i].name == ingredientName) {
            ingredientIndices.push_back(i);
        }
    }

    if (ingredientIndices.empty()) {
        throw runtime_error("Ingredient not found in the refrigerator.");
    }

    // Sort ingredients by freshness in ascending order
    sort(ingredientIndices.begin(), ingredientIndices.end(), [&](int a, int b) {
        return ingredientData[a].freshness < ingredientData[b].freshness;
    });

    // Subtract the requested amount from the freshness-sorted ingredients
    for (auto index : ingredientIndices) {
        if (amount <= 0) {
            break;
        }
        if (amount >= ingredientData[index].quantity) {
            amount -= ingredientData[index].quantity;
            ingredientData[index].quantity = 0;
        } else {
            ingredientData[index].quantity -= amount;
            amount = 0;
        }
    }

    // Remove ingredients with zero quantity
    ingredientData.erase(remove_if(ingredientData.begin(), ingredientData.end(),
                                   [](const IngredientItem& ing) { return ing.quantity <= 0; }), ingredientData.end());
}