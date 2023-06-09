#ifndef RECIPERECOMMENDATION_H
#define RECIPERECOMMENDATION_H

// RecipeRecommendation class
class RecipeRecommendation {
public:
    vector<RecipeRow> recommendRecipe(string mood);
    bool checkMakable(RecipeRow recipe); // helper function

    vector<IngredientDetail> checkLackIngredient(RecipeRow recipe); // helper function
};


#endif //RECIPERECOMMENDATION_H
