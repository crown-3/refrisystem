#ifndef REFRISYSTEM_REFRIRECIPE_H
#define REFRISYSTEM_REFRIRECIPE_H

#include <vector>
#include "../hooks/Table.h"
#include <nlohmann/json.hpp>
using namespace std;
using namespace nlohmann;

class RefriRecipe {
    // vector<Recipe> recipeList;
public:
    RefriRecipe();
    void MAIN_RefriStorage();

    // Show All Recipes
    static vector<Row> loadRecipeList(string data_path);
    static void showRecipeList();

    // Remove Recipe
    static void removeRecipe();

    // Adding Recipe
    static void showPossibleTags();
    static json inputIngredients();
    static json inputSteps();
    static void addRecipe();
    // ..
};

#endif //REFRISYSTEM_REFRIRECIPE_H
