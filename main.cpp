#include "include/RefriSystem.h"
#include "utils/Title.h"

using namespace std;

int main(void){
    try {
        RefriSystem refri;
        refri.MAIN_MENU();
    } catch (const std::exception &e) {
        Error(e.what());
    }

    return 0;
}
