#include <fstream>
#include <nlohmann/json.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

#include "../include/RefriRecipe.h"
#include "../hooks/CutDecimal.h"
#include "../hooks/Title.h"
#include "../hooks/Input.h"
#include "../hooks/ColorfulCli.h"

using namespace std;
using namespace nlohmann;

// 가능한 RecipeTag 종류
vector<string> RecipeTagList = {
        "Vegan", "Vegetarian",  // 성향 tag
        "Healthy", "Premium",   // 기타 tag
        "Cold", "Hot", "Sweet", "Salty", "Spicy", "Sour", "Savory", // 맛 관련 tag
        "Fat", "Protein", "Carbohydrate", "Vitamin", "Mineral"      // 영양성분 관련 tag
};

// RecipeTag 중요도 정보
map<string, vector<string>> moodPreference = {      // 기분에 따른 선호 RecipeTag
        {"happy", {"Healthy", "Cold", "Salty", "Sour", "Vitamin", "Mineral"}},
        {"moody", {"Premium", "Sweet", "Savory", "Fat", "Carbohydrate", "Vitamin", "Mineral"}},
        {"exhausted", {"Healthy", "Sweet", "Fat", "Protein", "Carbohydrate"}},
        {"fine", {"Healthy", "Premium", "Cold", "Hot", "Sweet", "Savory", "Protein", "Carbohydrate", "Mineral"}},
        {"mentally_tired", {"Premium", "Hot", "Spicy", "Fat", "Vitamin"}}
};
map<int, vector<string>> userPreference = {      // 사용자의 RecipeTag 선호순서
        {1, {"Hot", "Sweet", "Salty", "Spicy", "Sour"}},
        {2, {"Premium", "Fat", "Protein", "Carbohydrate"}},
        {3, {"Vitamin", "Mineral", "Savory", "Cold"}},
        {4, {"Healthy", "Vegan", "Vegetarian"}}
};

// ------------------------------------------------------------------------------------------------------------
// Load from JSON file
RefriRecipe::RefriRecipe(string data_path, Refrigerator& ref)
: RawJSON_path(data_path), refrigeratorRef(ref)
{
    loadRecipeData();
}
void RefriRecipe::loadRecipeData() {
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
    saveRecipeData();
}
void RefriRecipe::saveRecipeData() {
    // load json file with data_path
    ofstream jsonFile(RawJSON_path);
    if (!jsonFile.is_open()) {
        throw runtime_error("Storage.json file could not be opened for writing. Check if the file exists and its permissions.");
    }

    // assign RecipeRow rows to each json item
    json j;
    for (auto& element : recipeData) {
        json sub;
        sub["NAME"] = element.name;
        sub["TAG"] = element.tags;
        sub["STEPS"] = element.steps;

        vector<json> ingredientInfo;
        for (const auto& ingredient : element.ingredients) {
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

// Show All Recipes
void RefriRecipe::showAllRecipe() {
    vector<string> title = {"NAME", "TAGS", "INGREDIENTS", "Makable?"};
    vector<Row> recipeDataStrings = stringifyRecipe(recipeData);

    table(title, recipeDataStrings); // print all rows in table format
}
void RefriRecipe::showPartialRecipe(vector<RecipeRow> targetRecipeData) {
    vector<string> title = {"NAME", "TAGS", "INGREDIENTS", "Makable?"};
    vector<Row> recipeDataStrings = stringifyRecipe(targetRecipeData);

    table(title, recipeDataStrings); // print all rows in table format
}
vector<Row> RefriRecipe::stringifyRecipe(vector<RecipeRow> targetRecipeData){
    vector<Row> result;

    for (const auto &recipe: targetRecipeData) {
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
        // 4열 : Makable
        string makable = checkMakable(recipe) ? "YES" : "NO";
        row.values.push_back(makable);

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
    catch (const exception &e) {
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

// ------------------------------------------------------------------------------------------------------------
// Recommend Recipe
vector<RecipeRow> RefriRecipe::recommendRecipe(string mood) {
    // Step 1: Select rows with tags matching moodPreference
    vector<RecipeRow> matchingRows;
    for (const auto& recipe : recipeData) {
        bool hasMatchingTag = false;
        for (const auto& tag : recipe.tags) {
            const auto& preference = moodPreference.find(mood);
            if (preference != moodPreference.end()) {
                const auto& preferenceTags = preference->second;
                if (find(preferenceTags.begin(), preferenceTags.end(), tag) != preferenceTags.end()) { // std::find 사용
                    hasMatchingTag = true;
                    break;
                }
            }
        }
        if (hasMatchingTag) {
            matchingRows.push_back(recipe);
        }
    }

    // Step 2: Sort rows based on userPreference order
    sort(matchingRows.begin(), matchingRows.end(), [&](const RecipeRow& a, const RecipeRow& b) { // std::sort 사용
        const auto& preference = userPreference.find(a.id);
        if (preference != userPreference.end()) {
            const auto& preferenceTags = preference->second;
            auto aIt = find_if(a.tags.begin(), a.tags.end(), [&](const string& tag) { // std::find_if 사용
                return find(preferenceTags.begin(), preferenceTags.end(), tag) != preferenceTags.end(); // std::find 사용
            });
            auto bIt = find_if(b.tags.begin(), b.tags.end(), [&](const string& tag) { // std::find_if 사용
                return find(preferenceTags.begin(), preferenceTags.end(), tag) != preferenceTags.end(); // std::find 사용
            });
            return aIt < bIt;
        }
        return false;
    });

    // Step 3: Sort makable recipes first
    sort(matchingRows.begin(), matchingRows.end(), [&](const RecipeRow& a, const RecipeRow& b) {
        bool aMakable = checkMakable(a);
        bool bMakable = checkMakable(b);

        if (aMakable && !bMakable) {
            return true;  // a is makable, but b is not, so a should come first
        } else if (!aMakable && bMakable) {
            return false;  // b is makable, but a is not, so b should come first
        }

        // Both a and b are either makable or not makable
        // Sort based on the number of available ingredients in the refrigerator
        int aAvailableIngredients = 0;
        for (const auto& ingredient : a.ingredients)
            if (refrigeratorRef.checkAmount(ingredient.name, 0) >= ingredient.amount)
                aAvailableIngredients++;
        int bAvailableIngredients = 0;
        for (const auto& ingredient : b.ingredients)
            if (refrigeratorRef.checkAmount(ingredient.name, 0) >= ingredient.amount)
                bAvailableIngredients++;
        return aAvailableIngredients > bAvailableIngredients;
    });

    return matchingRows;
}
bool RefriRecipe::checkMakable(RecipeRow recipe) {
    bool makable = true;
    for (const auto& ingredient : recipe.ingredients) {
        if (refrigeratorRef.checkAmount(ingredient.name, 0) < ingredient.amount) {
            makable = false;
            break;
        }
    }
    return makable;
}
vector<IngredientDetail> RefriRecipe::checkLackIngredient(RecipeRow recipe) {
    vector<IngredientDetail> lackIngredientList;
    cout << "===================checkLackIngredient===================" << endl;
    for (const auto& ingredient : recipe.ingredients) {
        double ownIngAmount = refrigeratorRef.checkAmount(ingredient.name, 0);
        cout << "\t" << ingredient.name << " now have " << ownIngAmount << ", need"
            << ingredient.amount << " , so " << ((ownIngAmount < ingredient.amount) ? "NON-SUFFICIENT" : "SUFFICIENT") << endl;
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