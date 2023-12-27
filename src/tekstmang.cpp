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

using namespace std;
// Ideed
// Save/load progress
// vastased


// init
string mc_name;

struct Player {
    // Basic info
    string name;
    string className; 
    string race; // useless

    // Stats
    int level; // ...
    int HPStat; // healthpoints
    int intStat; // inteligence, mana, magic
    int strStat; // strength
    int agiStat; // speed

    // other
    int gold; // Amount of money the main character has.
    vector<string> inventory; // list of items, inventory.

    Player(string playerName, string Class, string race, int level, int HPStat, int intStat, int strStat, int agiStat, int gold, vector<string> inventory)
        : name(playerName), className(Class), race(race), level(level), HPStat(HPStat), intStat(intStat), strStat(strStat), agiStat(agiStat), gold(gold), inventory(inventory) {}
};

struct sideChar {
    // basic info
    string name;
    int age;
    int relationship; // Relationship with the main character. 1-10
    int gold; // Amount of money this character has.
};

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

int randomnumber() {
	int x = time(0);
	srand(x);
    return x;
}

// Just types text with a customizable delay.
void typeText(string text, int delay = 65, int endDelay = 50, int startDelay = 0) {
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
void choiceDialog(string talker, string text, vector<string> choices, int delay = 65, int endDelay = 50, int startDelay = 0) {
    this_thread::sleep_for(chrono::milliseconds(startDelay));
    cout << "" << talker << ": " << std::flush;
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
void typeDialog(string talker, string text, int delay = 65, int endDelay = 50, int startDelay = 0) {
    this_thread::sleep_for(chrono::milliseconds(startDelay));
    cout << "" << talker << ": " << std::flush;
    for (char c : text) {
        cout << c << std::flush;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    this_thread::sleep_for(chrono::milliseconds(endDelay));
    cout << endl;
}

// Initialization of sidecharacters to be used later on.
sideChar mother = {"Mom", 45, 9, 0};
sideChar friend1 = {"John", 13 , 5, 0};
sideChar friend2 = {"William", 15, 7, 0};
sideChar OldJack = {"Old Jack", 100 , 1, 0};
sideChar YoungJack = {"Young Jack", 100 , 1, 0};

// Initialization of functions to be used later on.
void hommik_1(Player mainChar); // Morning, waking up, meeting friends.
void caves_1_choice(Player mainChar); // Going into the cave. Exploring the cave.

void startGame() { // algus
    clearWnd();
    // Ask the user for their name.
    printf("Enter your name: ");
    cin >> mc_name;
    Player mainChar = Player(mc_name, "Adventurer", "Human", 0, 100, 0, 12, 8, 0, {});
    clearWnd();
    typeText("You start waking up from a deep sleep, your throat parched and body heavy.", 50, 25, 1000);
    typeText("The air is thick with unfamiliar stillness, and as you open your eyes,", 50, 25, 100);
    typeText("Your gaze follows the faint glow that beckons from the end of a narrow tunnel", 50, 25, 100);
    typeText("A surge of fear courses through you, and you start thinking about the past you have been through.", 50, 1000, 100);
    hommik_1(mainChar);
}

int main()
{
    resizeConsole(550, 800);
    clearWnd();
    makePage("Welcome to this fantastic RPG, without a name. Enjoy!", {"Load Game", "New Game", "Settings", "About", "Quit Game"});
    int x;
    printf("> ");
    cin >> x;
    switch (x) {
        case 1:
            // Load an already existing save.
            printf("Nothing here");
            break;
        case 2:
            // Make a new save
            startGame();
            break;
        case 3:
            // Settings, currently empty
            // change text speed etc.
            clearWnd();
            main();
            break;
        case 4:
            // The about section
            clearWnd();
            printf("Made by Atlaz and Alabanik\n");
            system("pause");
            clearWnd();
            main();
            break;
        case 5:
            // Quit the game.
            exit(0);
            break;
        default:
            // If the case isnt listed.
            printf("Please enter valid number");
            system("pause");
            main();
            break;
    }
    // continue..
    return 0;
}

void hommik_1(Player mainChar){
     clearWnd();
     typeDialog("Mum", "Wake up! Breakfast is ready.", 65, 50, 0);
     typeDialog(mainChar.name, "Just one more minute, please.", 65, 50, 0);
     typeDialog("Mum", "Okay, but don’t forget your friends are waiting for you outside in 30 minutes. ", 65, 100, 0);
     typeDialog(mainChar.name, "Whatever.", 65, 1000, 0);
     typeText("Friends arrive", 30, 1000, 2000);
     typeDialog(friend1.name, "Well look who woke up.", 65, 50, 0);
     typeDialog(mainChar.name, "Shut it.", 65, 50, 0);
     typeDialog(friend2.name, "Chill, why so cranky.", 65, 50, 0);
     typeDialog(mainChar.name, "Sorry, I just had a bad sleep. Let’s go, check out the cave.");
     typeDialog(mainChar.name, "(looking at the cave entrance) Wow! So dark inside. Do you think it’s a good idea?");
     typeDialog(friend1.name, "(Making a terrified face) Are you scared little boy?");
     typeDialog(friend2.name, "He is definitely thinking about running back to his mum.");
     typeDialog("Friends", "(Laughing)");
     typeDialog(mainChar.name, "Ha Ha so funny. Come on, whoever is last has to kiss Jessica.");
     typeText("They all enter the cave together..", 65, 50, 600);
     caves_1_choice(mainChar);
}

void caves_1_choice(Player mainChar){
    typeDialog(friend2.name, "It’s so wet here.");
    typeDialog(friend1.name, "I can smell something nasty, I think it is dead corps..");
    typeDialog(mainChar.name, "Take the torch and light it.");
    typeText("The three of them proceed to light up the torches and find an old chest with spider webs all over it.", 65, 50, 600);
    typeDialog(mainChar.name, "Look! A chest.");
    typeDialog(friend2.name, "Some poor old man has probably forgotten it here and is not coming back for it.");
    typeDialog(friend1.name, "(Sneaking up them and with a distorted voice says) Don’t you dare to open my secret or thousand different illnesses shall be upon you.");
    typeDialog(mainChar.name, "(visibly angry) WHAT IS WRONG WITH YOU!");
    typeDialog(friend2.name, "That’s too far man, don’t do it.");
    typeDialog(friend1.name, "Hah, got you guys. What are we looking at?");
    typeDialog(mainChar.name, "(Still little angry) Some old crate. Shall we open it?");
    typeDialog(friend1.name, "Definitely!");
    choiceDialog(mainChar.name, "(Should I open this chest?)", { "What is the worst that can happen?", "I dont think this will be a good idea." });
    int choice_1;
    printf("> ");
    cin >> choice_1;
    switch (choice_1) {
        case 1:
            // opens chest
            break;
        case 2:
            // decides not to open.
            typeDialog(friend1.name, "Yeah, we will open this chest.");
            break;
    }
    // continue
}
