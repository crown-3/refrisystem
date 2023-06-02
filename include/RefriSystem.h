#ifndef REFRISYSTEM_REFRISYSTEM_H
#define REFRISYSTEM_REFRISYSTEM_H

#include "RefriStorage.h"
#include "RefriRecipe.h"

class RefriSystem{
    int select; // 메뉴 선택 상태 (1~5)
    bool flag = true; // 상세 페이지 IN 또는 OUT
    // bool is_ON = true; // 냉장고 ON 또는 OFF

    RefriStorage refriStorage;
    RefriRecipe refriRecipe;

public:
    static void MAIN_MENU();
    static void Introduction();
    void Briefing();
    static void Act();
};

#endif //REFRISYSTEM_REFRISYSTEM_H
