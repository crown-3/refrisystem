#include "../include/Storage.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include "../utils/Table.h"
#include "../utils/ColorfulCli.h"
#include "../utils/Title.h"
#include "../utils/Input.h"

using namespace std;
using namespace nlohmann;


Storage::Storage(string data_path)
        : RawJSON_path(data_path) {
    dataManager = new StorageDataManagement(data_path);
}

Storage::~Storage() {
    dataManager->saveData();
    delete dataManager;
}

void Storage::printStorage() {
    dataManager->loadData();

    vector<Row> spoiledRow;
    vector<IngredientItem> spoiledIngredients = getIngredientsFreshnessLowerThan(DANGER);
    for (const auto &ing: spoiledIngredients) {
        Row newRow;
        newRow.values.push_back(to_string(ing.id));
        newRow.values.push_back(ing.storageType == StorageType::FRIDGE ? "fridge" :
                                ing.storageType == StorageType::FREEZER ? "freezer" : "pantry");
        newRow.values.push_back(ing.name);
        newRow.values.push_back(to_string(ing.freshness));
        newRow.values.push_back(to_string(ing.quantity));
        spoiledRow.push_back(newRow);
    }

    vector<Row> fridgeRow;
    vector<Row> freezerRow;
    vector<Row> pantryRow;
    for (const auto &ing: dataManager->getData()) {
        if (ing.storageType == StorageType::FRIDGE) {
            Row newRow;
            newRow.values.push_back(to_string(ing.id));
            newRow.values.push_back(ing.name);
            newRow.values.push_back(to_string(ing.freshness));
            newRow.values.push_back(to_string(ing.quantity));
            fridgeRow.push_back(newRow);
        } else if (ing.storageType == StorageType::FREEZER) {
            Row newRow;
            newRow.values.push_back(to_string(ing.id));
            newRow.values.push_back(ing.name);
            newRow.values.push_back(to_string(ing.freshness));
            newRow.values.push_back(to_string(ing.quantity));
            freezerRow.push_back(newRow);
        } else if (ing.storageType == StorageType::PANTRY) {
            Row newRow;
            newRow.values.push_back(to_string(ing.id));
            newRow.values.push_back(ing.name);
            newRow.values.push_back(to_string(ing.freshness));
            newRow.values.push_back(to_string(ing.quantity));
            pantryRow.push_back(newRow);
        }
    }

    vector<string> alertTitle = {"ID", "STORAGE", "NAME", "FRESHNESS", "QUANTITY"};

    // check if the spoiled items are empty
    if (spoiledRow.empty()) {
        TextColor(BLACK, GREEN);
        cout << "No ingredients are about to expire!";
        TextColor(WHITE, BLACK);
        cout << endl;
    } else {
        TextColor(BLACK, RED);
        cout << "!ALERT! These ingredients are about to expire!";
        TextColor(WHITE, BLACK);
        cout << endl;
        table(alertTitle, spoiledRow);
    }

    vector<string> storageTitle = {"ID", "NAME", "FRESHNESS", "QUANTITY"};

    TextColor(BLACK, CYAN);
    cout << "Fridge";
    TextColor(WHITE, BLACK);
    cout << endl;
    table(storageTitle, fridgeRow);

    TextColor(BLACK, CYAN);
    cout << "Freezer";
    TextColor(WHITE, BLACK);
    cout << endl;
    table(storageTitle, freezerRow);

    TextColor(BLACK, CYAN);
    cout << "Pantry";
    TextColor(WHITE, BLACK);
    cout << endl;
    table(storageTitle, pantryRow);
}

bool Storage::isInStorage(std::string ingredientName) {
    for (const auto &ing: dataManager->getData()) {
        if (ing.name == ingredientName) {
            return true;
        }
    }
    return false;
}


// this function checks if there is any  spoiled ingredients
bool Storage::isSpoiledEmpty(int criteria) {
    return getIngredientsFreshnessLowerThan(criteria).empty();
}

bool Storage::isDangerEmpty() {
    return isSpoiledEmpty(DANGER);
}

bool Storage::isRottenEmpty() {
    return isSpoiledEmpty(ROTTEN);
}


// this function deletes the spoiled ingredients / clear the freshness of the spoiled ingredients, and it also returns a string of the spoiled ingredients
string Storage::spoiledIngredientsBehavior(int criteria, SpoiledIngredientsBehavior behavior) {
    vector<IngredientItem> spoiledFoods = getIngredientsFreshnessLowerThan(criteria);
    stringstream ss;

    for (size_t i = 0; i < spoiledFoods.size(); ++i) {
        if (i == spoiledFoods.size() - 1) { // last item
            ss << "and " << spoiledFoods[i].name << " * " << spoiledFoods[i].quantity;
        } else {
            ss << spoiledFoods[i].name << " * " << spoiledFoods[i].quantity << ", ";
        }

        // behavior
        switch (behavior) {
            case DELETE:
                dataManager->removeData(spoiledFoods[i].name, spoiledFoods[i].quantity);
                break;
            case CLEAR:
                dataManager->clearFreshness(spoiledFoods[i].name, 100, criteria);
                break;
            default:
                // case of "NONE"
                break;
        }
    }

    return ss.str();
}

