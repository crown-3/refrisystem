#include <iostream>
#include <chrono>
#include <thread>
#include "../include/RefriSystem.h"
#include "../include/Storage.h"
#include "../include/Consts.h"
#include "../utils/ColorfulCli.h"
#include "../utils/Title.h"
#include "../utils/Input.h"

using namespace std;

//string RefriSystem::Storage_RawJSON_path = "../db/Storage.json";
//Storage RefriSystem::refrigerator(RefriSystem::Storage_RawJSON_path);

RefriSystem::RefriSystem()
 : refrigerator(Storage_RawJSON_path), refriRecipe(Recipe_RawJSON_path, refrigerator)
{ }

void RefriSystem::MAIN_MENU() {
    RefriSystem::Introduction();
    PressEnterToContinue();
    RefriSystem::Briefing();
    RefriSystem::Act();
}

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
    vector<IngredientItem> spoiledFoods = refrigerator.getIngredientsFreshnessLowerThan(0);
    stringstream ss1;

    if (!spoiledFoods.empty()) {
        for (size_t i = 0; i < spoiledFoods.size(); ++i) {
            if (i == spoiledFoods.size() - 1) { // last item
                ss1 << "and " << spoiledFoods[i].name << " * " << spoiledFoods[i].quantity;
            } else {
                ss1 << spoiledFoods[i].name << " * " << spoiledFoods[i].quantity << ", ";
            }
            refrigerator.removeIngredient(spoiledFoods[i].name, spoiledFoods[i].quantity);
        }
        ss1 << " have rotten, so were thrown away.";
    } else {
        ss1 << "No ingredients have rotten.";
    }

    // Tomato * 2, Ketchup * 0.4, and Onion * 2 have rotten, so was thrown away.
    Subtitle(ss1.str());

    // Print the danger ingredients
    vector<IngredientItem> dangerFoods = refrigerator.getIngredientsFreshnessLowerThan(10);
    stringstream ss2;
    if (!dangerFoods.empty()) {
        ss2 << dangerFoods[0].name << " * " << dangerFoods[0].quantity;
        for (size_t i = 1; i < dangerFoods.size(); ++i) {
            if (i == dangerFoods.size() - 1) { // last item
                ss2 << ", and " << dangerFoods[i].name << " * " << dangerFoods[i].quantity;
            } else {
                ss2 << ", " << dangerFoods[i].name << " * " << dangerFoods[i].quantity;
            }
        }
        ss2 << " are in dangerous status.";

        // Tomato * 2, Ketchup * 0.4, and Onion * 2 are in dangerous status.
        Subtitle(ss2.str() + " Are you gonna dump them all, master?");

        int answer1 = SingleChoiceWithNumber({"Yes", "No"});
        switch (answer1) {
            case 1: {
                Subtitle("Certainly! Do you want to refill them with fresh ones, master?");
                int answer2 = SingleChoiceWithNumber({"Yes", "No"});
                switch (answer2) {
                    case 1:
                        for (size_t i = 0; i < dangerFoods.size(); ++i) {
                            refrigerator.clearFreshness(dangerFoods[i].name, 100);
                        }
                        break;
                    case 2:
                        for (size_t i = 0; i < dangerFoods.size(); ++i) {
                            refrigerator.removeIngredient(dangerFoods[i].name, dangerFoods[i].quantity);
                        }
                        break;
                };

                break;
            }
        }
    } else {
        ss2 << "No ingredients are in danger.";
        Subtitle(ss2.str());
    }
    refrigerator.saveIngredient();

}

void RefriSystem::Act() {
    Subtitle("Are you gonna eat something, master?");
    int answer1 = SingleChoiceWithNumber({"Yes", "No"});
    switch (answer1) {
        case 1: {
            RefriSystem::Eat();
            break;
        }
        case 2: {
            // storage or recipe
            Subtitle("Then, what do you want to do, master?");
            int answer2 = SingleChoiceWithNumber({"Open Storage", "Show Recipe"});

            switch (answer2) {
                case 1: {
                    ManageStorage();
                    break;
                }
                case 2: {
                    ShowRecipe();
                    break;
                }
            }
            break;
        }
    }
}

// STORAGE--------------------------------------------------------------------------------------------------------------
void RefriSystem::ManageStorage() {
    Title("Open Storage");
    refrigerator.printStorage();
    PressEnterToContinue();
    Act();
}

// RECIPE---------------------------------------------------------------------------------------------------------------
void RefriSystem::ShowRecipe() {
    Title("Show Recipe");
    refriRecipe.showAllRecipe();
}

