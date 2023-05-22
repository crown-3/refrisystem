//
// Created by User on 2023-05-21.
//

#include <fstream>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>

#include "../include/RefriRecipe.h"
#include "../hooks/CutDecimal.h"
#include "../hooks/Title.h"
#include "../hooks/Input.h"
#include "../hooks/TagLists.h"
#include "../hooks/ColorfulCli.h"

using namespace std;
using namespace nlohmann;

RefriRecipe::RefriRecipe() {
    // recipe dataset load , assign to : vector<Recipe> recipeList
//    string data_path = "../source/recipeData.json";
//    vector<Row> data = RefriRecipe::loadRecipeList(data_path);


}

void RefriRecipe::MAIN_RefriStorage() {
    // show recipelist

}

vector<Row> RefriRecipe::loadRecipeList(string data_path) {

    // load json file with data_path
    ifstream jsonFile(data_path);
    nlohmann::json j;
    jsonFile >> j;


    vector<Row> result;

    for (const auto &recipe: j) {
        Row row;

        // Recipe name
        row.values.push_back(recipe["NAME"].get<string>());

        // Tags
        string tags;
        for (const auto &tag: recipe["TAG"]) {
            tags += tag.get<string>() + ", ";
        }
        tags = tags.substr(0, tags.size() - 2); // remove the trailing comma
        row.values.push_back(tags);

        // Ingredients
        string ingredients;
        for (const auto &ingredient: recipe["INGREDIENT_INFO"]) {
            ingredients += CutDecimal(to_string(ingredient["amount"].get<float>()), 1) + " " +
                           ingredient["name"].get<string>() + ", ";
        }
        ingredients = ingredients.substr(0, ingredients.size() - 2); // remove the trailing comma
        row.values.push_back(ingredients);
        result.push_back(row);
    }

    return result;
}

void RefriRecipe::showRecipeList() {
    vector<string> title = {"NAME", "TAGS", "INGREDIENTS"};
    vector<Row> data = RefriRecipe::loadRecipeList("../source/recipeData.json");

    table(title, data);
}

void RefriRecipe::removeRecipe(string targetName) {
    vector<string> title = {"NAME", "TAGS", "INGREDIENTS"};
    vector<Row> data = RefriRecipe::loadRecipeList("../source/recipeData.json");

}

void RefriRecipe::showPossibleTags() {
    cout << "Possible Tags: ";
    for (size_t i = 0; i < RecipeTagList.size(); ++i) {
        TextColor(LIGHTGREEN, BLACK);
        cout << RecipeTagList[i];
        TextColor(WHITE, BLACK);
        if (i != RecipeTagList.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

json RefriRecipe::inputIngredients() {
    fstream dataFile;
    dataFile.open("../source/FoodData.json");

    json j;
    dataFile >> j;

    map<string, string> foodData;

    for (auto &element: j) {
        foodData[element["name"]] = element["storage"];
    }

    json ingredientList = json::array(); // JSON array to hold the ingredient list

    while (true) {
        double amount;
        string ingredient;
        cout << "Input an ingredient name and amount (-1 to finish) : ";
        cin >> amount;
        if (amount == -1) {
            break;
        }
        cin.ignore();
        getline(cin, ingredient);

        if (foodData.find(ingredient) == foodData.end()) {
            char response;
            cout << "----| Add new ingredient '" << ingredient << "' to food data? (y/n) : ";
            cin >> response;
            if (response == 'y' || response == 'Y') {
                string storage;
                storage = MultipleChoice({"fridge", "freezer", "pantry"},
                                         "----| What is the suit storage for '" + ingredient + "'?",
                                         "(fridge / freezer / pantry)")[0];
                foodData[ingredient] = storage;
                json newElement = {
                        {"name",    ingredient},
                        {"storage", storage}
                };
                j.push_back(newElement);
                // Write the updated data to the file immediately
                ofstream output;
                output.open("../source/FoodData.json");
                output << j.dump(4); // pretty-printing with 4 spaces indent
                output.close();
            } else {
                continue;
            }
        }

        // Add the ingredient and amount to the ingredientList
        ingredientList.push_back({{"name",   ingredient},
                                  {"amount", amount}});
    }

    return ingredientList;
}

void RefriRecipe::addRecipe() {
    Title("Add Recipe");

    Subtitle("Recipe Name");
    string recipeName = Input("Enter your recipe name");

    Subtitle("Recipe Tags");
    showPossibleTags();
    vector<string> recipeTagSelect = MultipleChoice(RecipeTagList, "Enter recipe's tags", "ex) Sweet, Fat");

    Subtitle("Recipe Ingredients");
    json recipeIngredients = inputIngredients();

    Subtitle("Recipe Steps");


    cout << recipeName << endl;
    for (const auto &tag: recipeTagSelect) {
        cout << tag << endl;
    }
}

