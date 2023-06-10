#ifndef REFRISYSTEM_RECIPEDATAMANAGEMENT_H
#define REFRISYSTEM_RECIPEDATAMANAGEMENT_H

#include "DataManagement.h"
#include "Interfaces.h"
#include "../utils/Table.h"
#include "Storage.h"
#include <string>
#include <vector>

using namespace std;

class RecipeDataManagement : public DataManagement {
private:
    string RawJSON_path;
    vector<RecipeItem> data;
    Storage* storage;
public:
    RecipeDataManagement(string data_path);
    ~RecipeDataManagement();

    void loadData() override;
    void saveData() override;

    vector<RecipeItem> getData();
    void addData(RecipeItem recipe);
    void removeData(string recipeName);
};


#endif //REFRISYSTEM_RECIPEDATAMANAGEMENT_H
