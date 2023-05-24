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
    //    string data_path = "../source/RecipeData.json";
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
    vector<Row> data = RefriRecipe::loadRecipeList("../source/RecipeData.json");

    table(title, data); // print all rows in table format
}

void RefriRecipe::removeRecipe(string targetName) {
    vector<string> title = {"NAME", "TAGS", "INGREDIENTS"};
    vector<Row> data = RefriRecipe::loadRecipeList("../source/RecipeData.json");


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
        cout << "Input an ingredient name and amount (-1 to finish): ";
        cin >> amount;
        if (amount == -1) {
            break;
        }
        cin.ignore();
        getline(cin, ingredient);

        if (foodData.find(ingredient) == foodData.end()) {
            char response;
            cout << "----| Add new ingredient '" << ingredient << "' to food data? (y/n): ";
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
                TextColor(DARKGRAY, BLACK);
                cout << "----| Adding ingredient '" << ingredient << "' canceled." << endl;
                TextColor(WHITE, BLACK);
                continue;
            }
        }

        // Add the ingredient and amount to the ingredientList
        ingredientList.push_back({{"name",   ingredient},
                                  {"amount", amount}});
    }

    return ingredientList;
}

json RefriRecipe::inputSteps() {
    json steps = json::array();
    string step;
    int step_num = 1;
    while (true) {
        step = Input("Step " + to_string(step_num), "(-1 to finish)");
        if (step == "-1")
            break;
        steps.push_back(step);
        ++step_num;
    }
    return steps;
}

void RefriRecipe::addRecipe() {
    try {
        Title("Add Recipe");

        Subtitle("Recipe Name");
        string recipeName = Input("Enter your recipe name");

        Subtitle("Recipe Tags");
        showPossibleTags();
        vector<string> recipeTagSelect = MultipleChoice(RecipeTagList, "Enter recipe's tags", "ex) Sweet, Fat");

        Subtitle("Recipe Ingredients");
        json recipeIngredients = inputIngredients();

        Subtitle("Recipe Steps");
        json recipeSteps = inputSteps();

        // Save Recipe to the RecipeData.json
        json newRecipe;
        newRecipe["NAME"] = recipeName;
        newRecipe["TAG"] = recipeTagSelect;
        newRecipe["STEPS"] = recipeSteps;
        newRecipe["INGREDIENT_INFO"] = recipeIngredients;

        // Load existing recipes
        ifstream i("../source/RecipeData.json");
        if (!i) {
            throw runtime_error("Unable to open file: RecipeData.json");
        }

        json existingRecipes;
        if (i.peek() != ifstream::traits_type::eof()) // Check if file is empty
            i >> existingRecipes;
        i.close();

        // Append the new recipe to the existing recipes
        existingRecipes.push_back(newRecipe);

        // Save the updated recipes back to the file
        ofstream o("../source/RecipeData.json");
        if (!o) {
            throw runtime_error("Unable to open file: RecipeData.json for writing");
        }

        o << existingRecipes.dump(4) << endl; // write with indentation of 4
        o.close();
        Subtitle("Successfully added new recipe '" + recipeName + "'!");
    }
    catch (const std::exception &e) {
        Subtitle(e.what());  // Display the error message
    }
}