string Storage::dangerIngredientsBehavior(SpoiledIngredientsBehavior behavior) {
    return spoiledIngredientsBehavior(DANGER, behavior);
}

string Storage::rottenIngredientsBehavior(SpoiledIngredientsBehavior behavior) {
    return spoiledIngredientsBehavior(ROTTEN, behavior);
}


// this function checks the usable amount of an ingredient
double Storage::checkUsableIngredientAmount(string ingredientName) {
    return checkIngredientAmount(ingredientName, ROTTEN);
}

bool Storage::checkIngredientPerishable(string ingredientName) {
    return (checkIngredientLowestFreshness(ingredientName) <= PERISHABLE);
}


// this function adds an ingredient to the storage
void Storage::addIngredientSequence() {
    IngredientItem newIngredient;

    string name;
    int quantity;
    StorageType storageType;

    Title("Add Ingredient");

    Subtitle("Ingredient Name");
    name = Input("Enter your ingredient name");

    Subtitle("Ingredient Quantity");
    quantity = InputInteger(0, 200, "Enter the quantity of ingredient", "Integer between 0 and 200");

    Subtitle("Storage type for the ingredient");
    string storageTypeStr = MultipleChoice({"fridge", "freezer", "pantry"},
                                           "Enter the type of storage to put the corresponding ingredient in.", "fridge / freezer / pantry")[0];

    transform(storageTypeStr.begin(), storageTypeStr.end(), storageTypeStr.begin(), ::tolower);
    if (storageTypeStr == "fridge") {
        storageType = StorageType::FRIDGE;
    } else if (storageTypeStr == "freezer") {
        storageType = StorageType::FREEZER;
    } else if (storageTypeStr == "pantry") {
        storageType = StorageType::PANTRY;
    } else {
        Error("Invalid storage type: internal error");
        return;
    }

    newIngredient.name = name;
    newIngredient.quantity = quantity;
    newIngredient.freshness = FRESH;
    newIngredient.storageType = storageType;

    dataManager->addData(newIngredient);

    Subtitle("Successfully added ingredient '" + name + "' to the storage!");
    PressEnterToContinue();
}


// this function removes an ingredient from the storage
void Storage::removeIngredientSequence() {
    Title("Remove Ingredient");

    string name;
    int quantity;

    Subtitle("Ingredient Name");

    while (true) {
        name = Input("Enter the ingredient name to remove");
        if (isInStorage(name)) {
            break;
        } else {
            TextColor(RED, BLACK);
            cout << "Ingredient '" << name << "' is not in the storage!";
            TextColor(WHITE, BLACK);
            cout << endl;
        }
    }


    Subtitle("Ingredient Quantity");
    quantity = InputInteger(0, 200, "Enter how much ingredient you want to remove", "Integer between 0 and 200");

    dataManager->removeData(name, quantity);

    Subtitle("Successfully removed ingredient '" + name + "' from the storage!");
    PressEnterToContinue();
}

void Storage::removeIngredientsInRecipe(RecipeItem recipe, bool useAll) {
    if (useAll) {
        // makable -> use each ingredients * (recipe.ingredient.amount)
        for (const auto &ing: recipe.ingredients) {
            dataManager->removeData(ing.name, ing.amount);
        }
    } else {
        // non-makable -> use each ingredients * [isIngSufficient ? (recipe.ingredient.amount) : (all)]
        // (this means ; For insufficient ingredient, Fill and use all)
        for (const auto &ing: recipe.ingredients) {
            double ownIngAmount = checkUsableIngredientAmount(ing.name);
            if (ownIngAmount >= ing.amount) {
                dataManager->removeData(ing.name, ing.amount);
            } else {
                if (ownIngAmount > 0)
                    dataManager->removeData(ing.name, ownIngAmount);
            }
        }
    }
}


// Get ingredients with freshness lower than a given criteria
vector<IngredientItem> Storage::getIngredientsFreshnessLowerThan(int criteria) {
    vector<IngredientItem> spoiledFoods;
    for (const auto& ing : dataManager->getData()) {
        if (ing.freshness <= criteria) {
            spoiledFoods.push_back(ing);
        }
    }
    return spoiledFoods;
}

// Check amount of an ingredient
double Storage::checkIngredientAmount(string ingredientName, int freshThreshold) {
    double amount = 0;
    int count = 0;

    for (const auto& ing : dataManager->getData()) {
        if ((ing.name == ingredientName) && (ing.freshness > freshThreshold)) {
            amount += ing.quantity;
        }
    }

    return amount;
}

// Check freshness of an ingredient
double Storage::checkIngredientLowestFreshness(string ingredientName) {
    double lowestFreshness = 0;
    bool isInStorage = false;

    for (auto& ing : dataManager->getData()) {
        if (ing.name == ingredientName) {
            if(!isInStorage || ing.freshness < lowestFreshness){
                isInStorage = true;
                lowestFreshness = ing.freshness;
            }
        }
    }

    if (isInStorage) {
        throw runtime_error("Ingredient not found in the refrigerator. [checkIngredientLowestFreshness]");
    }

    return lowestFreshness;
}