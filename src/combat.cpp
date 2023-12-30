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
#include <set>
#include "structs.h"
#include "utils.h"
#include "dialog.h"

using namespace std;

int calcDefence(vector<Item>& inventory, int attack_hand) {
    int percDefence = 0;
    set<string> uniqueItems;
    for (auto& item : inventory) {
        if (uniqueItems.count(item.name) > 0) {
            continue;
        }
        if (item.isDefence) {
            percDefence += item.damage;
            uniqueItems.insert(item.name);
        }
    }
    int remainingAttack = attack_hand - (attack_hand * percDefence / 100);
    return max(1, remainingAttack);
}

int getHealthPotion(Player mainChar) {
    int amount = 0;
    for (auto& item : mainChar.inventory) {
        if (item.name == (string)"Big Health Potion") {
            amount += 1;
        }
    }
    return amount;
}

int getBigHealthPotion(Player mainChar) {
    int amount = 0;
    for (auto& item : mainChar.inventory) {
        if (item.name == (string)"Big Health Potion") {
            amount += 1;
        }
    }
    return amount;
}

void removeFromInventory(vector<Item>& inventory, string itemName) {
    auto it = std::find_if(inventory.begin(), inventory.end(),
        [&itemName](const Item& item) {
            return item.name == itemName;
        });
    if (it != inventory.end()) {
        inventory.erase(it);
    }
}


