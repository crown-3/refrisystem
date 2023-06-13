#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
#include "../include/RefriSystem.h"
#include "../include/Storage.h"
#include "../utils/ColorfulCli.h"
#include "../utils/Title.h"
#include "../utils/Input.h"

using namespace std;

// ------------------------------------------------------------------------------------------------------------
// Constants used for User Interaction : Ask List & User Answer & Responses
// ------------------------------------------------------------------------------------------------------------

// [Mood] Question List for User Mood
vector<string> MoodQ = {
        "I'm very happy now.",
        "I feel moody today.",
        "I'm exhausted because of physical movement.",
        "I'm just fine.",
        "I'm mentally tired."
};

// [Mood] Switch number answer to Mood answer
map<int, string> MoodA = {
        {1, "happy"},
        {2, "moody"},
        {3, "exhausted"},
        {4, "fine"},
        {5, "mentally_tired"}
};

// [Mood] Responses for each Mood answer
map<string, string> MoodR = {
        {"happy",          "Oh, I'm really glad to hear that!"},
        {"moody",          "Oh, I understand how you feel."},
        {"exhausted",      "Oh, you need to recharge."},
        {"fine",           "Oh, that's nice."},
        {"mentally_tired", "Oh, I'm so sorry to hear that."}
};

// ------------------------------------------------------------------------------------------------------------
// Function Implementations
// -----------------------------------------------------------------------------------------------------------

//string RefriSystem::Storage_RawJSON_path = "../db/Storage.json";
//Storage RefriSystem::storage(RefriSystem::Storage_RawJSON_path);

RefriSystem::RefriSystem() :
        storage(Storage_RawJSON_path),
        recipe(Recipe_RawJSON_path, storage) {}

void RefriSystem::MAIN_MENU() {
    RefriSystem::Introduction();

    PressEnterToContinue();
    RefriSystem::Briefing();
    RefriSystem::Act();
}

