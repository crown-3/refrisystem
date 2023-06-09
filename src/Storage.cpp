#include "../include/Storage.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include "../utils/Table.h"
#include "../utils/ColorfulCli.h"

using namespace std;
using namespace nlohmann;

vector<IngredientItem> Storage::ingredientData;

// Load from JSON file

Storage::Storage(string data_path)
: RawJSON_path(data_path)
{
    loadData();
}

void Storage::loadData() {
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

Storage::~Storage() {
    saveData();
}

void Storage::saveData() {
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

// ------------------------------------------------------------------------------------------------------------
// Add Ingredient
void Storage::addIngredient(IngredientItem ingredient) {
    Storage::ingredientData.push_back(ingredient);
}

// Remove Ingredient
void Storage::removeIngredient(string ingredientName, double amount) {
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

// Print Current Storage Storage (Using Table.h)

void Storage::printStorage() {

    vector<Row> spoiledRow;
    vector<IngredientItem> spoiledIngredients = getIngredientsFreshnessLowerThan(10);
    for (const auto& ing : spoiledIngredients) {
        Row newRow;
        newRow.values.push_back(to_string(ing.id));
        newRow.values.push_back(ing.storageType == StorageType::FRIDGE ? "fridge" :
                                ing.storageType == StorageType::FREEZER ? "freezer" : "pantry");
        newRow.values.push_back(ing.name);
        newRow.values.push_back(to_string(ing.freshness));
        newRow.values.push_back(to_string(ing.quantity));
        spoiledRow.push_back(newRow);
    }

    vector<Row> fridgeRow;
    vector<Row> freezerRow;
    vector<Row> pantryRow;
    for (const auto& ing : ingredientData) {
        if (ing.storageType == StorageType::FRIDGE) {
            Row newRow;
            newRow.values.push_back(to_string(ing.id));
            newRow.values.push_back(ing.name);
            newRow.values.push_back(to_string(ing.freshness));
            newRow.values.push_back(to_string(ing.quantity));
            fridgeRow.push_back(newRow);
        } else if (ing.storageType == StorageType::FREEZER) {
            Row newRow;
            newRow.values.push_back(to_string(ing.id));
            newRow.values.push_back(ing.name);
            newRow.values.push_back(to_string(ing.freshness));
            newRow.values.push_back(to_string(ing.quantity));
            freezerRow.push_back(newRow);
        } else if (ing.storageType == StorageType::PANTRY) {
            Row newRow;
            newRow.values.push_back(to_string(ing.id));
            newRow.values.push_back(ing.name);
            newRow.values.push_back(to_string(ing.freshness));
            newRow.values.push_back(to_string(ing.quantity));
            pantryRow.push_back(newRow);
        }
    }

    vector<string> alertTitle = {"ID", "STORAGE", "NAME", "FRESHNESS", "QUANTITY"};

    TextColor(BLACK, RED);
    cout << "!ALERT! These ingredients are about to expire!";
    TextColor(WHITE, BLACK);
    cout << endl;
    table(alertTitle, spoiledRow);

    vector<string> storageTitle = {"ID", "NAME", "FRESHNESS", "QUANTITY"};

    TextColor(BLACK, CYAN);
    cout << "Fridge";
    TextColor(WHITE, BLACK);
    cout << endl;
    table(storageTitle, fridgeRow);

    TextColor(BLACK, CYAN);
    cout << "Freezer";
    TextColor(WHITE, BLACK);
    cout << endl;
    table(storageTitle, freezerRow);

    TextColor(BLACK, CYAN);
    cout << "Pantry";
    TextColor(WHITE, BLACK);
    cout << endl;
    table(storageTitle, pantryRow);
}



// Check Ingredient amount
double Storage::checkAmount(string ingredientName, int freshThreshold) {
    double amount = 0.0;
    for (const auto& ing : ingredientData) {
        if (ing.name == ingredientName && ing.freshness >= freshThreshold) {
            amount += ing.quantity;
        }
    }
    // cout << ingredientName << "   " << amount << endl;
    return amount;
}

// ------------------------------------------------------------------------------------------------------------

void Storage::clearFreshness(string ingredientName, int init) {
    for (auto& ing : ingredientData) {
        if (ing.name == ingredientName && ing.freshness <= 10) {
            ing.freshness = init;
        }
    }
    saveData();
}

// ------------------------------------------------------------------------------------------------------------

vector<IngredientItem> Storage::getIngredientsFreshnessLowerThan(int criteria) {
    vector<IngredientItem> spoiledFoods;
    for (const auto& ing : ingredientData) {
        if (ing.freshness <= criteria) {
            spoiledFoods.push_back(ing);
        }
    }
    return spoiledFoods;
}
