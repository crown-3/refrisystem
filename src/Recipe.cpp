#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/Recipe.h"
#include "../include/IngredientInfo.h"
#include "../include/Table.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <tuple>

using namespace std;

struct Table {
    string title;
    vector<string> tag;
    vector<string> steps;
    vector<IngredientInfo> ingredientInfo;
};

vector<tuple<string, string, string>> Recipe(const string& json_string) {
    // Parse JSON string into json object
    auto recipes = nlohmann::json::parse(json_string);

    // Prepare an empty result vector
    vector<tuple<string, string, string>> result;

    // Iterate through each recipe
    for (const auto& recipe : recipes) {
        // Get recipe name
        string name = recipe["NAME"];

        // Convert tags to a comma-separated string
        string tags = nlohmann::json(recipe["TAG"]).dump();
        tags = tags.substr(1, tags.size() - 2);  // Remove brackets

        // Prepare ingredients string
        string ingredients;
        for (const auto& ingredient_info : recipe["INGREDIENT_INFO"]) {
            // Append ingredient info
            ingredients += to_string(ingredient_info["amount"].get<double>()) + " " + ingredient_info["name"].get<string>() + ", ";
        }
        ingredients = ingredients.substr(0, ingredients.size() - 2);  // Remove last comma

        // Append recipe info to result
        result.push_back(make_tuple(name, tags, ingredients));
    }

    return result;
}

void Recipe::showRecipe(){

}