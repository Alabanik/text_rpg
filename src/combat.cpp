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
#include "structs.h"
#include "utils.h"
#include "dialog.h"

using namespace std;

// Combat features
// Added sword(with strongest one) / hand attack
// Added blocking attack.
// 
// TODO: Critical, block chance


// Enemies
// Goblin (35), Juut (129) 

void combatOccur(Player& mainChar, Enemy enemy) {
    Enemy enemy_t = enemy; // temporary variable for the enemy
    bool isDefeated = false;
    bool yourTurn = true;
    bool blockNext = false;
    Item strongestWpn = mainChar.strongestWeapon(mainChar.inventory);
    while (!isDefeated) {
        clearWnd();
        if (enemy_t.name == (string)"Goblin") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                        choiceDialog("Goblin", "No like humans. Humans bad.", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice)
                        {
                        case 1:
                            // attack with weapon
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name
                                    + " with a " + strongestWpn.name
                                    + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        case 2:
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                    else {
                        choiceDialog("Goblin", "Munch crunch tasty bones! Humans weak.", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            enemy_t.HPstat -= attack_dmg;
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            // attack with weapon
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                }
                else {
                    yourTurn = true;
                    // goblin attacks player, expected for goblins not to have any weapons.
                    int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                    if (blockNext) {
                        mainChar.HPStat -= attack_hand;
                        if (mainChar.HPStat > 0) {
                            typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                        }
                        else {
                            isDefeated = true;
                            deathOccur();
                        }
                    }
                    else {
                        typeText("You blocked the " + enemy_t.name + " attack.", 30, 2000, 0);
                        blockNext = true;
                    }
                }
            }
        }
        else if (enemy_t.name == (string)"Old Goblin") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                }
                if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                    choiceDialog("Old Goblin", "You cant beat me with that fragile body of yours. Muhaha", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
                    int attack_choice;
                    printf("> ");
                    cin >> attack_choice;
                    switch (attack_choice) {
                    case 1:
                        // attack with weapon
                        enemy_t.HPstat -= attack_dmg;
                        if (crit) {
                            typeText("You got a critical hit!", 30, 100, 0);
                        }
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name
                                + " with a " + strongestWpn.name
                                + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            for (Quest &quest : mainChar.quests) {
                                if (quest.name == (string)"Old Goblin") {
                                    quest.progress = "WAITING_FOR_REWARD";
                                    break;
                                }
                            }
                            isDefeated = true;
                        }
                        break;
                    case 2:
                        enemy_t.HPstat -= attack_dmg;
                        if (crit) {
                            typeText("You got a critical hit!", 30, 100, 0);
                        }
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Old Goblin") {
                                    quest.progress = "WAITING_FOR_REWARD";
                                    break;
                                }
                            }
                            isDefeated = true;
                        }
                        break;
                    default:
                        blockNext = true;
                        break;
                    }
                }
                else {
                    choiceDialog("Old Goblin", "You have no chance winning against me.", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
                    int attack_choice;
                    printf("> ");
                    cin >> attack_choice;
                    switch (attack_choice) {
                    case 1:
                        enemy_t.HPstat -= attack_dmg;
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Old Goblin") {
                                    quest.progress = "WAITING_FOR_REWARD";
                                    break;
                                }
                            }
                            isDefeated = true;
                        }
                        // attack with weapon
                        break;
                    default:
                        blockNext = true;
                        break;
                    }
                }
            }
            else {
                yourTurn = true;
                // goblin attacks player, expected for goblins not to have any weapons.
                int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                if (!blockNext) {
                    mainChar.HPStat -= attack_hand;
                    if (mainChar.HPStat > 0) {
                        typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                    }
                    else {
                        isDefeated = true;
                        deathOccur();
                    }
                }
                else {
                    typeText("You blocked the " + enemy_t.name + " attack.", 30, 2000, 0);
                    blockNext = false;
                }
            }
        }
        else if (enemy_t.name == (string)"Juut") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                if (crit) {
                    attack_dmg = attack_dmg * 2;

                    if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                        choiceDialog("Jew", "I think all jews deserve happiness.", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            // attack with weapon
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name
                                    + " with a " + strongestWpn.name
                                    + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        case 2:
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                    else {
                        choiceDialog("Jew", "I think im the problem.", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            enemy_t.HPstat -= attack_dmg;
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            // attack with weapon
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                }
                else {
                    yourTurn = true;
                    // goblin attacks player, expected for goblins not to have any weapons.
                    int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                    if (blockNext) {
                        mainChar.HPStat -= attack_hand;
                        if (mainChar.HPStat > 0) {
                            typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                        }
                        else {
                            isDefeated = true;
                            deathOccur();
                        }
                    }
                    else {
                        typeText("You blocked the " + enemy_t.name + " attack.", 30, 2000, 0);
                        blockNext = true;
                    }
                }
            }
        }
        else if (enemy_t.name == (string)"Ghost") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                if (crit) {
                    attack_dmg = attack_dmg * 2;

                    if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                        choiceDialog("Ghost", "...", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            // attack with weapon
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name
                                    + " with a " + strongestWpn.name
                                    + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        case 2:
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                    else {
                        choiceDialog("Ghost", ".....", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            enemy_t.HPstat -= attack_dmg;
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            // attack with weapon
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                }
                else {
                    yourTurn = true;
                    // goblin attacks player, expected for goblins not to have any weapons.
                    int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                    if (blockNext) {
                        mainChar.HPStat -= attack_hand;
                        if (mainChar.HPStat > 0) {
                            typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                        }
                        else {
                            isDefeated = true;
                            deathOccur();
                        }
                    }
                    else {
                        typeText("You blocked the " + enemy_t.name + " attack.", 30, 2000, 0);
                        blockNext = true;
                    }
                }
            }
        }
        else if (enemy_t.name == (string)"Ghost") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                if (crit) {
                    attack_dmg = attack_dmg * 2;

                    if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                        choiceDialog("Ghost", "...", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            // attack with weapon
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name
                                    + " with a " + strongestWpn.name
                                    + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        case 2:
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                    else {
                        choiceDialog("Ghost", ".....", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            enemy_t.HPstat -= attack_dmg;
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            // attack with weapon
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                }
                else {
                    yourTurn = true;
                    // goblin attacks player, expected for goblins not to have any weapons.
                    int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                    if (blockNext) {
                        mainChar.HPStat -= attack_hand;
                        if (mainChar.HPStat > 0) {
                            typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                        }
                        else {
                            isDefeated = true;
                            deathOccur();
                        }
                    }
                    else {
                        typeText("You blocked the " + enemy_t.name + " attack.", 30, 2000, 0);
                        blockNext = true;
                    }
                }
            }
        }
        else if (enemy_t.name == (string)"Salamander") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                if (crit) {
                    attack_dmg = attack_dmg * 2;

                    if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                        choiceDialog("Salamander", "Im the strongest", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            // attack with weapon
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name
                                    + " with a " + strongestWpn.name
                                    + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        case 2:
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                    else {
                        choiceDialog("Salamander", "Are you trying to disrespect me?", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            enemy_t.HPstat -= attack_dmg;
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            // attack with weapon
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                }
                else {
                    yourTurn = true;
                    // goblin attacks player, expected for goblins not to have any weapons.
                    int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                    if (blockNext) {
                        mainChar.HPStat -= attack_hand;
                        if (mainChar.HPStat > 0) {
                            typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                        }
                        else {
                            isDefeated = true;
                            deathOccur();
                        }
                    }
                    else {
                        typeText("You blocked the " + enemy_t.name + " attack.", 30, 2000, 0);
                        blockNext = true;
                    }
                }
            }
        }
        else if (enemy_t.name == (string)"Orphan Slaughterer") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                if (crit) {
                    attack_dmg = attack_dmg * 2;

                    if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                        choiceDialog("Orphan Slaughterer", "I hate everyone equally, especially orphans.", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            // attack with weapon
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name
                                    + " with a " + strongestWpn.name
                                    + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        case 2:
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                    else {
                        choiceDialog("Orphan Slaughterer", "Are you a orphan?", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            enemy_t.HPstat -= attack_dmg;
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            // attack with weapon
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                }
                else {
                    yourTurn = true;
                    // goblin attacks player, expected for goblins not to have any weapons.
                    int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                    if (blockNext) {
                        mainChar.HPStat -= attack_hand;
                        if (mainChar.HPStat > 0) {
                            typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                        }
                        else {
                            isDefeated = true;
                            deathOccur();
                        }
                    }
                    else {
                        typeText("You blocked the " + enemy_t.name + " attack.", 30, 2000, 0);
                        blockNext = true;
                    }
                }
            }
        }
        else if (enemy_t.name == (string)"Young Jack") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                if (crit) {
                    attack_dmg = attack_dmg * 2;

                    if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                        choiceDialog("Young Jack", "Hello, im jack.", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            // attack with weapon
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name
                                    + " with a " + strongestWpn.name
                                    + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        case 2:
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                    else {
                        choiceDialog("Young Jack", "I hate people like you the most.", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            enemy_t.HPstat -= attack_dmg;
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            // attack with weapon
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                }
                else {
                    yourTurn = true;
                    // goblin attacks player, expected for goblins not to have any weapons.
                    int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                    if (blockNext) {
                        mainChar.HPStat -= attack_hand;
                        if (mainChar.HPStat > 0) {
                            typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                        }
                        else {
                            isDefeated = true;
                            deathOccur();
                        }
                    }
                    else {
                        typeText("You blocked the " + enemy_t.name + " attack.", 30, 2000, 0);
                        blockNext = true;
                    }
                }
            }
        }
        else if (enemy_t.name == (string)"Old Jack") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                if (crit) {
                    attack_dmg = attack_dmg * 2;

                    if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                        choiceDialog("Old Jack", "Who are you again?", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            // attack with weapon
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name
                                    + " with a " + strongestWpn.name
                                    + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        case 2:
                            enemy_t.HPstat -= attack_dmg;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                    else {
                        choiceDialog("Old Jack", "Im also known as an Ancient Dragon.", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
                        int attack_choice;
                        printf("> ");
                        cin >> attack_choice;
                        switch (attack_choice) {
                        case 1:
                            enemy_t.HPstat -= attack_dmg;
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                isDefeated = true;
                            }
                            // attack with weapon
                            break;
                        default:
                            blockNext = true;
                            break;
                        }
                    }
                }
                else {
                    yourTurn = true;
                    // goblin attacks player, expected for goblins not to have any weapons.
                    int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                    if (blockNext) {
                        mainChar.HPStat -= attack_hand;
                        if (mainChar.HPStat > 0) {
                            typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                        }
                        else {
                            isDefeated = true;
                            deathOccur();
                        }
                    }
                    else {
                        typeText("You blocked the " + enemy_t.name + " attack.", 30, 2000, 0);
                        blockNext = true;
                    }
                }
            }
        }
        else {
            typeText("A random enemy just passed by...", 30, 2000, 0);
            isDefeated = true;
        }
    }
}