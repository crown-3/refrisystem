#include <fstream>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <algorithm>
#include <utility>

#include "../include/Recipe.h"
#include "../include/Interfaces.h"
#include "../utils/CutDecimal.h"
#include "../utils/Title.h"
#include "../utils/Input.h"
#include "../utils/ColorfulCli.h"
#include "../utils/Table.h"

using namespace std;
using namespace nlohmann;

// Possible Tags for Recipes
vector<string> RecipeTagList = {
        "Vegan", "Vegetarian",  // tags related to Dietary pattern
        "Cold", "Hot", "Sweet", "Salty", "Spicy", "Sour", "Savory", // tags related to Flavor
        "Fat", "Protein", "Carbohydrate", "Vitamin", "Mineral", // tags related to Nutrition
        "Healthy", "Premium", // // tags related to Miscellaneous
};

// Constructor
Recipe::Recipe(string data_path, Storage &ref)
: RawJSON_path(move(data_path)), storageRef(ref)
{
    dataManager = new RecipeDataManagement(data_path);
    recommendManager = new RecipeRecommendation(*this);
}

// Destructor : Save to JSON file & delete managers
Recipe::~Recipe() {
    dataManager->saveData();
    delete recommendManager;
    delete dataManager;
}

// ------------------------------------------------------------------------------------------------------------
// Showing Recipes
// ------------------------------------------------------------------------------------------------------------
void Recipe::showAllRecipe() {
    vector<string> title = {"NAME", "TAGS", "INGREDIENTS", "Makable?"};
    vector<Row> recipeDataStrings = stringifyRecipeForTable(dataManager->getData());

    table(title, recipeDataStrings); // print all rows in table format
}

void Recipe::showPartialRecipe(vector<RecipeItem> targetRecipeData) {
    vector<string> title = {"NAME", "TAGS", "INGREDIENTS", "Makable?"};
    vector<Row> recipeDataStrings = stringifyRecipeForTable(targetRecipeData);

    table(title, recipeDataStrings); // print all rows in table format
}


bool Recipe::checkRecipeExist(string recipeName) {
    bool isFound = false;

    for (const auto &recipe: dataManager->getData()) {
        if (recipe.name == recipeName) {
            isFound = true;
            break;
        }
    }

    return isFound;
};


vector<Row> Recipe::stringifyRecipeForTable(vector<RecipeItem> targetRecipeData) {
    vector<Row> result;

    for (const auto &recipe: targetRecipeData) {
        Row row;

        // 1열: Recipe name
        row.values.push_back(recipe.name);

        // 2열: Tags
        string tags;
        for (const auto &tag: recipe.tags) {
            tags += tag + ", ";
        }
        tags = tags.substr(0, tags.size() - 2); // remove the trailing comma
        row.values.push_back(tags);
        // 3열: Ingredients
        string ingredients;
        for (const auto &ingredient: recipe.ingredients) {
            ingredients += CutDecimal(to_string(ingredient.amount), 1) + " " +
                           ingredient.name + ", ";
        }
        ingredients = ingredients.substr(0, ingredients.size() - 2); // remove the trailing comma
        row.values.push_back(ingredients);
        // 4열 : Makable
        string makable = checkMakable(recipe) ? "YES" : "NO";
        row.values.push_back(makable);

        result.push_back(row);
    }

    return result;
}

// ------------------------------------------------------------------------------------------------------------
// Add Recipe
// ------------------------------------------------------------------------------------------------------------
void Recipe::addRecipeSequence() {
    Title("Add Recipe");

    Subtitle("Recipe Name");

    string recipeName;
    // check if the entered recipe name already exists
    while (true){
        recipeName = Input("Enter your recipe name");
        if (checkRecipeExist(recipeName)) {
            TextColor(RED, BLACK);
            cout << "The recipe name already exists." << endl;
            TextColor(WHITE, BLACK);
        } else {
            break;
        }
    }

    Subtitle("Recipe Tags");
    printPossibleTags();
    vector<string> recipeTagSelect = MultipleChoice(RecipeTagList, "Enter recipe's tags", "ex) Sweet, Fat");

    Subtitle("Recipe Ingredients");
    vector<IngredientDetail> recipeIngredients = inputIngredients();

    Subtitle("Recipe Steps");
    vector<string> recipeSteps = inputSteps();

    // manipulate recipeData : append newRecipe
    RecipeItem newRecipe;
    newRecipe.name = recipeName;
    newRecipe.tags = recipeTagSelect;
    newRecipe.steps = recipeSteps;
    newRecipe.ingredients = recipeIngredients;

    dataManager->addData(newRecipe);

    // UI : Display success message
    Subtitle("Successfully added new recipe '" + recipeName + "'!");
}

