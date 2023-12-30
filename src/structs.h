#pragma once

#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>

using namespace std;

struct Item { // also counts for weapon
    string name;
    string description;
    int damage;
    int rarity; // 1-5, 1 common, 5-legendary (common, uncommon, rare, epic, legendary)
    bool isWeapon;
    bool isDefence;
};

struct Quest {
    string name;
    string description;
    string progress; // "IN_PROGRESS", "COMPLETED", "WAITING_FOR_REWARD"
};

struct Player {
    // Basic info
    string name;
    string className;
    string race; // useless
    string cord; // where the player is at.
    
    vector<Quest> quests;

    // Stats
    int level; // ...
    int HPStat; // healthpoints
    int intStat; // inteligence, mana, magic
    int strStat; // strength
    int agiStat; // speed

    // other
    int gold; // Amount of money the main character has.
    vector<Item> inventory; // list of items, inventory.

    Item strongestWeapon(vector<Item> inventory) {
        if (inventory.empty()) {
            return Item{ "", "", 0, 0, true };
        }
        Item* strongestWeapon = nullptr;
        for (auto& weapon : inventory) {
            if (weapon.isWeapon && (!strongestWeapon || weapon.damage > strongestWeapon->damage)) {
                strongestWeapon = &weapon;
            }
        }
        return strongestWeapon ? *strongestWeapon : Item{ "", "", 0, 0 };
    }

};

struct Enemy {
    string name;
    int HPstat;
    int level;
    Item weapon;
    int gold;
    int minDamage;
    int maxDamage;
};

//Weapon weapon_1 = {"relva_nimi", 20};
//Enemy enemy_1 = { "Nimi", 20, 100, weapon_1 };

struct sideChar {
    // basic info
    string name;
    int age;
    int relationship; // Relationship with the main character. 1-10
    int gold; // Amount of money this character has.
};

#endif
