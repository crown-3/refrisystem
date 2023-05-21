//
// Created by User on 2023-05-21.
//

#include <fstream>
#include <nlohmann/json.hpp>
#include "../include/RefriRecipe.h"
#include "../include/Table.h"

RefriRecipe::RefriRecipe(){
    // recipe dataset load , assign to : vector<Recipe> recipeList
//    string data_path = "../source/recipeData.json";
//    vector<Row> data = RefriRecipe::loadRecipeList(data_path);


}

void RefriRecipe::MAIN_RefriStorage(){
    // show recipelist

}

vector<Row> RefriRecipe::loadRecipeList(string data_path){

    // load json file with data_path
    ifstream jsonFile(data_path);
    nlohmann::json j;
    jsonFile >> j;


    vector<Row> result;

    for (const auto& recipe : j) {
        Row row;

        // Recipe name
        row.values.push_back(recipe["NAME"].get<string>());

        // Tags
        string tags;
        for (const auto& tag : recipe["TAG"]) {
            tags += tag.get<string>() + ", ";
        }
        tags = tags.substr(0, tags.size()-2); // remove the trailing comma
        row.values.push_back(tags);

        // Ingredients
        string ingredients;
        for (const auto& ingredient : recipe["INGREDIENT_INFO"]) {
            ingredients += to_string(ingredient["amount"].get<double>()) + " " + ingredient["name"].get<string>() + ", ";
        }
        ingredients = ingredients.substr(0, ingredients.size()-2); // remove the trailing comma
        row.values.push_back(ingredients);
        result.push_back(row);
    }

    return result;
}

void RefriRecipe::showRecipeList(){
    vector<string> title = {"NAME","TAGS","INGREDIENTS"};
    vector<Row> data = RefriRecipe::loadRecipeList("../source/recipeData.json");

    table(title, data);
}

void RefriRecipe::removeRecipe(string targetName){
    vector<string> title = {"NAME","TAGS","INGREDIENTS"};
    vector<Row> data = RefriRecipe::loadRecipeList("../source/recipeData.json");

}

