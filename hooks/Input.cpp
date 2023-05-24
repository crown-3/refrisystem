#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "ColorfulCli.h"
using namespace std;

string Input(string question, string example = "") {
    string value;
    bool firstAttempt = true;
    while (value.length() <= 0) {
        TextColor(YELLOW, BLACK);
        cout << question;
        TextColor(DARKGRAY, BLACK);
        cout << " " << example << ": ";
        TextColor(WHITE, BLACK);

        getline(cin, value);
        if (value.length() <= 0 && !firstAttempt) {
            cout << "Please Enter Something!" << endl;
        }
        firstAttempt = false;
    }
    return value;
}

vector<string> MultipleChoice(vector<string> choices, string question, string example = "") {
    string input;
    vector<string> selectedTags;

    while (true) {
        input = Input(question, example);
        selectedTags.clear();

        stringstream ss(input);
        string tag;
        bool isValidInput = true;
        vector<string> invalidTags;

        while(getline(ss, tag, ',')) {
            tag.erase(tag.begin(), find_if(tag.begin(), tag.end(), [](unsigned char ch) {
                return !isspace(ch);
            }));
            tag.erase(find_if(tag.rbegin(), tag.rend(), [](unsigned char ch) {
                return !isspace(ch);
            }).base(), tag.end());

            if(find(choices.begin(), choices.end(), tag) != choices.end()) {
                selectedTags.push_back(tag);
            } else {
                isValidInput = false;
                invalidTags.push_back(tag);
            }
        }

        if (isValidInput) {
            break;
        } else {
            cout << "'";
            for (size_t i = 0; i < invalidTags.size(); ++i) {
                cout << invalidTags[i];
                if (i != invalidTags.size() - 1) {
                    cout << "', '";
                }
            }
            cout << "' is not in the list. Please try again.\n";
        }
    }

    return selectedTags;
}