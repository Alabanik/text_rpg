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

using namespace std;

// init
string mc_name;

// Initialization of sidecharacters to be used later on.
sideChar mother = { "Mom", 45, 9, 0 };
sideChar friend1 = { "John", 13 , 5, 0 };
sideChar friend2 = { "William", 15, 7, 0 };
sideChar OldJack = { "Old Jack", 100 , 1, 0 };
sideChar YoungJack = { "Young Jack", 100 , 1, 0 };
sideChar Witch = { "Witch", 60 , 1, 0 };

// enemy init
Enemy goblin = { "Goblin", 10, 1, {}, 0 }; //{"Goblin", 30 , 1,  0}


// weapon init
Item Sword = { "Sword", "This shit is almost broken", 4, 1 };

// Initialization of functions to be used later on.
void hommik_1(Player mainChar); // Morning, waking up, meeting friends.
void caves_1_choice(Player mainChar); // Going into the cave. Exploring the cave.
void meetMonsterChance(Player mainChar);
void cave_choice_2(Player mainChar, bool skipped);
void go_outside_village(Player mainChar);
void noid_dialog(Player mainChar);
void startGame();


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
    choiceDialog("Narrator", "(Would you like to skip the dialog?)", { "Yes, I have already read through it.", "Id rather read it." }, 30, 60, 0);
    int choice_2;
    printf("> ");
    cin >> choice_2;
    switch (choice_2) {
    case 1:
        // skip the dialog.
        typeText("tl;dr : Your friends decided to open a chest inside a cave.");
        cave_choice_2(mainChar, true);
        break;
    case 2:
        hommik_1(mainChar); //choice 2. Reads
        break;
    }

}

bool isOutside;
bool hasSword;
void hommik_1(Player mainChar) {
    clearWnd();
    typeDialog("Mum", "Wake up! Breakfast is ready.", 45, 50, 0);
    typeDialog(mainChar.name, "Just one more minute, please.", 45, 50, 0);
    typeDialog("Mum", "Okay, but don’t forget your friends are waiting for you outside in 30 minutes. ", 45, 100, 0);
    typeDialog(mainChar.name, "Whatever.", 45, 1000, 0);
    typeText("Friends arrive..", 30, 1000, 3000);
    // Make the player go outside (ask directions and where to go. Move inside the house etc.)
    go_outside_village(mainChar);
    typeText("You are outside.", 10, 30, 0);
    typeDialog(friend1.name, "Well look who woke up.", 45, 50, 0);
    typeDialog(mainChar.name, "Shut it.", 45, 50, 0);
    typeDialog(friend2.name, "Chill, why so cranky.", 45, 50, 0);
    typeDialog(mainChar.name, "Sorry, I just had a bad sleep. Let’s go, check out the cave.");
    typeDialog(mainChar.name, "(looking at the cave entrance) Wow! So dark inside. Do you think it’s a good idea?");
    typeDialog(friend1.name, "(Making a terrified face) Are you scared little boy?");
    typeDialog(friend2.name, "He is definitely thinking about running back to his mum.");
    typeDialog("Friends", "(Laughing)");
    typeDialog(mainChar.name, "Ha Ha so funny. Come on, whoever is last has to kiss Jessica.");
    typeText("They all enter the cave together..", 45, 50, 600);
    caves_1_choice(mainChar);
}

void go_outside_village(Player mainChar) {
    while (!isOutside) {
        clearWnd();
        int dir_choice;
        choiceDialog("Narrator", "Your friends are waiting for you, take your equipment and go outside", {"go outside", "take sword", "talk with mom"}, 10, 30, 0);
        printf("> ");
        cin >> dir_choice;
        switch (dir_choice) {
            case 1:
                for (auto& weapon : mainChar.inventory) {
                    if (weapon.name == "Sword") {
                        isOutside = true;
                        break;
                    }
                }
                if (!isOutside) {
                    typeText("You have to take the weapon before going outside.", 10, 400, 20);
                }
                break;
            case 2:
                // give the sword to player.
                for (auto& weapon : mainChar.inventory) {
                    if (weapon.name == "Sword") {
                        hasSword = true;
                        break;
                    }
                }
                if (!hasSword) {
                    mainChar.inventory.push_back(Sword);
                    typeText("You have the sword, now go outside.", 10, 500, 20);
                }
                else {
                    typeText("You already took the sword, hurry up. Friends are waiting.", 10, 500, 20);
                }
                break;
            case 3:
                typeDialog("Mom", "Make sure to eat well before you leave.", 45, 400, 20);
                break;
            default:
                typeText("Invalid choice.", 10, 900, 3);

        }
    };
    clearWnd();
}

