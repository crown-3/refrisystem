#include <iostream>
#include <string>
#include <typeinfo>
#include "ColorfulCli.h"


#define LENGTH 50
using namespace std;

void Title(const string title) {
    if (title.length() > LENGTH - 4) { // considering two spaces and two '#' characters on each side
        cout << "!!! <ERR> Title length is too long! !!!" << endl;
    } else {
        int side_length = (LENGTH - title.length() - 4) / 2;
        string side(side_length, '#');

        cout << "\n";

        cout << string(LENGTH, '#') << endl;
        cout << "#" << side << " ";

        TextColor(GREEN,BLACK);
        cout << title;
        TextColor(WHITE,BLACK);

        cout << " " << side;
        if((LENGTH - title.length()) % 2 != 0) cout << "#"; // if the total length is not even, we add one more '#' to make it even
        cout << "#" << endl;
        cout << string(LENGTH, '#') << endl;
        cout << "\n";
    }
}

void Subtitle(const string subtitle) {
    cout << "\n";
    cout << "### ";
    TextColor(LIGHTCYAN, BLACK);
    cout << subtitle << endl;
    TextColor(WHITE, BLACK);
}