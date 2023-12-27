#include <iostream>
#include <cstdio>
#include <cstdlib>

// the shit
#include "structs.h"
#include "utils.h"
#include "dialog.h"

using namespace std;

// Ideed
// Save/load progress
// vastased

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