// helper function for addRecipe(): it shows possible tags
void Recipe::printPossibleTags() {
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

// helper function for addRecipe(): it takes input for recipe steps
vector<string> Recipe::inputSteps() {
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

// helper function for addRecipe(): it takes input for recipe ingredients
vector<IngredientDetail> Recipe::inputIngredients() {
    fstream dataFile;
    dataFile.open("../source/FoodData.json");

    json j;
    dataFile >> j;

    map<string, string> foodData;

    for (auto &element: j) {
        foodData[element["name"]] = element["storage"];
    }

    json ingredientList = json::array(); // JSON array to hold the ingredient list
    vector<IngredientDetail> recipeIngredients; // vector to hold the ingredient list

    while (true) {
        double amount;
        string name;
        IngredientDetail ingredient;

        cout << "Input an ingredient name and amount (-1 to finish): ";
        cin >> amount;
        if (amount == -1) {
            break;
        }
        cin.ignore();
        getline(cin, name);



        if (foodData.find(name) == foodData.end()) {
            char response;
            cout << "----| Add new ingredient '" << name << "' to food data? (y/n): ";
            cin >> response;
            if (response == 'y' || response == 'Y') {
                string storage;
                storage = MultipleChoice({"fridge", "freezer", "pantry"},
                                         "----| What is the suit storage for '" + name + "'?",
                                         "(fridge / freezer / pantry)")[0];
                foodData[name] = storage;
                json newElement = {
                        {"name",    name},
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
                cout << "----| Adding ingredient '" << name << "' canceled." << endl;
                TextColor(WHITE, BLACK);
                continue;
            }
        }

        ingredient.name = name;
        ingredient.amount = amount;

        // Add the ingredient and amount to the recipeIngredients
        recipeIngredients.push_back(ingredient);
    }

    return recipeIngredients;
}

// ------------------------------------------------------------------------------------------------------------
// Remove Recipe
// ------------------------------------------------------------------------------------------------------------
void Recipe::removeRecipeSequence() {
    Title("Remove Recipe");

    string recipeName;
    // check if the entered recipe name exists
    while (true){
        recipeName = Input("Enter the name of recipe you want to remove");
        if (!checkRecipeExist(recipeName)) {
            TextColor(RED, BLACK);
            cout << "The recipe name does not exists." << endl;
            TextColor(WHITE, BLACK);
        } else {
            break;
        }
    }

    dataManager->removeData(recipeName);
}

// ------------------------------------------------------------------------------------------------------------
// Recommend Recipe
// ------------------------------------------------------------------------------------------------------------
vector<RecipeItem> Recipe::recommendRecipe(string mood) {
    recommendManager->setUserMood(mood);
    return recommendManager->recommend(dataManager->getData());
}

// ------------------------------------------------------------------------------------------------------------

bool Recipe::checkMakable(RecipeItem recipe) {
    bool makable = true;
    for (const auto &ingredient: recipe.ingredients) {
        if (storageRef.checkUsableIngredientAmount(ingredient.name) < ingredient.amount) {
            makable = false;
            break;
        }
    }
    return makable;
}

int Recipe::countLackIngredients(RecipeItem recipe) {
    int lackCount = 0;
    for (const auto &ingredient: recipe.ingredients) {
        if (storageRef.checkUsableIngredientAmount(ingredient.name) < ingredient.amount) {
            lackCount ++;
        }
    }
    return lackCount;
}

vector<IngredientDetail> Recipe::checkLackIngredients(RecipeItem recipe) {
    vector<IngredientDetail> lackIngredientList;
    cout << "===================checkLackIngredient===================" << endl;
    for (const auto &ingredient: recipe.ingredients) {
        double ownIngAmount = storageRef.checkUsableIngredientAmount(ingredient.name);
        cout << "\t" << ingredient.name << " now have " << ownIngAmount << ", need"
             << ingredient.amount << " , so " << ((ownIngAmount < ingredient.amount) ? "NON-SUFFICIENT" : "SUFFICIENT")
             << endl;
        if (ownIngAmount < ingredient.amount) {
            IngredientDetail lackIng;
            lackIng.name = ingredient.name;
            lackIng.amount = ingredient.amount - ownIngAmount;
            lackIngredientList.push_back(lackIng);
            break;
        }
    }
    cout << "=========================================================" << endl;
    return lackIngredientList;
}