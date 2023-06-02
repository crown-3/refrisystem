#include "../include/Refrigerator.h"
#include <fstream>
#include <algorithm>

void Refrigerator::loadIngredient() {
    ifstream i("../source/Storage.json");
    if (!i.is_open()) {
        throw runtime_error("Storage.json file could not be opened. Check if the file exists and its permissions.");
    }

    json j;
    i >> j;
    i.close();

    for (auto& element : j) {
        Ingredient newIngredient;
        newIngredient.name = element["name"];
        newIngredient.quantity = element["amount"];
        newIngredient.freshness = element["freshness"];
        newIngredient.storageType = element["storage"] == "fridge" ? StorageType::FRIDGE :
                                    element["storage"] == "freezer" ? StorageType::FREEZER :
                                    StorageType::PANTRY;
        this->ingredient.push_back(newIngredient);
    }
}

void Refrigerator::saveIngredient() {
    ofstream o("../source/Storage.json");
    if (!o.is_open()) {
        throw runtime_error("Storage.json file could not be opened for writing. Check if the file exists and its permissions.");
    }

    json j;
    for (auto& element : this->ingredient) {
        json sub;
        sub["name"] = element.name;
        sub["amount"] = element.quantity;
        sub["freshness"] = element.freshness;
        sub["storage"] = element.storageType == StorageType::FRIDGE ? "fridge" :
                         element.storageType == StorageType::FREEZER ? "freezer" : "pantry";
        j.push_back(sub);
    }

    o << j.dump(4) << endl;
    o.close();
}

void Refrigerator::addIngredient(Ingredient ingredient) {
    this->ingredient.push_back(ingredient);
}

void Refrigerator::removeIngredient(string ingredientName, double amount) {
    auto it = find_if(ingredient.begin(), ingredient.end(), [&ingredientName](const Ingredient& ing) { return ing.name == ingredientName; });

    if (it == ingredient.end()) {
        throw runtime_error("Ingredient not found in the refrigerator.");
    }

    if (it->quantity <= amount) {
        ingredient.erase(it);
    } else {
        it->quantity -= amount;
    }
}

vector<Ingredient> Refrigerator::getIngredientsFreshnessLowerThan(int criteria) {
    vector<Ingredient> spoiledFoods;
    for (const auto& ing : ingredient) {
        if (ing.freshness <= criteria) {
            spoiledFoods.push_back(ing);
        }
    }
    return spoiledFoods;
}