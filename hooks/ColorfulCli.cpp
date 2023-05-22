#include<windows.h>

void TextColor(int foreground, int background)
{
    int color=foreground+background*16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