void combatOccur(Player& mainChar, Enemy enemy) {
    Enemy enemy_t = enemy; // temporary variable for the enemy
    bool isDefeated = false;
    bool yourTurn = true;
    bool blockNext = false;
    Item strongestWpn = mainChar.strongestWeapon(mainChar.inventory);
    while (!isDefeated) {
        clearWnd();
        int bighealthpotions = getBigHealthPotion(mainChar);
        int healthpotions = getHealthPotion(mainChar);
        if (enemy_t.name == (string)"Goblin") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                blockNext = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                int attack_dmg_hand = mainChar.strStat;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    attack_dmg_hand = attack_dmg_hand * 2;
                }
                if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                    choiceDialog("Goblin", "No like humans. Humans bad.", { "Attack with " + strongestWpn.name, "Attack with your hands", "Use 'Health Potion'",  "Block next attack"}, 20, 40, 0);
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
                            int reward = getRandom(1, 10);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            isDefeated = true;
                        }
                        break;
                    case 2:
                        enemy_t.HPstat -= attack_dmg_hand;
                        if (crit) {
                            typeText("You got a critical hit!", 30, 100, 0);
                        }
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg_hand) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            int reward = getRandom(1, 10);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            isDefeated = true;
                        }
                        break;
                    case 3:
                        if (healthpotions == 0) {
                            yourTurn = true;
                            typeText("You dont have any 'Health Potions'");
                        }
                        else {
                            removeFromInventory(mainChar.inventory, "Health Potion");
                            int rem_health = (100 - mainChar.HPStat);
                            if (rem_health >= 25) {
                                mainChar.HPStat += 25;
                            }
                            else {
                                mainChar.HPStat += rem_health;
                            }
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
                            int reward = getRandom(1, 10);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
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
                attack_hand = calcDefence(mainChar.inventory, attack_hand);
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
                    blockNext = true;
                }
            }
        }
        else if (enemy_t.name == (string)"Old Goblin") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                blockNext = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                int attack_dmg_hand = mainChar.strStat;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    attack_dmg_hand = attack_dmg_hand * 2;
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
                                for (Quest& quest : mainChar.quests) {
                                    if (quest.name == (string)"Old Goblin") {
                                        quest.progress = "WAITING_FOR_REWARD";
                                        break;
                                    }
                                }
                                int reward = getRandom(75, 100);
                                typeText("You received " + to_string(reward) + " gold.");
                                mainChar.gold += reward;
                                isDefeated = true;
                            }
                            break;
                        case 2:
                            enemy_t.HPstat -= attack_dmg_hand;
                            if (crit) {
                                typeText("You got a critical hit!", 30, 100, 0);
                            }
                            if (enemy_t.HPstat > 0) {
                                typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg_hand) + " damage!", 30, 1000, 0);
                            }
                            else {
                                typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                                for (Quest& quest : mainChar.quests) {
                                    if (quest.name == (string)"Old Goblin") {
                                        quest.progress = "WAITING_FOR_REWARD";
                                        break;
                                    }
                                }
                                int reward = getRandom(75, 100);
                                typeText("You received " + to_string(reward) + " gold.");
                                mainChar.gold += reward;
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
                                int reward = getRandom(75, 100);
                                typeText("You received " + to_string(reward) + " gold.");
                                mainChar.gold += reward;
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
                attack_hand = calcDefence(mainChar.inventory, attack_hand);
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
        else if (enemy_t.name == (string)"Ghost") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                blockNext = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                int attack_dmg_hand = mainChar.strStat;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    attack_dmg_hand = attack_dmg_hand * 2;
                }
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
                            int reward = getRandom(30, 50);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Welcome back dad!") {
                                    quest.progress = "COMPLETED";
                                    typeText("...", 300, 200, 0);
                                    typeDialog(mainChar.name, "NOOOOOO DAD!", 20, 100, 0);
                                    typeDialog("Dad", "It’s good son. Don’t worry I will be  in a better place, but take this and carry it with you.", 50, 300, 0);
                                    typeDialog(mainChar.name, "I  will dad !", 20, 2000, 0);
                                    typeText("(Dad passes away..)", 20, 1000, 0);
                                    break;
                                }
                            }
                            isDefeated = true;
                        }
                        break;
                    case 2:
                        enemy_t.HPstat -= attack_dmg_hand;
                        if (crit) {
                            typeText("You got a critical hit!", 30, 100, 0);
                        }
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg_hand) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            int reward = getRandom(30, 50);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Welcome back dad!") {
                                    quest.progress = "COMPLETED";
                                    typeText("...", 300, 200, 0);
                                    typeDialog(mainChar.name, "NOOOOOO DAD!", 20, 100, 0);
                                    typeDialog("Dad", "It’s good son. Don’t worry I will be  in a better place, but take this and carry it with you.", 50, 300, 0);
                                    typeDialog(mainChar.name, "I  will dad !", 20, 2000, 0);
                                    typeText("(Dad passes away..)", 20, 1000, 0);
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
                    choiceDialog("Ghost", "-- ..... --", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
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
                            int reward = getRandom(30, 50);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Welcome back dad!") {
                                    quest.progress = "COMPLETED";
                                    typeText("...", 300, 200, 0);
                                    typeDialog(mainChar.name, "NOOOOOO DAD!", 20, 100, 0);
                                    typeDialog("Dad", "It’s good son. Don’t worry I will be  in a better place, but take this and carry it with you.", 50, 300, 0);
                                    typeDialog(mainChar.name, "I  will dad !", 20, 2000, 0);
                                    typeText("(Dad passes away..)", 20, 1000, 0);
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
                int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                attack_hand = calcDefence(mainChar.inventory, attack_hand);
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
                blockNext = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                int attack_dmg_hand = mainChar.strStat;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    attack_dmg_hand = attack_dmg_hand * 2;
                }
                if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                    choiceDialog("Juut", "I earned it honestly", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
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
                            int reward = getRandom(30, 50);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Lost jewelry") {
                                    quest.progress = "COMPLETED";
                                    break;
                                }
                            }
                            isDefeated = true;
                        }
                        break;
                    case 2:
                        enemy_t.HPstat -= attack_dmg_hand;
                        if (crit) {
                            typeText("You got a critical hit!", 30, 100, 0);
                        }
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg_hand) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            int reward = getRandom(30, 50);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Lost jewelry") {
                                    quest.progress = "COMPLETED";
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
                    choiceDialog("Juut", "I earned it honestly", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
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
                            int reward = getRandom(30, 50);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Lost jewelry") {
                                    quest.progress = "COMPLETED";
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
                int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                attack_hand = calcDefence(mainChar.inventory, attack_hand);
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
        else if (enemy_t.name == (string)"Spider") {
            typeText("COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                blockNext = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                int attack_dmg_hand = mainChar.strStat;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    attack_dmg_hand = attack_dmg_hand * 2;
                }
                if (strongestWpn.damage != 0) {// if the player even has a weapon to attack with
                    choiceDialog("Spider", "Prepare to be entangled in my silky threads of doom!", { "Attack with " + strongestWpn.name, "Attack with your hands", "Block next attack" }, 20, 40, 0);
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
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Last escape?") {
                                    quest.progress = "COMPLETED";
                                    break;
                                }
                            }
                            int reward = getRandom(20, 30);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            isDefeated = true;
                        }
                        break;
                    case 2:
                        enemy_t.HPstat -= attack_dmg_hand;
                        if (crit) {
                            typeText("You got a critical hit!", 30, 100, 0);
                        }
                        if (enemy_t.HPstat > 0) {
                            typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg_hand) + " damage!", 30, 1000, 0);
                        }
                        else {
                            typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                            for (Quest& quest : mainChar.quests) {
                                if (quest.name == (string)"Last escape?") {
                                    quest.progress = "COMPLETED";
                                    break;
                                }
                            }
                            int reward = getRandom(20, 30);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
                            isDefeated = true;
                        }
                        break;
                    default:
                        blockNext = true;
                        break;
                    }
                }
                else {
                    choiceDialog("Spider", "You're about to get caught in a web of trouble!", { "Attack with your hands", "Block next attack" }, 20, 40, 0);
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
                                if (quest.name == (string)"Last escape?") {
                                    quest.progress = "COMPLETED";
                                    break;
                                }
                            }
                            int reward = getRandom(20, 30);
                            typeText("You received " + to_string(reward) + " gold.");
                            mainChar.gold += reward;
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
                attack_hand = calcDefence(mainChar.inventory, attack_hand);
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
        else if (enemy_t.name == (string)"Salamander") {
            typeText("BOSS COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                blockNext = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                int attack_dmg_hand = mainChar.strStat;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    attack_dmg_hand = attack_dmg_hand * 2;
                }
                choiceDialog("Salamander", "I AM THE SALAMANDER", { "Attack with " + strongestWpn.name, "Attack with your hands", "Use 'Big Health Potion' (" + to_string(bighealthpotions) + ")", "Use 'Health Potion' (" + to_string(healthpotions) + ")", "Block next attack"}, 20, 40, 0);
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
                        int reward = getRandom(300, 400);
                        typeText("You received " + to_string(reward) + " gold.");
                        mainChar.gold += reward;
                        for (Quest& quest : mainChar.quests) {
                            if (quest.name == (string)"Beat Salamander") {
                                quest.progress = "COMPLETED";
                                typeDialog(mainChar.name, "(breathing heavily) The flames may be fierce, but they couldn't withstand the strength of my resolve.");
                                typeDialog(enemy_t.name, "(weakened) You... you have proven yourself. I did not expect such tenacity from a mere mortal.");
                                typeDialog(mainChar.name, "(offering mercy) Your flames may have scorched, but I won't let vengeance guide my hand. Return to the embers, Salamander, and find peace.");
                                typeDialog(enemy_t.name, "(fading) Perhaps... I underestimated the worthiness of mortals. May your path be free of the fire that consumed me.", 45, 2000, 0);
                                break;
                            }
                        }
                        isDefeated = true;
                    }
                    break;
                case 2:
                    enemy_t.HPstat -= attack_dmg_hand;
                    if (crit) {
                        typeText("You got a critical hit!", 30, 100, 0);
                    }
                    if (enemy_t.HPstat > 0) {
                        typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg_hand) + " damage!", 30, 1000, 0);
                    }
                    else {
                        typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                        int reward = getRandom(300, 400);
                        typeText("You received " + to_string(reward) + " gold.");
                        mainChar.gold += reward;
                        for (Quest& quest : mainChar.quests) {
                            if (quest.name == (string)"Beat Salamander") {
                                quest.progress = "COMPLETED";
                                typeDialog(mainChar.name, "(breathing heavily) The flames may be fierce, but they couldn't withstand the strength of my resolve.");
                                typeDialog(enemy_t.name, "(weakened) You... you have proven yourself. I did not expect such tenacity from a mere mortal.");
                                typeDialog(mainChar.name, "(offering mercy) Your flames may have scorched, but I won't let vengeance guide my hand. Return to the embers, Salamander, and find peace.");
                                typeDialog(enemy_t.name, "(fading) Perhaps... I underestimated the worthiness of mortals. May your path be free of the fire that consumed me.");
                                break;
                            }
                        }
                        isDefeated = true;
                    }
                    break;
                case 3:
                    if (bighealthpotions == 0) {
                        yourTurn = true;
                        typeText("You dont have any 'Big Health Potions'");
                    }
                    else {
                        removeFromInventory(mainChar.inventory, "Big Health Potion");
                        int rem_health = (100 - mainChar.HPStat);
                        if (rem_health >= 50) {
                            mainChar.HPStat += 50;
                        }
                        else {
                            mainChar.HPStat += rem_health;
                        }
                    }
                    break;
                case 4:
                    if (healthpotions == 0) {
                        yourTurn = true;
                        typeText("You dont have any 'Health Potions'");
                    }
                    else {
                        removeFromInventory(mainChar.inventory, "Health Potion");
                        int rem_health = (100 - mainChar.HPStat);
                        if (rem_health >= 25) {
                            mainChar.HPStat += 25;
                        }
                        else {
                            mainChar.HPStat += rem_health;
                        }
                    }
                    break;
                default:
                    blockNext = true;
                    break;
                }
            }
            else {
                yourTurn = true;
                // goblin attacks player, expected for goblins not to have any weapons.
                int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                attack_hand = calcDefence(mainChar.inventory, attack_hand);
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
        else if (enemy_t.name == (string)"Orphan Slaughterer") {
            typeText("BOSS COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                blockNext = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                int attack_dmg_hand = mainChar.strStat;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    attack_dmg_hand = attack_dmg_hand * 2;
                }
                choiceDialog("Orphan Slaughterer", "Are you an orphan? No.. thats sad. I will kill you anyway.", { "Attack with " + strongestWpn.name, "Attack with your hands", "Use 'Big Health Potion' (" + to_string(bighealthpotions) + ")", "Use 'Health Potion' (" + to_string(healthpotions) + ")", "Block next attack" }, 20, 40, 0);
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
                        int reward = getRandom(500, 600);
                        typeText("You received " + to_string(reward) + " gold.");
                        mainChar.gold += reward;
                        for (Quest& quest : mainChar.quests) {
                            if (quest.name == (string)"Beat Orphan Slaughterer") {
                                quest.progress = "COMPLETED";
                                typeDialog(mainChar.name, "(standing victorious) Your reign of terror has come to an end. No more innocents will suffer by your blade.");
                                typeDialog(enemy_t.name, "(defeated) You may have bested me, but the darkness within humanity cannot be extinguished so easily.");
                                typeDialog(mainChar.name, " (with empathy) Darkness can be overcome. It's never too late to find redemption. May you find peace in the afterlife.");
                                typeDialog(enemy_t.name, "(fading) Redemption... a concept I thought lost to me. Perhaps there is hope for a tormented soul like mine.", 45, 2000, 0);
                                break;
                            }
                        }
                        isDefeated = true;
                    }
                    break;
                case 2:
                    enemy_t.HPstat -= attack_dmg_hand;
                    if (crit) {
                        typeText("You got a critical hit!", 30, 100, 0);
                    }
                    if (enemy_t.HPstat > 0) {
                        typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg_hand) + " damage!", 30, 1000, 0);
                    }
                    else {
                        typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                        int reward = getRandom(500, 600);
                        typeText("You received " + to_string(reward) + " gold.");
                        mainChar.gold += reward;
                        for (Quest& quest : mainChar.quests) {
                            if (quest.name == (string)"Beat Orphan Slaughterer") {
                                quest.progress = "COMPLETED";
                                typeDialog(mainChar.name, "(standing victorious) Your reign of terror has come to an end. No more innocents will suffer by your blade.");
                                typeDialog(enemy_t.name, "(defeated) You may have bested me, but the darkness within humanity cannot be extinguished so easily.");
                                typeDialog(mainChar.name, " (with empathy) Darkness can be overcome. It's never too late to find redemption. May you find peace in the afterlife.");
                                typeDialog(enemy_t.name, "(fading) Redemption... a concept I thought lost to me. Perhaps there is hope for a tormented soul like mine.", 45, 2000, 0);
                                break;
                            }
                        }
                        isDefeated = true;
                    }
                    break;
                case 3:
                    if (bighealthpotions == 0) {
                        yourTurn = true;
                        typeText("You dont have any 'Big Health Potions'");
                    }
                    else {
                        removeFromInventory(mainChar.inventory, "Big Health Potion");
                        int rem_health = (100 - mainChar.HPStat);
                        if (rem_health >= 50) {
                            mainChar.HPStat += 50;
                        }
                        else {
                            mainChar.HPStat += rem_health;
                        }
                    }
                    break;
                case 4:
                    if (healthpotions == 0) {
                        yourTurn = true;
                        typeText("You dont have any 'Health Potions'");
                    }
                    else {
                        removeFromInventory(mainChar.inventory, "Health Potion");
                        int rem_health = (100 - mainChar.HPStat);
                        if (rem_health >= 25) {
                            mainChar.HPStat += 25;
                        }
                        else {
                            mainChar.HPStat += rem_health;
                        }
                    }
                    break;
                default:
                    blockNext = true;
                    break;
                }
            }
            else {
                yourTurn = true;
                // goblin attacks player, expected for goblins not to have any weapons.
                int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                attack_hand = calcDefence(mainChar.inventory, attack_hand);
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
        else if (enemy_t.name == (string)"Young Jack") {
            typeText("BOSS COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                blockNext = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                int attack_dmg_hand = mainChar.strStat;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    attack_dmg_hand = attack_dmg_hand * 2;
                }
                choiceDialog("Young Jack", "AAAAAAAARRRHH WHAT DO YOU WANT!!", { "Attack with " + strongestWpn.name, "Attack with your hands", "Use 'Big Health Potion' (" + to_string(bighealthpotions) + ")", "Use 'Health Potion' (" + to_string(healthpotions) + ")", "Block next attack" }, 10, 40, 0);
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
                        int reward = getRandom(900, 1000);
                        typeText("You received " + to_string(reward) + " gold.");
                        mainChar.gold += reward;
                        for (Quest& quest : mainChar.quests) {
                            if (quest.name == (string)"Beat Young Jack") {
                                quest.progress = "COMPLETED";
                                typeDialog(mainChar.name, "The cycle of darkness ends with you, Jack. Your father's twisted legacy won't continue.");
                                typeDialog(enemy_t.name, "(weakened) You... you don't understand. He's powerful beyond measure. You can't stop him.");
                                typeDialog(mainChar.name, " (resolute) I'll face whatever challenges come my way. Your father won't escape the consequences of his actions.");
                                typeDialog(enemy_t.name, "(fading) Maybe... maybe you can change things. Break the cycle. Please...", 45, 2000, 0);
                                break;
                            }
                        }
                        isDefeated = true;
                    }
                    break;
                case 2:
                    enemy_t.HPstat -= attack_dmg_hand;
                    if (crit) {
                        typeText("You got a critical hit!", 30, 100, 0);
                    }
                    if (enemy_t.HPstat > 0) {
                        typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg_hand) + " damage!", 30, 1000, 0);
                    }
                    else {
                        typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                        int reward = getRandom(900, 1000);
                        typeText("You received " + to_string(reward) + " gold.");
                        mainChar.gold += reward;
                        for (Quest& quest : mainChar.quests) {
                            if (quest.name == (string)"Beat Young Jack") {
                                quest.progress = "COMPLETED";
                                typeDialog(mainChar.name, "The cycle of darkness ends with you, Jack. Your father's twisted legacy won't continue.");
                                typeDialog(enemy_t.name, "(weakened) You... you don't understand. He's powerful beyond measure. You can't stop him.");
                                typeDialog(mainChar.name, " (resolute) I'll face whatever challenges come my way. Your father won't escape the consequences of his actions.");
                                typeDialog(enemy_t.name, "(fading) Maybe... maybe you can change things. Break the cycle. Please...", 45, 2000, 0);
                                break;
                            }
                        }
                        isDefeated = true;
                    }
                    break;
                case 3:
                    if (bighealthpotions == 0) {
                        yourTurn = true;
                        typeText("You dont have any 'Big Health Potions'");
                    }
                    else {
                        removeFromInventory(mainChar.inventory, "Big Health Potion");
                        int rem_health = (100 - mainChar.HPStat);
                        if (rem_health >= 50) {
                            mainChar.HPStat += 50;
                        }
                        else {
                            mainChar.HPStat += rem_health;
                        }
                    }
                    break;
                case 4:
                    if (healthpotions == 0) {
                        yourTurn = true;
                        typeText("You dont have any 'Health Potions'");
                    }
                    else {
                        removeFromInventory(mainChar.inventory, "Health Potion");
                        int rem_health = (100 - mainChar.HPStat);
                        if (rem_health >= 25) {
                            mainChar.HPStat += 25;
                        }
                        else {
                            mainChar.HPStat += rem_health;
                        }
                    }
                    break;
                default:
                    blockNext = true;
                    break;
                }
            }
            else {
                yourTurn = true;
                // goblin attacks player, expected for goblins not to have any weapons.
                int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                attack_hand = calcDefence(mainChar.inventory, attack_hand);
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
        else if (enemy_t.name == (string)"Old Jack") {
            typeText("BOSS COMBAT MODE: Your enemy is " + enemy_t.name + " [HP: " + to_string(enemy_t.HPstat) + "]", 10, 300, 0);
            typeText("Your health -> " + to_string(mainChar.HPStat) + "\n", 10, 300, 0);
            if (yourTurn) {
                yourTurn = false;
                blockNext = false;
                bool crit = shouldCall(30);
                int attack_dmg = mainChar.strStat + strongestWpn.damage;
                int attack_dmg_hand = mainChar.strStat;
                if (crit) {
                    attack_dmg = attack_dmg * 2;
                    attack_dmg_hand = attack_dmg_hand * 2;
                }
                choiceDialog("Old Jack", "You are too young to even try defeating me, kid.", { "Attack with " + strongestWpn.name, "Attack with your hands", "Use 'Big Health Potion' (" + to_string(bighealthpotions) + ")", "Use 'Health Potion' (" + to_string(healthpotions) + ")", "Block next attack" }, 10, 40, 0);
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
                        int reward = getRandom(1500, 2000);
                        typeText("You received " + to_string(reward) + " gold.");
                        mainChar.gold += reward;
                        for (Quest& quest : mainChar.quests) {
                            if (quest.name == (string)"Beat Old Jack") {
                                quest.progress = "COMPLETED";
                                typeDialog(mainChar.name, "(breathing heavily) It's over, Old Jack. The darkness you embraced couldn't withstand the resilience of a single soul.");
                                typeDialog(enemy_t.name, "(defeated but defiant) You may have defeated me, but darkness is eternal. It will find another vessel.");
                                typeDialog(mainChar.name, " (steadfast) Darkness can't thrive without a host. I'll ensure your legacy ends here, and the world is free from your malevolence.");
                                typeDialog(enemy_t.name, "Perhaps... perhaps there is hope yet. May you find the strength to face the challenges that lie ahead.");
                                typeDialog(mainChar.name, "Fuck you and you goons.....");
                                typeText("Made by Atlaz and Alabanik (3 am gang)", 45, 20000, 0);
                                break;
                            }
                        }
                        isDefeated = true;
                    }
                    break;
                case 2:
                    enemy_t.HPstat -= attack_dmg_hand;
                    if (crit) {
                        typeText("You got a critical hit!", 30, 100, 0);
                    }
                    if (enemy_t.HPstat > 0) {
                        typeText("You attacked the " + enemy_t.name + " and dealt " + to_string(attack_dmg_hand) + " damage!", 30, 1000, 0);
                    }
                    else {
                        typeText("You have killed the " + enemy_t.name + "!", 30, 1000, 0);
                        int reward = getRandom(1500, 2000);
                        typeText("You received " + to_string(reward) + " gold.");
                        mainChar.gold += reward;
                        for (Quest& quest : mainChar.quests) {
                            if (quest.name == (string)"Beat Old Jack") {
                                quest.progress = "COMPLETED";
                                typeDialog(mainChar.name, "(breathing heavily) It's over, Old Jack. The darkness you embraced couldn't withstand the resilience of a single soul.");
                                typeDialog(enemy_t.name, "(defeated but defiant) You may have defeated me, but darkness is eternal. It will find another vessel.");
                                typeDialog(mainChar.name, " (steadfast) Darkness can't thrive without a host. I'll ensure your legacy ends here, and the world is free from your malevolence.");
                                typeDialog(enemy_t.name, "Perhaps... perhaps there is hope yet. May you find the strength to face the challenges that lie ahead.");
                                typeDialog(mainChar.name, "Fuck you and you goons.....");
                                typeText("Made by Atlaz and Alabanik (3 am gang)", 45, 20000, 0);
                                break;
                            }
                        }
                        isDefeated = true;
                    }
                    break;
                case 3:
                    if (bighealthpotions == 0) {
                        yourTurn = true;
                        typeText("You dont have any 'Big Health Potions'");
                    }
                    else {
                        removeFromInventory(mainChar.inventory, "Big Health Potion");
                        int rem_health = (100 - mainChar.HPStat);
                        if (rem_health >= 50) {
                            mainChar.HPStat += 50;
                        }
                        else {
                            mainChar.HPStat += rem_health;
                        }
                    }
                    break;
                case 4:
                    if (healthpotions == 0) {
                        yourTurn = true;
                        typeText("You dont have any 'Health Potions'");
                    }
                    else {
                        removeFromInventory(mainChar.inventory, "Health Potion");
                        int rem_health = (100 - mainChar.HPStat);
                        if (rem_health >= 25) {
                            mainChar.HPStat += 25;
                        }
                        else {
                            mainChar.HPStat += rem_health;
                        }
                    }
                    break;
                default:
                    blockNext = true;
                    break;
                }
            }
            else {
                yourTurn = true;
                // goblin attacks player, expected for goblins not to have any weapons.
                int attack_hand = getRandom(enemy_t.minDamage, enemy_t.maxDamage);
                attack_hand = calcDefence(mainChar.inventory, attack_hand);
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
        else {
            typeText("A random enemy just passed by...", 30, 2000, 0);
            isDefeated = true;
        }
    }
}