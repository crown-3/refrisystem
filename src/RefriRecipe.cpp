//
// Created by User on 2023-05-21.
//

#include <fstream>
#include <nlohmann/json.hpp>

#include <iostream>
#include <iomanip>
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

void RefriRecipe::removeRecipe() {
    try {
        Title("Remove Recipe");

        Subtitle("Recipe Name");
        string recipeName = Input("Enter target recipe name");

        // Load existing recipes
        ifstream i("../source/RecipeData_removeTest.json");
        if (!i) {
            throw runtime_error("Unable to open file: RecipeData_removeTest.json");
        }

        json existingRecipes;
        if (i.peek() != ifstream::traits_type::eof()) // Check if file is empty
            i >> existingRecipes;

        // Find the recipe with the matching target name
        bool found = false;
        json removedRecipes;

        for (const auto& element : existingRecipes.items()) {
            if (element.value().is_object() && element.value().contains("NAME") && element.value()["NAME"] == recipeName) {
                found = true;
            }else{
                removedRecipes.push_back(element);
            }
        }
        if (!found) {
            throw runtime_error("There is no remove target in existing data");
        }

        i.close();

        // Save the updated recipes back to the file
        ofstream o("../source/RecipeData_removeTest.json");
        if (!o) {
            throw runtime_error("Unable to open file: RecipeData_removeTest.json for writing");
        }

        o << removedRecipes.dump(4) << endl; // write with indentation of 4
        o.close();
        Subtitle("Successfully removed recipe '" + recipeName + "'!");
    }
    catch (const std::exception &e) {
        Subtitle(e.what());  // Display the error message
    }
}

void RefriRecipe::makeFood() {
    // recommended test case : Blueberry Yogurt Smoothie
    try {
        Title("Make Food");

        Subtitle("Recipe Name");
        string recipeName = Input("Enter target recipe name");

        // Load existing recipes
        ifstream i("../source/RecipeData.json");
        if (!i) {
            throw runtime_error("Unable to open file: RecipeData.json");
        }

        json existingRecipes;
        if (i.peek() != ifstream::traits_type::eof()) // Check if file is empty
            i >> existingRecipes;

        // Find the recipe with the matching target name
        bool found = false;
        json targetRecipe;

        for (const auto& element : existingRecipes.items()) {
            if (element.value().is_object() && element.value().contains("NAME") && element.value()["NAME"] == recipeName) {
                found = true;
                targetRecipe = element.value();
                break;
            }
        }
        if (!found) {
            throw runtime_error("There is no target in existing data");
        }

        i.close();

        // Load current storage info
        ifstream istorage("../source/Storage_makeFoodTest.json");
        if (!istorage) {
            throw runtime_error("Unable to open file: Storage_makeFoodTest.json");
        }

        json havingFoods;
        if (istorage.peek() != ifstream::traits_type::eof()) // Check if file is empty
            istorage >> havingFoods;

        // check if storage have enough ingredients for target recipe
        json updatedStorage = havingFoods;
        json updateTargets;

        for (const auto& ingredient : targetRecipe["INGREDIENT_INFO"].items()) {
            string foodName = ingredient.value()["name"];
            double foodAmount = ingredient.value()["amount"];
            bool makable = false;
            int index = 0;
            for (const auto& food : havingFoods.items()) {
                if (food.value()["name"] == foodName && food.value()["amount"] >= foodAmount && food.value()["freshness"] > 0) {
                    makable = true;
                    // cout << foodName << endl;

                    json updatedFood;
                    updatedFood = food.value();
                    double updatedAmount = static_cast<double>(food.value()["amount"]) - foodAmount;
                    updatedAmount = round(updatedAmount * 10) / 10;
                    updatedFood["amount"] = updatedAmount;
                    // cout << std::fixed << setprecision(2);
                    // cout << updatedFood << endl;
                    updatedStorage[index] = updatedFood;
                    updatedFood["usedAmount"] = foodAmount;
                    updateTargets.push_back(updatedFood);
                }
                index += 1;
            }
            if (!makable) {
                throw runtime_error("There are not enough ingredients in storage to make food");
            }
        }

        istorage.close();

        // make food : Save the updated storageData back to the file
        ofstream o("../source/Storage_makeFoodTest.json");
        if (!o) {
            throw runtime_error("Unable to open file: Storage_makeFoodTest.json for writing");
        }

        o << updatedStorage.dump(4) << endl; // write with indentation of 4
        o.close();

        Subtitle("Making '" + recipeName + "' ... ... ...");
        // 소모된 food 출력
        for (const auto& food : updateTargets.items()) {
            cout << "\t" << food.value()["name"] << "used " << food.value()["usedAmount"] << ", left amount: "
                 << food.value()["amount"] << endl;
        }
        // steps 출력
        int stepIndex = 1;
        for (const auto& step : targetRecipe["STEPS"].items()) {
            string stepText = step.value();
            stepText.erase(remove(stepText.begin(), stepText.end(), '\"'), stepText.end());
            cout << stepIndex << ") " << stepText << endl;
            stepIndex += 1;
        }
        Subtitle("Successfully made '" + recipeName + "'!");
    }
    catch (const std::exception &e) {
        Subtitle(e.what());  // Display the error message
    }
};