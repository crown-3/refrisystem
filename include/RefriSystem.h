#ifndef REFRISYSTEM_REFRISYSTEM_H
#define REFRISYSTEM_REFRISYSTEM_H

#include "Storage.h"
#include "Recipe.h"


class RefriSystem{
    // composition of instance Storage
    string Storage_RawJSON_path = "../../../db/Storage.json";
    Storage storage;

    // composition of instance Recipe
    string Recipe_RawJSON_path = "../../../db/RecipeData.json";
    Recipe recipe;
public:
    RefriSystem();

    // MAIN MENU
    void MAIN_MENU();

    // each displays
    void Introduction();
    void Briefing();
    void Act();
    void Eat();
    void Cook(string mood);

    // sub-menu of Act()
    void ManageStorage();
    void ManageRecipe();
};

#endif //REFRISYSTEM_REFRISYSTEM_H
