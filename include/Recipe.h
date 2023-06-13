#ifndef REFRISYSTEM_RECIPE_H
#define REFRISYSTEM_RECIPE_H

#include <vector>
#include "../utils/Table.h"
#include "Storage.h"
#include <nlohmann/json.hpp>
#include "Interfaces.h"
#include "RecipeRecommendation.h"
#include "RecipeDataManagement.h"

using namespace std;
using namespace nlohmann;

// Possible kinds of RecipeTag
extern vector<string> RecipeTagList;

// Recipe class
class RecipeRecommendation;
class Recipe {
private:
    string RawJSON_path;
    RecipeDataManagement* dataManager;
    RecipeRecommendation* recommendManager;
    Storage &storageRef; // Reference to Storage instance
public:
    Recipe(string data_path, Storage &ref);
    ~Recipe();

    void showAllRecipe();
    void showPartialRecipe(vector<RecipeItem> targetRecipeData);

    bool checkRecipeExist(string recipeName);
    vector<Row> stringifyRecipeForTable(vector<RecipeItem> targetRecipeData); // helper function for showRecipe()

    // ------------------------------------------------------------------------------------------------------------
    // Add Recipe
    // ------------------------------------------------------------------------------------------------------------
    void addRecipeSequence();

    void printPossibleTags(); // helper function for addRecipe()
    vector<IngredientDetail> inputIngredients(); // helper function for addRecipe()
    vector<string> inputSteps(); // helper function for addRecipe()

    // ------------------------------------------------------------------------------------------------------------
    // Remove Recipe
    // ------------------------------------------------------------------------------------------------------------
    void removeRecipeSequence();

    // ------------------------------------------------------------------------------------------------------------
    // Recommend Recipe
    // ------------------------------------------------------------------------------------------------------------
    vector<RecipeItem> recommendRecipe(string mood);

    // ------------------------------------------------------------------------------------------------------------
    // Inspect Recipe
    // ------------------------------------------------------------------------------------------------------------
    void inspectRecipeSequence();


    // helper functions for RecipeRecommendation
    bool checkMakable(RecipeItem recipe);
    int countLackIngredients(RecipeItem recipe);
    vector<IngredientDetail> checkLackIngredients(RecipeItem recipe);
};

#endif //REFRISYSTEM_RECIPE_H