#include "../include/RecipeDataManagement.h"
#include "../utils/Table.h"
#include "../utils/Title.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using namespace nlohmann;

RecipeDataManagement::RecipeDataManagement(string data_path) {
    RawJSON_path = data_path;
    loadData();
}

RecipeDataManagement::~RecipeDataManagement() {
    saveData();
}

void RecipeDataManagement::loadData() {
    // load json file with data_path
    ifstream jsonFile(RawJSON_path);
    if (!jsonFile.is_open()) {
        throw runtime_error("RecipeData.json file could not be opened. Check if the file exists and its permissions.");
    }
    json j;
    jsonFile >> j;
    jsonFile.close();

    int idCounter = 1; // Counter for assigning row.id

    // assign json items to each RecipeRow row
    for (const auto &recipe: j) {
        RecipeItem row;

        // Assign values to RecipeRow members
        row.id = idCounter++;
        row.name = recipe["NAME"];
        row.tags = recipe["TAG"];
        row.steps = recipe["STEPS"];

        // Assign ingredient details
        const auto &ingredientInfo = recipe["INGREDIENT_INFO"];
        for (const auto &ingredient: ingredientInfo) {
            IngredientDetail detail;
            detail.name = ingredient["name"];
            detail.amount = ingredient["amount"];
            row.ingredients.push_back(detail);
        }

        // Add the RecipeRow to recipeData vector
        data.push_back(row);
    }
}

void RecipeDataManagement::saveData() {
    // load json file with data_path
    ofstream jsonFile(RawJSON_path);
    if (!jsonFile.is_open()) {
        throw runtime_error(
                "Storage.json file could not be opened for writing. Check if the file exists and its permissions.");
    }

    // assign RecipeRow rows to each json item
    json j;
    for (auto &element: data) {
        json sub;
        sub["NAME"] = element.name;
        sub["TAG"] = element.tags;
        sub["STEPS"] = element.steps;

        vector<json> ingredientInfo;
        for (const auto &ingredient: element.ingredients) {
            json ingredientItem;
            ingredientItem["name"] = ingredient.name;
            ingredientItem["amount"] = ingredient.amount;
            ingredientInfo.push_back(ingredientItem);
        }
        sub["INGREDIENT_INFO"] = ingredientInfo;

        j.push_back(sub);
    }

    // overwrite(save) jsonFile
    jsonFile << j.dump(4) << endl;
    jsonFile.close();
}

vector<RecipeItem> RecipeDataManagement::getData() {
    return data;
}

void RecipeDataManagement::addData(RecipeItem recipe) {
    data.push_back(recipe);
}

void RecipeDataManagement::removeData(string recipeName) {
    bool isFound = false;

    for (auto it = data.begin(); it != data.end(); it++) {
        if (it->name == recipeName) {
            isFound = true;
            data.erase(it);
            break;
        }
    }

    if (!isFound) {
        Error("Recipe name not found.");
    }
}
