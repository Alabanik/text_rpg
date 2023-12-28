#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <random>

using namespace std;

// Just types text with a customizable delay.
void typeText(string text, int delay = 45, int endDelay = 50, int startDelay = 0) {
    this_thread::sleep_for(chrono::milliseconds(startDelay));
    for (char c : text) {
        cout << c << std::flush;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    this_thread::sleep_for(chrono::milliseconds(endDelay));
    cout << endl;
}

// function which prints out dialog with a delay.
// Gives the user choices on what they can answer with.
// Output (e.g): "Mom": "Did you do the dishes yet?"
// Multiple choices from 1 to infinite. Put the answers inside a choices vector (e.g: {"Yes", "No"})
void choiceDialog(string talker, string text, vector<string> choices, int delay = 45, int endDelay = 50, int startDelay = 0) {
    this_thread::sleep_for(chrono::milliseconds(startDelay));
    cout << "\n" << talker << ": " << std::flush;
    for (char c : text) {
        cout << c << std::flush;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    this_thread::sleep_for(chrono::milliseconds(endDelay));
    cout << "\n";
    for (size_t i = 0; i < choices.size(); ++i) {
        cout << "> " << to_string(i + 1) << ". " << choices[i] << "\n";
    }
    cout << endl;
}

// funtion to type out dialog with a specified delay to make it look more like a dialog.
// Output (e.g): "Mom": "Wake up! Breakfast is ready."
void typeDialog(string talker, string text, int delay = 45, int endDelay = 50, int startDelay = 0) {
    this_thread::sleep_for(chrono::milliseconds(startDelay));
    cout << "\n" << talker << ": " << std::flush;
    for (char c : text) {
        cout << c << std::flush;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    this_thread::sleep_for(chrono::milliseconds(endDelay));
    cout << endl;
}

void makePage(string topMessage, vector<string> bottomMessages) {
    cout << "> " << (topMessage) << "\n\n";
    printf("+---------------------------------------------------------+\n");
    for (size_t i = 0; i < bottomMessages.size(); ++i) {
        cout << " " << to_string(i + 1) << ". " << bottomMessages[i] << "\n";
    }
    printf("+---------------------------------------------------------+\n");
}

void resizeConsole(int width, int height) { // function resizes the window to the specified width and height.
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void clearWnd() { // function which clears the window.
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int getRandom(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gen);
}

bool shouldCall(int prob) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 100);
    return distribution(gen) <= prob;
}