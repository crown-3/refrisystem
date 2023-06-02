#include <iostream>
#include "../include/RefriSystem.h"
#include "../include/Refrigerator.h"
#include "../hooks/ColorfulCli.h"
#include "../hooks/Title.h"

using namespace std;

void RefriSystem::Introduction() {

    TextColor(CYAN, BLACK);
    cout << R"( ______     __    __     ______     ______     ______      ______     ______     ______   ______     __ )" << endl;
    cout << R"(/\  ___\   /\ "-./  \   /\  __ \   /\  == \   /\__  _\    /\  == \   /\  ___\   /\  ___\ /\  == \   /\ \   )" << endl;
    cout << R"(\ \___  \  \ \ \-./\ \  \ \  __ \  \ \  __<   \/_/\ \/    \ \  __<   \ \  __\   \ \  __\ \ \  __<   \ \ \  )" << endl;
    cout << R"( \/\_____\  \ \_\ \ \_\  \ \_\ \_\  \ \_\ \_\    \ \_\     \ \_\ \_\  \ \_____\  \ \_\    \ \_\ \_\  \ \_\ )" << endl;
    cout << R"(  \/_____/   \/_/  \/_/   \/_/\/_/   \/_/ /_/     \/_/      \/_/ /_/   \/_____/   \/_/     \/_/ /_/   \/_/ )" << endl << endl;
    TextColor(BLACK, CYAN);
    cout << "WELCOME TO THE SMART REFRI SYSTEM." << endl;
    TextColor(WHITE, BLACK);
}

void RefriSystem::Briefing() {

    // Title
    Title("Briefing");

    // Print and remove the rotten ingredients
    vector<Ingredient> spoiledFoods = Refrigerator::getIngredientsFreshnessLowerThan(0);
    stringstream ss1;

    if (!spoiledFoods.empty()) {
        ss1 << spoiledFoods[0].name << " * " << spoiledFoods[0].quantity;
        for (size_t i = 1; i < spoiledFoods.size(); ++i) {
            if (i == spoiledFoods.size() - 1) { // last item
                ss1 << ", and " << spoiledFoods[i].name << " * " << spoiledFoods[i].quantity;
            } else {
                ss1 << ", " << spoiledFoods[i].name << " * " << spoiledFoods[i].quantity;
            }
            Refrigerator::removeIngredient(spoiledFoods[i].name, spoiledFoods[i].quantity);
        }
        ss1 << " have rotten, so were thrown away.";
    } else {
        ss1 << "No ingredients have rotten.";
    }

    // Tomato * 2, Ketchup * 0.4, and Onion * 2 have rotten, so was thrown away.
    Subtitle(ss1.str());


    // Print the danger ingredients
    vector<Ingredient> dangerFoods = Refrigerator::getIngredientsFreshnessLowerThan(10);
    stringstream ss2;
    if (!spoiledFoods.empty()) {
        ss2 << spoiledFoods[0].name << " * " << spoiledFoods[0].quantity;
        for (size_t i = 1; i < spoiledFoods.size(); ++i) {
            if (i == spoiledFoods.size() - 1) { // last item
                ss2 << ", and " << spoiledFoods[i].name << " * " << spoiledFoods[i].quantity;
            } else {
                ss2 << ", " << spoiledFoods[i].name << " * " << spoiledFoods[i].quantity;
            }
        }
        ss2 << " are in dangerous status.";

        // Tomato * 2, Ketchup * 0.4, and Onion * 2 are in dangerous status.
        Subtitle(ss2.str());
        Subtitle("Are you gonna dump them all, master?");

    } else {
        ss2 << "No ingredients are in danger.";
        Subtitle(ss2.str());
    }

}

void RefriSystem::Act() {
    Subtitle("Are you gonna eat something, master?");

}

void RefriSystem::MAIN_MENU() {
    RefriSystem::Introduction();
}