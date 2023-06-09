#ifndef RECIPERECOMMENDATION_H
#define RECIPERECOMMENDATION_H

#include "Recommendation.h"
#include "Interfaces.h"

class RecipeRecommendation : public Recommendation<RecipeItem, > {
public:
    virtual vector<RecipeItem> recommend(vector<RecipeItem> TargetList, State UserState, Criteria PriorityCriteria) override;

    /*
     * vector<RecipeItem> recommendRecipe(string mood);
     * bool checkMakable(RecipeItem recipe); // helper function
     * vector<IngredientDetail> checkLackIngredient(RecipeItem recipe); // helper function
     */
};


#endif //RECIPERECOMMENDATION_H
