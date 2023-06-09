//
// Created by strik on 2023-06-09.
//

#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

class Recommendation {

public:
    virtual vector<Row> recommend()=0;

    /*
     * vector<RecipeRow> recommendRecipe(string mood);
     * bool checkMakable(RecipeRow recipe); // helper function
     * vector<IngredientDetail> checkLackIngredient(RecipeRow recipe); // helper function
     */

};

#endif //RECOMMENDATION_H
