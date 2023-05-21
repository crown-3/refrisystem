#include <iostream>
#include "../include/RefriSystem.h"

using namespace std;

void RefriSystem::MAIN_MENU() {
    while(true) {
        cout << "################ MAIN MENU ################" << endl;
        cout << "1. Refri Storage" << endl;
        cout << "2. Refri Recipe" << endl;
        cout << "3. Refri ShoppingList" << endl;
        cout << "4. Refri ShoppingCart" << endl;
        cout << "5. ※※※ OFF SmartRefri ※※※" << endl;
        cout << "###########################################" << endl;

        cout << "|  SELECT  :  ";
        cin >> select;

        switch (select) {
            case 1:
                cout << "1. Refri Storage  -------- 아직 구현되지 않음" << endl;
//                flag = true;
//                while (flag) {
//                    // refriStorage.MAIN_RefriStorage();
//                }
                break;
            case 2:
                cout << "2. Refri Recipe  -------- 아직 구현되지 않음" << endl;
//                flag = true;
//                while (flag) {
//                    // refriRecipe.MAIN_RefriRecipe();
//                }
                break;
            case 3:
                cout << "3. Refri ShoppingList  -------- 아직 구현되지 않음" << endl;
//                flag = true;
//                while (flag) {
//                    //
//                }
                break;
            case 4:
                cout << "4. Refri ShoppingCart  -------- 아직 구현되지 않음" << endl;
//                flag = true;
//                while (flag) {
//                    //
//                }
                break;
            case 5:
                cout << "※※※※※※ BYE USER ※※※※※※" << endl;
                exit(0);
            default:
                // 잘못 입력 에러 메시지
                cout << "Wrong Select" << endl; // 나중에 고치기
        }
    }
}