void RefriSystem::Eat() {
    // Title for Part <Cook & Eat>
    Title("Cook & Eat");

    // Ask for the User's mood
    Subtitle("How do you do today, master?");
    int answer_mood = SingleChoiceWithNumber(MoodQ);
    string mood = MoodA[answer_mood];

    // Response output according to User's mood
    Subtitle(MoodR[mood]);

    // Recommend recipes according to User's mood
    vector<RecipeItem> recipePriorityList = refriRecipe.recommendRecipe(mood);
    while (!recipePriorityList.empty()){
        // Extract first 3 recipes
        vector<RecipeItem> extractedRecipes;
        for (int i = 0; i < 3 && !recipePriorityList.empty(); ++i) {
            extractedRecipes.push_back(recipePriorityList.front());
            recipePriorityList.erase(recipePriorityList.begin());
        }

        // Output detailed info of first 3 recipes
        Subtitle("Then, I recommend these foods. ");
        Subtitle("Here is the detailed info of TOP 3 recommended recipes");
        refriRecipe.showPartialRecipe(extractedRecipes);

        // Ask to choose recipe
        Subtitle("Please choose the food you want. ");
        int answer_recipeChosen = SingleChoiceWithNumber({
            extractedRecipes[0].name,
            extractedRecipes[1].name,
            extractedRecipes[2].name,
            "There's nothing I like."
        });

        // When recipe is not selected
        if (answer_recipeChosen == 4) {
            continue;
        }

        RecipeItem selectedRecipe = extractedRecipes[answer_recipeChosen-1];
        // Whether selectedRecipe is makable, Ask Cook OR Buy
        bool selectedIsMakable = refriRecipe.checkMakable(selectedRecipe);
        if (selectedIsMakable){
            Subtitle(selectedRecipe.name +" is chosen, We can make this food with the ingredients in the storage! ");
            Subtitle("Would you like to cook now, master? ");
        } else {
            Subtitle(selectedRecipe.name +" is chosen, But we need more of the following ingredients: ");
            vector<IngredientDetail> lackIngredientList = refriRecipe.checkLackIngredient(selectedRecipe);
            for (size_t i = 0; i < lackIngredientList.size(); ++i) {
                cout << "\t" << lackIngredientList[i].name << " * " << lackIngredientList[i].amount << endl;
            }
            Subtitle("Shall we buy these ingredients and then cook now, master? ");
        }
        // Receive answer for ask:[Cook/Buy]
        int answer_willCook = SingleChoiceWithNumber({
                                                             "Yes, please!",
                                                             "No, I wanna go back to the food list.",
                                                             "No, I wanna go back to home."
                                                     });
        switch (answer_willCook) {
            case 1:
                // UI : display Starting Cook message
                Subtitle("All right! I'll prepare a perfect meal, master. ");
                TextColor(CYAN, BLACK);
                cout << R"( ______     ______     ______     __  __     __     __   __     ______ )" << endl;
                cout << R"(/\  ___\   /\  __ \   /\  __ \   /\ \/ /    /\ \   /\ "-.\ \   /\  ___\ )" << endl;
                cout << R"(\ \ \____  \ \ \/\ \  \ \ \/\ \  \ \  _"-.  \ \ \  \ \ \-.  \  \ \ \__ \ )" << endl;
                cout << R"( \ \_____\  \ \_____\  \ \_____\  \ \_\ \_\  \ \_\  \ \_\\"\_\  \ \_____\)" << endl;
                cout << R"(  \/_____/   \/_____/   \/_____/   \/_/\/_/   \/_/   \/_/ \/_/   \/_____/ )" << endl << endl;
                for (int i = 5; i >= 0; i--) {
                    TextColor(BLACK, CYAN);
                    cout << "Refri is cooking ... (" << i << "s)";
                    TextColor(WHITE, BLACK);
                    cout << endl;
                    this_thread::sleep_for(chrono::seconds(1));
                }
                // [Cook] selected food -> update(reduce quantity) refrigerator.ingredientData
                if(selectedIsMakable) {
                    // makable -> use each ingredients * (recipe.ingredient.amount)
                    for (const auto& ing : selectedRecipe.ingredients) {
                        refrigerator.removeIngredient(ing.name, ing.amount);
                    }
                } else {
                    // non-makable -> use each ingredients * [isIngSufficient ? (recipe.ingredient.amount) : (all)]
                    // (this means ; For insufficient ingredient, Fill and use all)
                    for (const auto& ing : selectedRecipe.ingredients) {
                        double ownIngAmount = refrigerator.checkAmount(ing.name, 0);
                        // cout << ing.name << "\t" << ownIngAmount << " :::: now having ------- " << endl; // TEST CODE
                        if (ownIngAmount >= ing.amount){
                            refrigerator.removeIngredient(ing.name, ing.amount);
                        } else {
                            if (ownIngAmount > 0)
                                refrigerator.removeIngredient(ing.name, ownIngAmount);
                        }
                        cout << "\t" << ing.name << "\t" << ing.amount << " used" << endl;
                    }
                }
                // UI : display Succeed Cook message
                TextColor(BLACK, CYAN);
                cout << selectedRecipe.name << " is ready!" << endl;
                TextColor(WHITE, BLACK);
                break;
            case 2:
                // (2) No, I wanna go back to the food list.

                break;
            case 3:
                // (3) No, I wanna go back to home.
                break;
        }


        break;
    }

}
