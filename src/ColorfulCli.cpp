#include<windows.h>

void textcolor(int foreground, int background)
{
    int color=foreground+background*16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
