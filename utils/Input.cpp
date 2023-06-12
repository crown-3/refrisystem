#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>

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

int InputInteger(int minimum, int maximum, string question, string example = "") {
    string value;
    int integer;
    bool firstAttempt = true;
    while (true) {
        TextColor(YELLOW, BLACK);
        cout << question;
        TextColor(DARKGRAY, BLACK);
        cout << " " << example << ": ";
        TextColor(WHITE, BLACK);

        getline(cin, value);

        stringstream ss(value);
        if (ss >> integer && !(ss >> value)) { // Check if the input is integer
            if (integer >= minimum && integer <= maximum) { // Check if the input is in the range
                return integer;
            } else {
                cout << "Please Enter a Number between " << minimum << " and " << maximum << "!" << endl;
            }
        } else {
            if (!firstAttempt || value.length() > 0) {
                cout << "Please Enter a Valid Number!" << endl;
            }
        }
        firstAttempt = false;
    }
}

int SingleChoiceWithNumber(vector<string> choices) {
    // Print the choices
    for (int i = 0; i < choices.size(); ++i) {
        cout << "(" << i+1 << ") ";
        TextColor(YELLOW, BLACK);
        cout << choices[i] << " ";
        TextColor(WHITE, BLACK);
    }
    cout << endl;

    // Get the user's choice
    int answer;

    TextColor(DARKGRAY, BLACK);
    cout << "Answer : ";
    TextColor(WHITE, BLACK);
    cin >> answer;

    // Check if the answer is valid
    if(answer < 1 || answer > choices.size()){
        cout << "Invalid choice. Please enter a number between 1 and " << choices.size() << endl;
        return SingleChoiceWithNumber(choices); // ask again
    }

    return answer;
}

void PressEnterToContinue() {
    TextColor(DARKGRAY,BLACK);
    cout << "Press ENTER to continue...";
    cout.flush();
    TextColor(WHITE,BLACK);
    cin.get();
//    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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