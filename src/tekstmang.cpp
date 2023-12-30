#include <iostream>
#include <cstdio>
#include <cstdlib>

// the shit
#include "structs.h"
#include "utils.h"
#include "dialog.h"

using namespace std;

// Ideed
// 
// vastased tehtud

int main()
{
    resizeConsole(1200, 600);
    clearWnd();
    makePage("Welcome to this fantastic RPG, without a name. Enjoy!", {"Play", "About", "Quit Game"});
    int x;
    printf("> ");
    cin >> x;
    switch (x) {
        case 1:
            startGame(false);
            break;
        case 2:
            // The about section
            clearWnd();
            printf("Made by Atlaz and Alabanik\n");
			printf("3 am, 3 days, 2 guys\n");
            system("pause");
            clearWnd();
            main();
            break;
        default:
            // Quit the game.
            exit(0);
            break;
    }
    // continue..
    return 0;
}