void RefriSystem::Introduction() {

    TextColor(CYAN, BLACK);
    cout << R"(

 .====================================================================================.
||    ______     __    __     ______     ______     ______           ______________   ||
||   /\  ___\   /\ "-./  \   /\  __ \   /\  == \   /\__  _\        /              /|  ||
||   \ \___  \  \ \ \-./\ \  \ \  __ \  \ \  __<   \/_/\ \/       /______________/ |  ||
||    \ \_____\  \ \_\ \ \_\  \ \_\ \_\  \ \_\ \_\    \ \_\       | .            | |  ||
||     \/_____/   \/_/  \/_/   \/_/\/_/   \/_/ /_/     \/_/       | !            | |  ||
||     ______     ______     ______   ______     __               |______________| |  ||
||    /\  == \   /\  ___\   /\  ___\ /\  == \   /\ \              | .            | |  ||
||    \ \  __<   \ \  __\   \ \  __\ \ \  __<   \ \ \             | |            | /  ||
||     \ \_\ \_\  \ \_____\  \ \_\    \ \_\ \_\  \ \_\            | !            |/   ||
||      \/_/ /_/   \/_____/   \/_/     \/_/ /_/   \/_/            |______________/    ||
||                                                                                    ||
 `===================================================================================='
)" << endl;


    TextColor(BLACK, CYAN);
    cout << "WELCOME TO THE SMART REFRI SYSTEM." << endl;
    TextColor(WHITE, BLACK);
}

void RefriSystem::Briefing() {

    // Title
    Title("Briefing");

    // Briefing about Rotten Ingredients
    if (storage.isRottenEmpty()) {
        Subtitle("No ingredients have rotten.");
    } else {
        Subtitle(storage.rottenIngredientsBehavior(DELETE) + " have rotten, so were thrown away.");
    }

    // Briefing about Danger Ingredients
    if (storage.isDangerEmpty()) {
        Subtitle("No ingredients are in dangerous status.");
    } else {
        Subtitle(storage.dangerIngredientsBehavior(NONE) +
                 " are in dangerous status. Are you gonna dump them all, master?");

        int answer = SingleChoiceWithNumber({"Yes", "No"});
        switch (answer) {
            case 1: {
                Subtitle("Certainly! Do you want to refill them with fresh ones, master?");
                int answer2 = SingleChoiceWithNumber({"Yes", "No"});

                switch (answer2) {
                    case 1:
                        storage.dangerIngredientsBehavior(CLEAR);
                        break;
                    case 2:
                        storage.dangerIngredientsBehavior(DELETE);
                        break;
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }

}

// ------------------------------------------------------------------------------------------------------------
// Actual menu (eat? -> what else?)
// ------------------------------------------------------------------------------------------------------------
void RefriSystem::Act() {
    Title("Act");
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
            int answer2 = SingleChoiceWithNumber({"Open Storage", "Manage Recipe", "Exit Program"});

            switch (answer2) {
                case 1: {
                    ManageStorage();
                    break;
                }
                case 2: {
                    ManageRecipe();
                    break;
                }
                case 3: {
                    Title("Exit Program");
                    Subtitle("Bye, master!");
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

// ------------------------------------------------------------------------------------------------------------
// STORAGE
// ------------------------------------------------------------------------------------------------------------
void RefriSystem::ManageStorage() {
    Title("Manage Storage");
    storage.printStorage();

    int answer1 = SingleChoiceWithNumber({"Add Ingredient", "Delete Ingredient", "Back to Menu"});
    switch (answer1) {
        case 1: {
            storage.addIngredientSequence();
            ManageStorage();
            break;
        }
        case 2: {
            storage.removeIngredientSequence();
            ManageStorage();
            break;
        }
        case 3: {
            Act();
            break;
        }
        default:
            break;
    }
}

// ------------------------------------------------------------------------------------------------------------
// RECIPE
// ------------------------------------------------------------------------------------------------------------
void RefriSystem::ManageRecipe() {
    Title("Manage Recipe");
    recipe.showAllRecipe();

    int answer1 = SingleChoiceWithNumber({"Add Recipe", "Delete Recipe", "Inspect Recipe", "Back to Menu"});
    switch (answer1) {
        case 1: {
            recipe.addRecipeSequence();
            ManageRecipe();
            break;
        }
        case 2: {
            recipe.removeRecipeSequence();
            ManageRecipe();
            break;
        }
        case 3: {
            recipe.inspectRecipeSequence();
            ManageRecipe();
            break;
        }
        case 4: {
            Act();
            break;
        }
        default:
            break;
    }
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

    Cook(mood);
}

void RefriSystem::Cook(string mood) {

    cout << endl;
    TextColor(DARKGRAY, BLACK);
    cout << "LODADING";
    TextColor(WHITE, BLACK);
    cout << endl;

    // Recommend recipes according to User's mood
    vector<RecipeItem> recipePriorityList = recipe.recommendRecipe(mood);
    while (!recipePriorityList.empty()) {
        // Extract first 3 recipes
        vector<RecipeItem> extractedRecipes;
        for (int i = 0; i < 3 && !recipePriorityList.empty(); ++i) {
            extractedRecipes.push_back(recipePriorityList.front());
            recipePriorityList.erase(recipePriorityList.begin());
        }

        // Output detailed info of first 3 recipes
        Subtitle("Then, I recommend these foods. ");
        Subtitle("Here is the detailed info of TOP 3 recommended recipes");
        recipe.showPartialRecipe(extractedRecipes);

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

        RecipeItem selectedRecipe = extractedRecipes[answer_recipeChosen - 1];
        // Whether selectedRecipe is makable, Ask Cook OR Buy
        bool selectedIsMakable = recipe.checkMakable(selectedRecipe);
        if (selectedIsMakable) {
            Subtitle(selectedRecipe.name + " is chosen, We can make this food with the ingredients in the storage! ");
            Subtitle("Would you like to cook now, master? ");
        } else {
            Subtitle(selectedRecipe.name + " is chosen, But we need more of the following ingredients: ");
            vector<IngredientDetail> lackIngredientList = recipe.checkLackIngredients(selectedRecipe);

            cout << endl;
            cout << "=================== Check Lack Ingredient ===================" << endl;
            for (size_t i = 0; i < lackIngredientList.size(); ++i) {
                cout << lackIngredientList[i].name << " * " << lackIngredientList[i].amount << endl;
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
                    TextColor(BLUE, BLACK);
                    cout << "Refri is cooking ... (" << i << "s)";
                    TextColor(WHITE, BLACK);
                    cout << endl;
                    this_thread::sleep_for(chrono::seconds(1));
                }

                // [Cook] selected food -> update(reduce quantity) storage.ingredientData
                storage.removeIngredientsInRecipe(selectedRecipe, selectedIsMakable);

                // UI : display Succeed Cook message
                TextColor(BLACK, CYAN);
                cout << selectedRecipe.name << " is ready!";
                TextColor(WHITE, BLACK);
                cout << endl;
                cout << endl;
                cout << "       ________  .====\n"
                        "      [________>< :===\n"
                        "                 '====\n"
                        "       ________ ___,,,,,,,\n"
                        "      [________>__________\\\n"
                        "\n"
                        "       ________   .==.\n"
                        "      [________>c((_  )\n"
                        "                  '=='" << endl;

                cout << endl;
                TextColor(BLACK, YELLOW);
                cout << "Enjoy your meal, master!";
                TextColor(WHITE, BLACK);
                cout << endl;
                Act();
                break;
            case 2:
                // (2) No, I wanna go back to the food list.
                Cook(mood);
                break;
            case 3:
                // (3) No, I wanna go back to home.
                Act();
                break;
        }
        break;
    }
};