void caves_1_choice(Player mainChar) {
    typeDialog(friend2.name, "It’s so wet here.");
    typeDialog(friend1.name, "I can smell something nasty, I think it is dead corps..");
    typeDialog(mainChar.name, "Take the torch and light it.");
    typeText("The three of them proceed to light up the torches and find an old chest with spider webs all over it.", 45, 50, 600);
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
    cave_choice_2(mainChar, false);
}

void cave_choice_2(Player mainChar, bool skipped) {
    if (!skipped) {
        typeText("Trio crack opens the chest.", 45, 50, 600);
        typeDialog(mainChar.name, "Wow! Look at  jewelry.");
        typeDialog(friend2.name, "This is incredible! We hit the jackpot!");
        typeDialog(friend1.name, "I told you there was something valuable here. Let's grab as much as we can.");
        typeDialog(mainChar.name, "Hold on, let's not get too carried away. We don't know if this belongs to someone, and we wouldn't want trouble.");
        typeDialog(friend2.name, "(mockingly)Oh, come on, don't be such a killjoy. It's probably been abandoned for years.");
        typeDialog(mainChar.name, " (hesitant)I don't know, guys. Something feels off about this.");
        typeDialog(friend1.name, "Stop being paranoid. Look at these gems and jewels. We could be rich!");
        typeDialog(mainChar.name, "Look a script.");
        typeDialog(friend2.name, "What does it say?");
        typeDialog(mainChar.name, " ( breathing heavily)“For those that are here, listen carefully.I am Old Jack, the father of Young Jack.Do not dare to steal from us or bad things will happen.This cave was made by me against the demons.The humans won that fight and trapped the souls in here.DO NOT go looking for them.” - Old Jack");
        typeDialog(friend1.name, "This is some crazy writing, who cares. Better to get the stuff and go back because it is getting late.");
        typeText("As the friends start dividing the loot among themselves, a distant sound echoes through the cave, like a low rumble. The atmosphere grows tense, and a sudden chill runs down their spines.", 45, 50, 600);
        typeDialog(mainChar.name, "(whispering) Did you hear that ?");
        typeDialog(friend2.name, "Relax, it's probably just the wind.");
        typeText("Suddenly, the torches flicker, and the cave seems to come alive with ominous whispers. The trio exchange uneasy glances as the air becomes charged with an otherworldly energy.", 45, 50, 600);
        typeDialog(friend1.name, "(nervously) Okay, maybe we should wrap this up and get out of here.");
        typeDialog(mainChar.name, "Agreed. Let's not overstay our welcome.");
        typeText("Just as they're about to leave, a mysterious mist envelops the cave, and the entrance disappears, leaving them trapped in the darkness.", 45, 50, 600);
        typeDialog(friend2.name, "What's happening? Where's the way out?");
        typeDialog(mainChar.name, "I knew we shouldn't have messed with this chest. We've unleashed something.");
    }
    typeText("As the trio frantically searches for an exit, the cave transforms into a labyrinth of winding passages and shifting walls. Panic sets in as they realize they are no longer in control.", 45, 50, 600);
    // alustab siit
    choiceDialog("Narrator", "You see a pathway going straight and left", { "straight", "left" });
    int choice;
    cin >> choice;
    switch(choice){
        case 1: //kohtub nõiaga
			noid_dialog(mainChar);
            break;

        case 2:
            noid_dialog(mainChar);
            break;
    }
}

void meetMonsterChance(Player mainChar) {

}

void noid_dialog(Player mainChar){
    //typeDialog
}