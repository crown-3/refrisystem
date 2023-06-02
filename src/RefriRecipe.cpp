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

// Load from JSON file
RefriRecipe::RefriRecipe(string data_path)
: RawJSON_path(data_path)
{
    loadRecipeData();
}

void RefriRecipe::loadRecipeData() {
    // load json file with data_path
    ifstream jsonFile(RawJSON_path);
    nlohmann::json j;
    jsonFile >> j;

    int idCounter = 1; // Counter for assigning row.id

    // assign json items to each RecipeRow row
    for (const auto& recipe : j) {
        RecipeRow row;

        // Assign values to RecipeRow members
        row.id = idCounter++;
        row.name = recipe["NAME"];
        row.tags = recipe["TAG"];
        row.steps = recipe["STEPS"];

        // Assign ingredient details
        const auto& ingredientInfo = recipe["INGREDIENT_INFO"];
        for (const auto& ingredient : ingredientInfo) {
            IngredientDetail detail;
            detail.name = ingredient["name"];
            detail.amount = ingredient["amount"];
            row.ingredients.push_back(detail);
        }

        // Add the RecipeRow to recipeData vector
        recipeData.push_back(row);
    }
}

// Save to JSON file
RefriRecipe::~RefriRecipe() {

}

// Show All Recipes
void RefriRecipe::showRecipe() {
    vector<string> title = {"NAME", "TAGS", "INGREDIENTS"};
    vector<Row> recipeDataStrings = RefriRecipe::stringifyRecipe();

    table(title, recipeDataStrings); // print all rows in table format
}
vector<Row> RefriRecipe::stringifyRecipe(){
    vector<Row> result;

    for (const auto &recipe: recipeData) {
        Row row;

        // 1열: Recipe name
        row.values.push_back(recipe.name);
        // 2열: Tags
        string tags;
        for (const auto& tag : recipe.tags) {
            tags += tag + ", ";
        }
        tags = tags.substr(0, tags.size() - 2); // remove the trailing comma
        row.values.push_back(tags);
        // 3열: Ingredients
        string ingredients;
        for (const auto& ingredient : recipe.ingredients) {
            ingredients += CutDecimal(to_string(ingredient.amount), 1) + " " +
                           ingredient.name + ", ";
        }
        ingredients = ingredients.substr(0, ingredients.size() - 2); // remove the trailing comma
        row.values.push_back(ingredients);

        result.push_back(row);
    }

    return result;
}

// ------------------------------------------------------------------------------------------------------------
// Add Recipe
void RefriRecipe::addRecipe() {
    try {
        // UI : input name/tags/ingredients/steps
        Title("Add Recipe");

        Subtitle("Recipe Name");
        string recipeName = Input("Enter your recipe name");

        Subtitle("Recipe Tags");
        showPossibleTags();
        vector<string> recipeTagSelect = MultipleChoice(RecipeTagList, "Enter recipe's tags", "ex) Sweet, Fat");

        Subtitle("Recipe Ingredients");
        vector<IngredientDetail> recipeIngredients = inputIngredients();

        Subtitle("Recipe Steps");
        vector<string> recipeSteps = inputSteps();

        // manipulate recipeData : append newRecipe
        RecipeRow newRecipe;
        newRecipe.name = recipeName;
        newRecipe.tags = recipeTagSelect;
        newRecipe.steps = recipeSteps;
        newRecipe.ingredients = recipeIngredients;

        recipeData.push_back(newRecipe);

        // UI : Display success message
        Subtitle("Successfully added new recipe '" + recipeName + "'!");
    }
    catch (const std::exception &e) {
        // UI : Display error message
        Subtitle(e.what());
    }
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

vector<string> RefriRecipe::inputSteps() {
    vector<string> steps = json::array();
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

vector<IngredientDetail> RefriRecipe::inputIngredients() {
    vector<IngredientDetail> ingredientList;

    while (true) {
        IngredientDetail ingredient;

        // UI : Input ingredient name and amount
        // ..

        // foodData에 없다면 정보 추가
        // ..

        // Add the ingredient and amount to the ingredientList
        ingredientList.push_back(ingredient);
    }

    return ingredientList;

}

// Remove Recipe
void removeRecipe(){

}
