#ifndef REFRISYSTEM_REFRISYSTEM_H
#define REFRISYSTEM_REFRISYSTEM_H

#include "Storage.h"
#include "Recipe.h"

// RefriSystem 클래스
class RefriSystem{
    // composition of instance Storage
    string Storage_RawJSON_path = "../db/Storage.json";
    Storage refrigerator;

    // composition of instance Recipe
    string Recipe_RawJSON_path = "../db/RecipeData.json";
    Recipe refriRecipe;
public:
    RefriSystem();

    // MAIN MENU
    void MAIN_MENU();

    // each displays
    void Introduction();
    void Briefing();
    void Act();
    void Eat();

    // sub-menu of Act()
    void ManageStorage();
    void ShowRecipe();
};

#endif //REFRISYSTEM_REFRISYSTEM_H
