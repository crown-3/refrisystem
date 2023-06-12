#include <iostream>
#include <string>
#include <typeinfo>
#include "ColorfulCli.h"
#include <chrono>
#include <thread>


#define LENGTH 50
using namespace std;

void Error(const string error);

void Title(const string title) {
    if (title.length() > LENGTH - 4) { // considering two spaces and two '#' characters on each side
        Error("Title length is too long!");
    } else {
        int side_length = (LENGTH - title.length() - 4) / 2;
        string side(side_length, '#');

        cout << endl;

        cout << string(LENGTH, '#') << endl;
        cout << "#" << side << " ";

        TextColor(GREEN,BLACK);
        cout << title;
        TextColor(WHITE,BLACK);

        cout << " " << side;
        if((LENGTH - title.length()) % 2 != 0) cout << "#"; // if the total length is not even, we add one more '#' to make it even
        cout << "#" << endl;
        cout << string(LENGTH, '#') << endl;
        cout << endl;
    }
}


void Subtitle(const string subtitle) {
    cout << endl;
    cout << "### ";
    TextColor(LIGHTCYAN, BLACK);

    for (char c : subtitle) {
        cout << c;
        cout.flush();  // clear buffer to make the text appear immediately
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    cout << endl;
    TextColor(WHITE, BLACK);
}

void Error(const string error) {
    cout << endl;
    TextColor(BLACK,RED);
    cerr << "!!! <ERR> " << error << " !!!" << endl;
    TextColor(WHITE, BLACK);
}

