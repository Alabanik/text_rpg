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
};

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
    vector<Item> inventory; // list of items, inventory.

    Player(string playerName, string Class, string race, int level, int HPStat, int intStat, int strStat, int agiStat, int gold, vector<Item> inventory)
        : name(playerName), className(Class), race(race), level(level), HPStat(HPStat), intStat(intStat), strStat(strStat), agiStat(agiStat), gold(gold), inventory(inventory) {}
};

struct Enemy {
    string name;
    int HPstat;
    int level;
    Item weapon;
    int gold;
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
