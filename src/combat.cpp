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

void combatOccur(Player mainChar, Enemy enemy) {
    Enemy enemy_t = enemy; // temporary variable for the enemy
    bool isDefeated = false;
    bool yourTurn = true;
    Item strongestWpn = mainChar.strongestWeapon(mainChar.inventory);
    while (!isDefeated) {
        clearWnd();
        if (enemy_t.name == (string)"Goblin") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                int attack_dmg_w_wpn = mainChar.strStat + strongestWpn.damage; // base str divided by 2 and add player's strongest weapon damage stat to attack.
                int attack_hand = mainChar.strStat;
                if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                    choiceDialog("Goblin", "No like humans. Humans bad.", { "Attack with " + strongestWpn.name, "Attack with your hands" }, 20, 40, 0);
                    int attack_choice;
                    printf("> ");
                    cin >> attack_choice;
                    switch (attack_choice) {
                    case 1:
                        // attack with weapon
                        enemy_t.HPstat -= attack_dmg_w_wpn;
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name
                                + " with a " + strongestWpn.name
                                + " and dealt " + to_string(attack_dmg_w_wpn) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            isDefeated = true;
                        }
                        break;
                    case 2:
                        enemy_t.HPstat -= attack_hand;
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_hand) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            isDefeated = true;
                        }
                        break;
                    default:
                        enemy_t.HPstat -= attack_dmg_w_wpn;
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name
                                + " with a " + strongestWpn.name
                                + " and dealt " + to_string(attack_dmg_w_wpn) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            isDefeated = true;
                        }
                        break;
                    }
                }
                else {
                    choiceDialog("Goblin", "Munch crunch tasty bones! Humans weak.", { "Attack with your hands" }, 20, 40, 0);
                    int attack_choice;
                    printf("> ");
                    cin >> attack_choice;
                    switch (attack_choice) {
                    case 1:
                        enemy_t.HPstat -= attack_hand;
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_hand) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            isDefeated = true;
                        }
                        // attack with weapon
                        break;
                    default:
                        enemy_t.HPstat -= attack_hand;
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name
                                + " with a " + strongestWpn.name
                                + " and dealt " + to_string(attack_hand) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            isDefeated = true;
                        }
                        // attack with weapon
                        break;
                    }
                }
            }
            else {
                yourTurn = true;
                // goblin attacks player, expected for goblins not to have any weapons.
                int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                mainChar.HPStat -= attack_hand;
                if (mainChar.HPStat > 0) {
                    typeText("The " + enemy_t.name + " dealt " + to_string(attack_hand) + " damage to you!", 30, 2000, 0);
                }
                else {
                    isDefeated = true;
                    typeText("You died.", 30, 2000, 0);
                    //deathOccur();
                }
            }
        }
    }
}