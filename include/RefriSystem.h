#ifndef REFRISYSTEM_REFRISYSTEM_H
#define REFRISYSTEM_REFRISYSTEM_H

#include "Refrigerator.h"
#include "RefriRecipe.h"

// RefriSystem 클래스
class RefriSystem{
    // composition of instance Refrigerator
    string Storage_RawJSON_path = "../db/Storage.json";
    Refrigerator refrigerator;

    // composition of instance RefriRecipe
    string Recipe_RawJSON_path = "../db/RecipeData.json";
    RefriRecipe refriRecipe;
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
