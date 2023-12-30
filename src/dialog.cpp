#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>

//#include <cmath>
#include <ctime>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include "structs.h"
#include "utils.h"
#include "dialog.h"
#include "combat.h"

using namespace std;

// make player, other init
string mc_name;
bool isOutside = false; // 1
bool hasSword = false; // 1
bool inLabyrinth = false;
bool isDadDead = false; // klui see ture = revart
bool hasBeenFarm = false;
bool hasLastStretchQuest = false;
bool areFriendsDead = false;

vector<string> lootedRooms;
vector<string> defeatedBosses;
// visited shit
bool visitedWitch = false; //Q1
bool visitedDad = false; //Q2
bool visitedSide1 = false; //Q3
bool visitedSide2 = false; //Q4

// characters - mainChar.gold += 1000;
// name, occupation, race, level, health, inteligence, str, agi, gold, inventory; 
Player mainChar = { mc_name, "Adventurer", "Human", "A1", {}, 0, 100, 0, 3, 8, 0, {} };
// side
sideChar mother = { "Mom", 45, 9, 0 };
sideChar friend1 = { "John", 13 , 5, 0 }; //side 1
sideChar friend2 = { "William", 15, 7, 0 }; //side 2
sideChar OldJack = { "Old Jack", 100 , 1, 0 };
sideChar YoungJack = { "Young Jack", 100 , 1, 0 };
sideChar Witch = { "Witch", 60 , 1, 0 }; //side 4
sideChar Dad = {"Dad", 47, 5, 0}; //side 3
sideChar shopKeep1 = { "Winslow's Wonders Market", 100, 100, 999999999};
sideChar shopKeep2 = {"Ivy's Curios Corner", 100, 100,  999999999};
// enemy
// name, hpstat, level, weapon, gold, minDmg, maxDmg
Enemy jew = { "Juut", 20 ,1 ,{}, 0, 3, 7};
Enemy goblin = { "Goblin", 10, 1, {}, 0, 1, 4 };
Enemy oldgoblin = { "Old Goblin", 50, 12, {}, 0, 10, 20 };
Enemy ghost = { "Ghost", 50, 1, {}, 0, 5, 10 }; // Q2 enemy
Enemy spider = { "Spider", 30, 1, {}, 0, 10, 20 }; // Q2 enemy
Enemy mini1 = { "Salamander", 100, 1, {}, 0, 10, 20 };
Enemy mini2 = { "Orphan Slaughterer", 250, 1, { }, 0, 20, 30};
Enemy mini3 = { "Young Jack", 500, 1, { }, 0, 20, 50};
Enemy boss = { "Old Jack", 10000, 250, { }, 0, 30, 70 };

// weapons
Item Sword = { "Sword", "This shit is almost broken", 4, 1, true, false};
Item dagger = { "Dagger", "Well it is something",1, 1, true, false};
Item ironSword = { "Iron Sword", "A simple sword.. but its made out of iron.", 50, 1, true, false };
Item magicalStaff = { "Magical Staff", "This has something magical to it.", 100, 1, true, false };
Item scaleSword = { "Scale Sword", "A sword made out of scales.. SCALES!", 350, 1, true, false };
Item dragonSword = { "Dragon Sword", "Aincent Sword made out of dragons bone.", 500, 1, true, false };
Item copperSword = { "Copper Sword", "Budget-friendly with modest cutting capabilities", 25,1, true, false };
Item OpSword = { "Op Sword", "Fucking cheater", 10000000,1,true, false };
// other items
Item cuteCatEars = {"Cute Cat ears", "Has no purpose", 25 ,1, false, true };
Item hpPotion = { "Health Potion", "Gives you a pat on the back.", 25, 1, false, false };
Item BhpPotion = { "Big Health Potion", "Gives you a bigger pat on the back.", 50, 1, false, false };
Item armor = { "Armor", "Gives you some extra protection if you know what I mean", 50, 1,false, true };

// quests
Quest noiaQuest = { "Old Goblin", "Retrive the stolen magical hat.", "IN_PROGRESS" }; //Q1
Quest dadQuest = { "Welcome back dad!", "Escape the labyrinth with dad", "IN_PROGRESS" };//Q2
Quest collectingJewelry = { "Lost jewelry", "Retrieve John's jewelry", "IN_PROGRESS" }; //Q3
Quest escapingCave = { "Last escape?", "Escape the dungeon with William", "IN_PROGRESS" }; //Q4
Quest lastStretch = {"Last stretch", "null", "IN_PROGRESS"};
Quest miniBoss_1 = { "Beat Salamander", "What the title says..", "IN_PROGRESS" };
Quest miniBoss_2 = { "Beat Orphan Slaughterer", "What the title says..", "IN_PROGRESS" };
Quest miniBoss_3 = { "Beat Young Jack", "What the title says..", "IN_PROGRESS" };
Quest finalBoss = { "Beat Old Jack", "Beat Old Jack to escape the cave!!!!!!!!!!", "IN_PROGRESS" };

// functions
void startGame(bool after_death); // d
void hommikScene(); // d
void goOutsideTheVillage(); // d
void exploreLabyrinth(); // big no, wdym big no?
void caveChoice_1(); // d
void caveChoice_2(bool skipped); // d
void noidDialog();
void dad_quest(bool acceptQuest);
bool hasBeenInLootroom(string name);
void deathOccur();
void checkStats(); // d
void openInventory(); // d
void Lost_jewelry(bool acceptQuest);
void Last_escape(bool acceptQuest);
void Pood(string t_cord);

void startGame(bool after_death) { // algus
    clearWnd();
    if (!after_death){
        // Ask the user for their name.
        printf("Enter your name: ");
        cin >> mc_name;
        mainChar.name = mc_name;
        lastStretch.description = "William and " + mc_name + " have found more intel to escape the dungeon";
        clearWnd();
        typeText("You start waking up from a deep sleep, your throat parched and body heavy.", 50, 25, 1000);
        typeText("The air is thick with unfamiliar stillness, and as you open your eyes,", 50, 25, 100);
        typeText("Your gaze follows the faint glow that beckons from the end of a narrow tunnel", 50, 25, 100);
        typeText("A surge of fear courses through you, and you start thinking about the past you have been through.", 50, 1000, 100);
    }
    choiceDialog("Narrator", "(Would you like to skip the dialog?)", { "Yes, I have already read through it.", "Id rather read it." }, 30, 60, 0);
    int choice_2;
    printf("> ");
    cin >> choice_2;
    switch (choice_2) {
    case 1:
        // skip the dialog.
        mainChar.inventory.push_back(Sword);
        mainChar.inventory.push_back(hpPotion);
        mainChar.inventory.push_back(hpPotion);
        mainChar.inventory.push_back(hpPotion);
        typeText("tl;dr : Your friends decided to open a chest inside a cave.");
        caveChoice_2(true);
        break;
    case 2:
        hommikScene(); //choice 2. Reads
        break;
    }

}

void hommikScene() {
    clearWnd();
    typeDialog("Mum", "Wake up! Breakfast is ready.", 45, 50, 0);
    typeDialog(mainChar.name, "Just one more minute, please.", 45, 50, 0);
    typeDialog("Mum", "Okay, but don’t forget your friends are waiting for you outside in 30 minutes. ", 45, 100, 0);
    typeDialog(mainChar.name, "Whatever.", 45, 1000, 0);
    typeText("Friends arrive..", 30, 1000, 3000);
    // Make the player go outside (ask directions and where to go. Move inside the house etc.)
    goOutsideTheVillage();
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
    caveChoice_1();
}

void goOutsideTheVillage() {
    while (!isOutside) {
        clearWnd();
        int dir_choice;
        choiceDialog("Narrator", "Your friends are waiting for you, take your equipment and go outside", { "go outside", "take sword", "talk with mom" }, 10, 30, 0);
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
                typeText("You have to take the weapon before going outside.", 10, 1400, 20);
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
                typeText("You have the sword, now go outside.", 10, 1500, 20);
            }
            else {
                typeText("You already took the sword, hurry up. Friends are waiting.", 10, 1500, 20);
            }
            break;
        case 3:
            typeDialog("Mom", "Make sure to eat well before you leave.", 45, 1400, 20);
            break;
        default:
            typeText("Invalid choice.", 10, 1900, 3);

        }
    };
    clearWnd();
}

void caveChoice_1() {
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
    caveChoice_2(false);
}

void caveChoice_2(bool skipped) {
    if (!skipped) {
        typeText("Trio cracks opens the chest.", 45, 50, 600);
        typeDialog(mainChar.name, "Wow! Look at jewelry.");
        typeDialog(friend2.name, "This is incredible! We hit the jackpot!");
        typeDialog(friend1.name, "I told you there was something valuable here. Let's grab as much as we can.");
        typeDialog(mainChar.name, "Hold on, let's not get too carried away. We don't know if this belongs to someone, and we wouldn't want trouble.");
        typeDialog(friend2.name, "(mockingly)Oh, come on, don't be such a killjoy. It's probably been abandoned for years.");
        typeDialog(mainChar.name, " (hesitant)I don't know, guys. Something feels off about this.");
        typeDialog(friend1.name, "Stop being paranoid. Look at these gems and jewels. We could be rich!");
        typeDialog(mainChar.name, "Look a script.");
        typeDialog(friend2.name, "What does it say?");
        typeDialog(mainChar.name, "( breathing heavily) \"For those that are here, listen carefully. I am Old Jack, the father of Young Jack.Do not dare to steal from us or bad things will happen.This cave was made by me against the demons.The humans won that fight and trapped the souls in here.DO NOT go looking for them.\" - Old Jack");
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
    mainChar.cord = "A1";
    inLabyrinth = true;
    exploreLabyrinth();
}

void noidDialog() { // quest 1
    bool noidQuest_acceptedQuest = false;
    bool noidQuest_waitingReward = false;
    bool noidQuest_gotReward = false;
    for (auto& quest : mainChar.quests) {
        if (quest.name == (string)"Old Goblin") {
            noidQuest_acceptedQuest = true;
            if (quest.progress == (string)"COMPLETED") {
                noidQuest_gotReward = true;
            }
        }
    }
    if (!noidQuest_acceptedQuest && !visitedWitch) {
        // has not got the quest yet
        typeDialog(Witch.name, "(with a crooked smile) Ah, a lone wanderer. Lost, perhaps? Or seeking something more... mystical?", 45, 100, 0);
        typeDialog(mainChar.name, "(guarded) I didn't mean to disturb anything. We were just exploring this cave, and now I don't know how to get out.", 45, 100, 0);
        typeDialog(Witch.name, "(laughs softly) Exploration often leads to unexpected encounters. Fear not, young one, for you stand in the realm of the arcane.", 45, 100, 0);
        typeDialog(mainChar.name, "(curious but cautious) Who are you?", 45, 100, 0);
        typeDialog(Witch.name, "(mysteriously) I am but a humble guardian of these ancient passages, a weaver of spells and keeper of secrets. What brings you to my domain?", 45, 100, 0);
        typeDialog(mainChar.name, "(choosing words carefully) We found a chest, treasures... and then everything changed. We're trapped, and I don't know how to undo it.", 45, 100, 0);
        typeDialog(Witch.name, "(stroking her chin) Trapped, you say? The threads of fate weave intricate patterns. To undo what's been done, a price must be paid.", 45, 100, 0);
        typeDialog(mainChar.name, "(worried) A price? What do you mean?", 45, 100, 0);
        typeDialog(Witch.name, "(leaning closer) The spirits of this cave demand balance. A choice must be made, and consequences embraced. Speak your desire, and the threads may yet be untangled.", 45, 100, 0);
        typeDialog(mainChar.name, "(hesitant) I just want to leave, to go back to where I belong.", 45, 100, 0);
        typeDialog(Witch.name, "(with a knowing gaze) Very well. The path to freedom is not always straightforward. Follow the whispers, and you shall find what you seek. But beware, for every choice echoes through the tapestry of destiny.", 45, 100, 0);
        typeDialog(Witch.name, "I can give a clue but first. There is an old goblin who stole my magical hat, if you can return me the hat you shall be given the knowledge of the old realm.", 45, 100, 0);
        choiceDialog(Witch.name, "What do you think about my offer?", { "I will accept your offer", "Sorry, I cant right now. (leave)" }, 30, 100, 0);
        visitedWitch = true;
        int choice_Noid;
        printf("> ");
        cin >> choice_Noid;
        switch (choice_Noid) {
            case 1: // accept
                mainChar.quests.push_back(noiaQuest);
                mainChar.cord = "Q1";
                break;
            case 2: // deny
                mainChar.cord = "A3";
                break;
            default: // deny
                mainChar.cord = "A3";
                break;
        }
    }
    else if (!noidQuest_acceptedQuest) {
        typeDialog(Witch.name, "Welcome back, I still need help retrieving my stolen magical hat.", 45, 100, 0);
        choiceDialog(Witch.name, "Have you changed your mind?", { "I will accept your offer", "Sorry, I cant right now. (leave)" }, 30, 100, 0);
        visitedWitch = true;
        int choice_Noid;
        printf("> ");
        cin >> choice_Noid;
        switch (choice_Noid) {
        case 1: // accept
            mainChar.quests.push_back(noiaQuest);
            mainChar.cord = "Q1";
            break;
        case 2: // deny
            mainChar.cord = "A3";
            break;
        default: // deny
            mainChar.cord = "A3";
            break;
        }
    }
    else if (noidQuest_acceptedQuest && !noidQuest_gotReward) {
        // if has accepted quest and is waiting to get their reward.
        typeDialog(Witch.name, "You've done it! The old goblin was defeated, and the magical hat returned. I Am grateful for your bravery.", 45, 100, 0);
        typeDialog(mainChar.name, "The goblin was tough, but the hat is back where it belongs.", 45, 100, 0);
        typeDialog(Witch.name, "I see, (hands you a red potion) take this and drink it.", 45, 700, 0);
        typeText("After drinking the potion you feel a lot better. Your health has increased.", 20, 500, 0);
        mainChar.HPStat = 100; // set hp to 100
        typeDialog(Witch.name, "Your heroism won't be forgotten.", 45, 100, 0);
        typeDialog(Witch.name, "Before exiting the cave you have to defeat 3 minibosses and final boss. Wander around and be careful goblins and other creatures have infested the place.", 45, 100, 0);
        mainChar.gold += 50;
        mainChar.quests.push_back(miniBoss_1);
        mainChar.quests.push_back(miniBoss_2);
        mainChar.quests.push_back(miniBoss_3);
        for (auto& quest : mainChar.quests) {
            if (quest.name == (string)"Old Goblin") {
                quest.progress = "COMPLETED";
            }
        }
        mainChar.cord = "A3";
    }
    else {
        // else, they have reveived the reward after completing the quest.
        typeDialog(Witch.name, "Havent seen you in some time.", 45, 100, 0);
        choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
        int choice_A2;
        printf("> ");
        cin >> choice_A2;
        switch (choice_A2) {
        case 1: mainChar.cord = "A1"; break;
        case 2: mainChar.cord = "A3"; break;
        case 3: openInventory(); break;
        case 4: checkStats(); break;
        default: break;
        }
    }
}

void dad_quest(bool acceptQuest) { // quest 2
    if (!acceptQuest && !visitedDad) {
        typeDialog("Dad", " (gravely) You've come.");
        typeDialog(mainChar.name, ": (nervously) Yeah, I... I wanted to see you.");
        typeDialog("Dad", "(stoically) It's been a long time.");
        typeDialog(mainChar.name, " (hesitant) I know. I thought maybe... maybe we could talk.");
        typeDialog("Dad", " (softly) There are things you don't know, things I couldn't say.");
        typeDialog(mainChar.name, "yearning) I just want to understand, Dad. Why did you leave?");
        typeDialog("Dad", "(whispering) Life, it's complicated. Choices are made, and sometimes they're not the right ones.");
        typeDialog(mainChar.name, "(vulnerable) I missed you. Mom missed you.");
        typeDialog("Dad", "regretful) I know, and I can't change what's happened, but I may know the way out of here!");
        choiceDialog(Dad.name, "Do you want to go?", { "Sure", "Sorry, I cant right now. (leave)" }, 30, 100, 0);
        visitedDad = true;
        int choice_dad;
        printf("> ");
        cin >> choice_dad;
        switch (choice_dad) {
        case 1: // accept
            mainChar.quests.push_back(dadQuest);
            mainChar.cord = "A22";
            break;
        case 2: // deny
            mainChar.cord = "A23";
            break;
        default: // deny
            mainChar.cord = "A23";
            break;
        }
    }
    else if (!acceptQuest) {
        typeDialog(Dad.name, "I still know the way", 45, 100, 0);
        choiceDialog(Dad.name, "Do you want to go?", { "Sure", "Sorry, I cant right now. (leave)" }, 30, 100, 0);
        visitedDad = true;
        int choice_dad;
        printf("> ");
        cin >> choice_dad;
        switch (choice_dad) {
        case 1: // accept
			mainChar.quests.push_back(dadQuest);
            mainChar.cord = "A22";
            break;
        case 2: // deny
            mainChar.cord = "A23";
            break;
        default: // deny
            mainChar.cord = "A23";
            break;
        }
    }

}

void Pood(string t_cord) {
    clearWnd();
    if (t_cord == (string)"P1") {
        typeText("You have " + to_string(mainChar.gold) + " gold.");
        choiceDialog(shopKeep1.name, "What would you like to purchase?", { "Health potion (50 G)", "Cute cat ears (199 G)", "Copper sword(100 G)", "Leave the store."}, 20, 600, 100);
        int shop;
        cin >> shop;
        switch (shop) {
        case 1:
            mainChar.inventory.push_back(hpPotion);
            mainChar.gold -= 50;
            break;
        case 2:
            mainChar.inventory.push_back(cuteCatEars);
            mainChar.gold -= 199;
            break;
        case 3:
            mainChar.inventory.push_back(copperSword);
            mainChar.gold -= 100;
            break;
        default:
            break;
        }
    }
    else if (t_cord == (string)"P2") {
        // teine shop
		typeText("You have " + to_string(mainChar.gold) + " gold.");
		choiceDialog(shopKeep2.name, "What would you like to purchase?", { "Big health potion (70 G)", "Cute cat ears (199 G)", "Iron sword(100 G)", "Armor(250 G)", "Leave the store." }, 20, 600, 100);
		int shop;
		cin >> shop;
		switch (shop) {
		case 1:
			mainChar.inventory.push_back(BhpPotion);
			mainChar.gold -= 50;
			break;
		case 2:
			mainChar.inventory.push_back(cuteCatEars);
			mainChar.gold -= 199;
			break;
		case 3:
			mainChar.inventory.push_back(ironSword);
			mainChar.gold -= 100;
			break;
            case 4:
				mainChar.inventory.push_back(armor);
				mainChar.gold -= 100;
				break;
		default:
			break;
		}
    }
};

void Lost_jewelry(bool acceptQuest) { //quest 3
	bool LostJewelryQuest_acceptedQuest = false;
	bool LostJewelryQuest_waitingReward = false;
	bool LostJewelryQuest_gotReward = false;
	for (auto& quest : mainChar.quests) {
		if (quest.name == (string)"Lost jewelry") {
			LostJewelryQuest_acceptedQuest = true;
			if (quest.progress == (string)"COMPLETED") {
				LostJewelryQuest_gotReward = true;
			}
		}
	}

    if (!acceptQuest && !visitedSide1) {
        typeDialog(friend1.name, " (excitedly): Well, look who decided to show up! Where have you been hiding, stranger?");
        typeDialog(mainChar.name, "Drop it. I said we shouldn't open the chest .");
        typeDialog(friend1.name, "Well we are here now, maybe stop crying? I lost my jewelry on the way here. Let's get it?");
        choiceDialog(friend1.name, "Do you want to help me?", { "Sure", "Sorry, I cant right now. (leave)" }, 30, 100, 0);
        visitedSide1 = true;
        int choice_Side1;
        printf("> ");
        cin >> choice_Side1;
        switch (choice_Side1) {
            case 1: // accept
                typeDialog(friend1.name,"Let's get a move on then!");
                mainChar.quests.push_back(collectingJewelry);
                mainChar.cord = "S2";
                break;
            case 2: // deny
                mainChar.cord = "A10";
                break;
            default: // deny
                mainChar.cord = "A10";
                break;
        }
    }else if (!acceptQuest) {
		typeDialog(friend1.name, "My jewelry is still lost!", 45, 100, 0);
		choiceDialog(friend1.name, "Do you want to help me?", { "Sure", "Sorry, I cant right now. (leave)" }, 30, 100, 0);
		visitedDad = true;
		int choice_Side1;
		printf("> ");
		cin >> choice_Side1;
		switch (choice_Side1) {
		case 1: // accept
			typeDialog(friend1.name, "Let's get a move on then!");
			mainChar.quests.push_back(collectingJewelry);
			mainChar.cord = "S2";
			break;
		case 2: // deny
			mainChar.cord = "A10";
			break;
		default: // deny
			mainChar.cord = "A10";
			break;
		}
	}

};

void Last_escape(bool acceptQuest){ // quest4
	bool LastEscapeQuest_acceptedQuest = false;
	bool LastEscapeQuest_waitingReward = false;
	bool LastEscapeQuest_gotReward = false;
	for (auto& quest : mainChar.quests) {
		if (quest.name == (string)"Last escape") {
			LastEscapeQuest_acceptedQuest = true;
			if (quest.progress == (string)"COMPLETED") {
				LastEscapeQuest_gotReward = true;
			}
		}
	}

	if (!acceptQuest && !visitedSide2) {
		typeDialog(friend2.name, " YOU ARE HERE!");
		typeDialog(mainChar.name, "Well well well.");
		typeDialog(friend2.name, "You little bastard made it.");
		typeDialog(mainChar.name, "Of course I did, who do you think I am?");
		typeDialog(friend2.name, "I met some weird things on my way out. Want to help me get rid of them?");
		typeDialog(mainChar.name, " If we can get out of here then yes.");
		choiceDialog(friend2.name, "Shall we go then?", { "Sure", "Sorry, I cant right now. (leave)" }, 30, 100, 0);
		visitedSide2 = true;
		int choice_Side2;
		printf("> ");
		cin >> choice_Side2;
		switch (choice_Side2) {
		case 1: // accept
			typeDialog(friend2.name, "Davai!");
            mainChar.quests.push_back(escapingCave);
			mainChar.cord = "S1";
			break;
		case 2: // deny
			mainChar.cord = "A8";
			break;
		default: // deny
			mainChar.cord = "A8";
			break;
		}
	}
	else if (!acceptQuest) {
		typeDialog(friend1.name, "We should go asap!", 45, 100, 0);
		choiceDialog(friend1.name, "Shall we go?", { "Sure", "Sorry, I cant right now. (leave)" }, 30, 100, 0);
		visitedDad = true;
		int choice_Side2;
		printf("> ");
		cin >> choice_Side2;
		switch (choice_Side2) {
		case 1: // accept
			typeDialog(friend1.name, "Davai!");
			mainChar.quests.push_back(escapingCave);
			mainChar.cord = "S1";
			break;
		case 2: // deny
			mainChar.cord = "A10";
			break;
		default: // deny
			mainChar.cord = "A10";
			break;
		}
	}

}

void openInventory() {
    clearWnd();
    typeText(mainChar.name + " - Inventory");
    typeText("Gold: " + to_string(mainChar.gold));
    typeText("List of items:");
    for (int i = 0; i < mainChar.inventory.size(); ++i) {
        cout << "> " << to_string(i + 1) << ". " << mainChar.inventory[i].name << "\n";
    }
    printf("\n");
    system("pause");
}

void checkStats() {
    clearWnd();
    typeText(mainChar.name + " - Stats");
    typeText("Class: " + mainChar.className);
    typeText("HP: " + to_string(mainChar.HPStat));
    typeText("STR: " + to_string(mainChar.strStat));
    typeText("LVL: " + to_string(mainChar.level));
    printf("\n");
    system("pause");
}

void callCombat(Enemy enemy) {
    if (shouldCall(31)) {
        combatOccur(mainChar, enemy);
    }
}

bool hasBeenInLootroom(string name) {
    bool hasBeenHere = false;
    for (auto& room : lootedRooms) {
        if (room == (string)name) {
            hasBeenHere = true;
        }
    }
    return hasBeenHere;
}

void deathOccur() {
    clearWnd();
    // set variables to original.
    visitedWitch = false; //Q1
    visitedDad = false; //Q2
    visitedSide1 = false; //Q3
    visitedSide2 = false; //Q4

    hasBeenFarm = false;
    hasLastStretchQuest = false;
    areFriendsDead = false;
    // clear inventory, but give back sword.
    mainChar.gold = 0;
    mainChar.inventory.clear();
    mainChar.inventory.push_back(Sword);

    // set cord to the start
    mainChar.cord = "A1";
    inLabyrinth = true; // respawns at the A1 cordinate.
    // other stats
    mainChar.HPStat = 100;
    lootedRooms.clear();
    mainChar.quests.clear();
    defeatedBosses.clear();

    typeText("You died. Your progress has reset.", 40, 1000, 0);
    typeText("Respawning at the start of the labyrinth..", 15, 2000, 0);
    clearWnd();
    exploreLabyrinth();
}

bool didDefeatBoss(string Boss) {
    for (auto& boss : defeatedBosses) {
        if (boss == Boss) {
            return true;
        }
    }
    return false;
}

string cord;
void exploreLabyrinth() {
    while (inLabyrinth) { // hell loop
        clearWnd();
        cout << "Your current cords: " << mainChar.cord << "\n";
        if (!mainChar.quests.size() == 0) {
            cout << "\n";
            for (int i = 0; i < mainChar.quests.size(); ++i) {
                cout << " " << to_string(i + 1) << ". " << mainChar.quests[i].name << " : \"" << mainChar.quests[i].description << "\" : " << mainChar.quests[i].progress << "\n";
            }
            cout << "\n\n";
        }
        cord = mainChar.cord;
        
        
        //Boss loops
        if (cord == (string)"M1") {
            bool defeatM1 = didDefeatBoss("M1");
            if (!defeatM1) {
                choiceDialog("Narrator", "Do you want to challenge " + mini1.name, { "I think im strong enough", "Sadly i have to decline :(" }, 10, 100, 100);
                int M1;
                cin >> M1;
                switch (M1) {
                case 1:
                    // accept
                    typeDialog(mainChar.name,"(grim determination) Salamander, your fiery reign of terror ends here! I won't let you harm this realm any longer." );
					typeDialog(mini1.name, "(sinister hiss) Mortal, you dare challenge the flames? Prepare to be consumed by the inferno you face!");
                    combatOccur(mainChar, mini1);
                    defeatedBosses.push_back("M1");
                    break;
                default:
                    mainChar.cord = "A34";
                }
            }
            else {
                choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
                int choice_A11;
                printf("> ");
                cin >> choice_A11;
                switch (choice_A11) {
                case 1: mainChar.cord = "A42"; break;
                case 2: mainChar.cord = "A34"; break;
                case 3: openInventory(); break;
                case 4: checkStats(); break;
                default: break;
                }
            }
        }

        if (cord == (string)"M2") {
            bool defeatM2 = didDefeatBoss("M2");
            if (!defeatM2) {
                choiceDialog("Narrator", "Do you want to challenge " + mini2.name, { "I think im strong enough", "Sadly i have to decline :(" }, 10, 100, 100);
                int M2;
                cin >> M2;
                switch (M2) {
                case 1:
                    // accept
                    typeDialog(mainChar.name, " (with a stern expression) Orphan Slaughterer, your merciless deeds end now! No more innocent lives will be taken by your hand.");
					typeDialog(mini2.name, "(cold laughter) Mortals are so predictable, clinging to their fragile notions of justice. Prepare to witness true power! ");
                    combatOccur(mainChar, mini2);
                    defeatedBosses.push_back("M2");
                    break;
                default:
                    mainChar.cord = "A34";
                }
            }
            else {
                choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
                int choice_M2;
                printf("> ");
                cin >> choice_M2;
                switch (choice_M2) {
                case 1: mainChar.cord = "A41"; break;
                case 2: mainChar.cord = "A35"; break;
                case 3: openInventory(); break;
                case 4: checkStats(); break;
                default: break;
                }
            }
        }

        if (cord == (string)"M3") {
            bool defeatM3 = didDefeatBoss("M3");
            if (!defeatM3) {
                choiceDialog("Narrator", "Do you want to challenge " + mini3.name, { "I think im strong enough", "Sadly i have to decline :(" }, 10, 100, 100);
                int M3;
                cin >> M3;
                switch (M3) {
                case 1:
                    // accept
                    typeDialog(mainChar.name, "Young Jack, your journey down this twisted path ends here. Your father's darkness won't consume any more lives.");
					typeDialog(mini3.name, " You know nothing about our struggles, outsider. This is the only way to survive in our world.");
                    combatOccur(mainChar, mini3);
                    defeatedBosses.push_back("M3");
                    break;
                default:
                    mainChar.cord = "A36";
                }
            }
            else {
                choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
                int choice_M3;
                printf("> ");
                cin >> choice_M3;
                switch (choice_M3) {
                case 1: mainChar.cord = "A40"; break;
                case 2: mainChar.cord = "A36"; break;
                case 3: openInventory(); break;
                case 4: checkStats(); break;
                default: break;
                }
            }
        }
        

        if (cord == (string)"FB1") {
            bool defeatM1 = didDefeatBoss("FB1");
            if (!defeatM1) {
                choiceDialog("Narrator", "Do you want to challenge " + boss.name, { "I think im strong enough", "Sadly i have to decline :(" }, 10, 100, 100);
                int FB1;
                cin >> FB1;
                switch (FB1) {
                case 1:
                    // accept
					typeDialog(finalBoss.name, " So, you've made it this far, little one. Impressive. But you cannot comprehend the power I wield.");
					typeDialog(mainChar.name, "Your reign of darkness ends here, Old Jack. The pain and suffering you've caused won't persist any longer.");
                    typeDialog(finalBoss.name, "(sinister chuckle) Pain, suffering... mere ingredients in the grand recipe of power. You think you can challenge what I've become?");
					typeDialog(mainChar.name, "I may be just one, but the strength of my conviction surpasses the shadows you've embraced. It's time to face the consequences.");
                    combatOccur(mainChar, boss);
                    defeatedBosses.push_back("FB1");
                    break;
                default:
                    mainChar.cord = "L2";
                }
            }
            else {
                choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
                int choice_FB1;
                printf("> ");
                cin >> choice_FB1;
                switch (choice_FB1) {
                case 1: mainChar.cord = "A42"; break;
                case 2: mainChar.cord = "A34"; break;
                case 3: openInventory(); break;
                case 4: checkStats(); break;
                default: break;
                }
            }
        }
        
        
        
        // noid
        if (cord == (string)"N1") {
            noidDialog();
        }
        // Q category
		if (cord == (string)"Q1") {
            // Old goblin quest.
            bool oldGoblin_acceptedQuest = false;
            bool oldGoblin_alreadyBeat = false;

            for (auto& quest : mainChar.quests) {
                if (quest.name == (string)"Old Goblin") {
                    oldGoblin_acceptedQuest = true;
                    if (quest.progress == (string)"COMPLETED") {
                        oldGoblin_alreadyBeat = true;
                    }
                    else if (quest.progress == (string)"WAITING_FOR_REWARD") {
                        oldGoblin_alreadyBeat = true;
                    }
                }
            }
            if (oldGoblin_acceptedQuest && !oldGoblin_alreadyBeat) {
                typeText("You search for the goblin that the witch described to you.");
                typeText("A little bit goes by...", 30, 1000, 0);
                typeText("...", 150, 1000, 0);
                typeText("You see a goblin sitting beside the wall with a magical hat on their head. You go closer.", 20, 1000, 0);
                typeDialog(mainChar.name, "Old Goblin, Im here to fight with you today.", 45, 150, 0);
                typeDialog("Old Goblin", "Who are you? Go away, I dont have time for some little kid to ruin my day!", 45, 150, 0);
                typeDialog(mainChar.name, "I know you stole the magical hat, return it and you dont have to die.", 45, 150, 0);
                typeDialog("Old Goblin", "You talk big for someone so little. Come get it if you dare. Muhaha", 45, 1000, 0);
                combatOccur(mainChar, oldgoblin);
            }
            else {
                choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
                int choice_A2;
                printf("> ");
                cin >> choice_A2;
                switch (choice_A2) {
                case 1: mainChar.cord = "N1"; break;
                case 2: mainChar.cord = "LT1"; break;
                case 3: openInventory(); break;
                case 4: checkStats(); break;
                default: break;
                }
            }
		}
		if (cord == (string)"Q2") {
            // dad quest
            bool dadQuest_acceptedQuest = false;
            bool dadQuest_gotReward = false;

            for (auto& quest : mainChar.quests) {
                if (quest.name == (string)"Welcome back dad!") {
                    dadQuest_acceptedQuest = true;
                    if (quest.progress == (string)"COMPLETED") {
                        dadQuest_gotReward = true;
                    }
                }
            }
            if (dadQuest_acceptedQuest && !dadQuest_gotReward && visitedDad) {
                // fight ghost
                typeDialog("Dad", "SON THERE IS A GHOST!!", 10, 300, 0);
                combatOccur(mainChar, ghost);
            }
            else {
                choiceDialog("Narrator", "You see a pathway going south and west", { "Go South", "Go West", "Open Inventory", "Check Stats" });
                int choice_A2;
                printf("> ");
                cin >> choice_A2;
                switch (choice_A2) {
                case 1: mainChar.cord = "A22"; break;
                case 2: mainChar.cord = "A21"; break;
                case 3: openInventory(); break;
                case 4: checkStats(); break;
                default: break;
                }
            }
            
		}
		if (cord == (string)"Q3") {
			bool collectingJewelryQuest_acceptedQuest = false;
			bool collectingJewelryQuest_gotReward = false;

			for (auto& quest : mainChar.quests) {
				if (quest.name == (string)"Lost jewelry") {
					collectingJewelryQuest_acceptedQuest = true;
					if (quest.progress == (string)"COMPLETED") {
						collectingJewelryQuest_gotReward = true;
					}
				}
			}
            if (collectingJewelryQuest_acceptedQuest && !collectingJewelryQuest_gotReward && visitedSide1){
                //fight juut
                typeDialog(friend1.name,"I found the guys who stole my jewelry", 45, 1000, 0);
                combatOccur(mainChar, jew);
            }
			else {
				choiceDialog("Narrator", "You see a pathway going south and east", { "Go South", "Go East", "Open Inventory", "Check Stats" });
				int choice_A2;
				printf("> ");
				cin >> choice_A2;
				switch (choice_A2) {
				case 1: mainChar.cord = "A20"; break;
				case 2: mainChar.cord = "A21"; break;
				case 3: openInventory(); break;
				case 4: checkStats(); break;
				default: break;
				}
			}
		}
		if (cord == (string)"Q4") {
			bool escapingCaveQuest_acceptedQuest = false;
			bool escapingCaveQuest_gotReward = false;

			for (auto& quest : mainChar.quests) {
				if (quest.name == (string)"Last escape?") {
					escapingCaveQuest_acceptedQuest = true;
					if (quest.progress == (string)"COMPLETED") {
						escapingCaveQuest_gotReward = true;
					}
				}
			}
            if (escapingCaveQuest_acceptedQuest && !escapingCaveQuest_gotReward && visitedSide2){
                // fight mdea 
                typeDialog(friend2.name, "These are the bugs who are blocking our escape");
                combatOccur(mainChar, spider);
            }
			else {
                // annab quest 5
                if (escapingCaveQuest_gotReward && !hasLastStretchQuest) {
                    mainChar.quests.push_back(lastStretch);
                    for (auto& quest : mainChar.quests) {
                        if (quest.name == (string)"Last stretch") {
                            hasLastStretchQuest = true;
                            break;
                        }
                    }
                }
				choiceDialog("Narrator", "You see a pathway going north, south, west and east", {"Go North", "Go South", "Go West", "Go East", "Open Inventory", "Check Stats" });
				int choice_A2;
				printf("> ");
				cin >> choice_A2;
				switch (choice_A2) {
				case 1: mainChar.cord = "A29"; break;
				case 2: mainChar.cord = "A16"; break;
                case 3: mainChar.cord = "A31"; break;
				case 4: mainChar.cord = "A17"; break;
				case 5: openInventory(); break;
				case 6: checkStats(); break;
				default: break;
				}
			}
		}
        if (cord == (string)"Q5") {
            // LAST STRETCH
            if (!areFriendsDead) {
                for (auto& quest : mainChar.quests) {
                    if (quest.name == (string)"Last stretch") {
                        quest.progress = "COMPLETED";
                        areFriendsDead = true;
                        typeDialog("Narrator", "You hear a bang and look back, your frinds have fallen through the floor and died", 20, 400, 0);
                    }
                }
            }
            choiceDialog("Narrator", "You see a pathway going north, west and east", { "Go North", "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A2;
            printf("> ");
            cin >> choice_A2;
            switch (choice_A2) {
            case 1: mainChar.cord = "A20"; break;
            case 2: mainChar.cord = "A19"; break;
            case 3: mainChar.cord = "A18"; break;
            case 4: openInventory(); break;
            case 5: checkStats(); break;
            default: break;
            }
        }
        // Side char

		if (cord == (string)"S1") { //escapingCave Last escape? Last_escape 
			bool escapingCaveQuest_acceptedQuest = false;
			bool escapingCaveQuest_gotReward = false;

			for (auto& quest : mainChar.quests) {
				if (quest.name == (string)"Last escape?") {
					escapingCaveQuest_acceptedQuest = true;
					if (quest.progress == (string)"COMPLETED") {
						escapingCaveQuest_gotReward = true;
					}
				}
			}
			if (!escapingCaveQuest_acceptedQuest) {
				Last_escape(escapingCaveQuest_acceptedQuest);
            }

			else {
				choiceDialog("Narrator", "You see a pathway going north south and west", { "Go North","Go South", "Go West", "Open Inventory", "Check Stats" });
				int choice_A2;
				printf("> ");
				cin >> choice_A2;
				switch (choice_A2) {
				case 1: mainChar.cord = "A24"; break;
				case 2: mainChar.cord = "A8"; break;
				case 3: mainChar.cord = "A27"; break;
				case 4: openInventory(); break;
				case 5: checkStats(); break;
				default: break;
				}
			}
		}
        if (cord == (string)"S2") {
			bool collectingJewelryQuest_acceptedQuest = false;
			bool collectingJewelryQuest_gotReward = false;

			for (auto& quest : mainChar.quests) {
				if (quest.name == (string)"Lost jewelry") {
					collectingJewelryQuest_acceptedQuest = true;
					if (quest.progress == (string)"COMPLETED") {
						collectingJewelryQuest_gotReward = true;
					}
				}
			}
			if (!collectingJewelryQuest_acceptedQuest) {
				Lost_jewelry(collectingJewelryQuest_acceptedQuest);
            } else {
			    choiceDialog("Narrator", "You see a pathway going south and west", { "Go South", "Go West", "Open Inventory", "Check Stats" });
			    int choice_A2;
			    printf("> ");
			    cin >> choice_A2;
			    switch (choice_A2) {
			        case 1: mainChar.cord = "A10"; break;
			        case 2: mainChar.cord = "A17"; break;
			        case 3: openInventory(); break;
			        case 4: checkStats(); break;
			        default: break;
			    }
			}


		}
        if (cord == (string)"S3") {
            bool dadQuest_acceptedQuest = false;
            bool dadQuest_gotReward = false;

            for (auto& quest : mainChar.quests) {
                if (quest.name == (string)"Welcome back dad!") {
                    dadQuest_acceptedQuest = true;
                    if (quest.progress == (string)"COMPLETED") {
                        dadQuest_gotReward = true;
                    }
                }
            }
            if (!dadQuest_acceptedQuest) {
                dad_quest(dadQuest_acceptedQuest);
            }
            else {
                choiceDialog("Narrator", "You see a pathway going south and west", { "Go South", "Go West", "Open Inventory", "Check Stats" });
                int choice_A2;
                printf("> ");
                cin >> choice_A2;
                switch (choice_A2) {
                case 1: mainChar.cord = "A23"; break;
                case 2: mainChar.cord = "A22"; break;
                case 3: openInventory(); break;
                case 4: checkStats(); break;
                default: break;
                }
            }
		}
        if (cord == (string)"S4") {
            // noid annab magical staffi
			bool hasBeenHere = hasBeenInLootroom("S4");
            if (!hasBeenHere) {
				typeDialog(mainChar.name, "Witch? WHat are you doing here?");
				typeDialog(Witch.name, "Ahh! Don’t scare me like that. I’m just wandering around.");
				typeDialog(mainChar.name, "Huh, Okei. Well I’m in some need of weapons can you help me with that?");
				typeDialog(Witch.name, "Let me look at what I got.");
                choiceDialog(Witch.name, "If you can guess the secret number", { "Enter number" });
                int num1;
                cin >> num1;
                if (num1 == 332) {
                    mainChar.inventory.push_back(magicalStaff);
                    lootedRooms.push_back("S4");
                }
                else {
					typeDialog(Witch.name, "Wrong no more tries.");
					lootedRooms.push_back("S4");
                }
            }
            else {
                // do something else
                typeDialog(Witch.name, "Get Out of here!");
                mainChar.cord = "A43";
            };
		}

        // misc catecory
        if (cord == (string)"F1") { // t
          

            if(!hasBeenFarm) { // kui ei ole olnud farmis
				typeDialog("Narrator", "The dilapidated farmcave stood as a weathered testament to seasons long gone, its timeworn walls echoing whispers of bygone harvests and the echoes of laughter that once filled the rustic air.", 20, 3000, 100);
                hasBeenFarm = true;
            }
            else {
                choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
                int choice_A1;
                printf("> ");
                cin >> choice_A1;
                switch (choice_A1) {
                case 1: mainChar.cord = "A43"; break;
                case 2: mainChar.cord = "A44"; break;
                case 3: openInventory(); break;
                case 4: checkStats(); break;
                default: break;
                }
            }
        }
        if (cord == (string)"P1") { // t
            choiceDialog("Narrator", "Do you want to enter the shop?", { "Yes", "No" }, 40, 100, 0);
            int shop_choice_1;
            printf("> ");
            cin >> shop_choice_1;
            switch (shop_choice_1) {
                case 1:
                    Pood("P1");
                    break;
                default:
                    choiceDialog("Narrator", "You see a pathway going north, south, east and west", { "Go North", "Go South", "Go East", "Go West", "Open Inventory", "Check Stats" });
                    int choice_A1;
                    printf("> ");
                    cin >> choice_A1;
                    switch (choice_A1) {
                        case 1: mainChar.cord = "A26"; break;
                        case 2: mainChar.cord = "A28"; break;
                        case 3: mainChar.cord = "A25"; break;
                        case 4: mainChar.cord = "A30"; break;
                        case 5: openInventory(); break;
                        case 6: checkStats(); break;
                        default: break;
                    }
                    break;
            }
        }
		if (cord == (string)"P2") { // t
            choiceDialog("Narrator", "Do you want to enter the shop?", { "Yes", "No" }, 40, 100, 0);
            int shop_choice_1;
            printf("> ");
            cin >> shop_choice_1;
            switch (shop_choice_1) {
            case 1:
                Pood("P2");
                break;
            default:
                choiceDialog("Narrator", "You see a pathway going south, east and west", { "Go South", "Go East", "Go West", "Open Inventory", "Check Stats" });
                int choice_A1;
                printf("> ");
                cin >> choice_A1;
                switch (choice_A1) {
                case 1: mainChar.cord = "A45"; break;
                case 2: mainChar.cord = "L2"; break;
                case 3: mainChar.cord = "A44"; break;
                case 4: openInventory(); break;
                case 5: checkStats(); break;
                default: break;
                }
                break;
            }
		}
        if(cord == (string)"HP1") {
            choiceDialog("Narrator", "Do you want to dip into the magic pool?", { "Yes", "No" });
            int aaaaa;
            cin >> aaaaa;
            switch (aaaaa) {
                case 1:
                    typeText("You feel better and find your health better than before");
                    mainChar.HPStat = 100;
                    break;
                case 2:
                    choiceDialog("Narrator", "You see a pathway going west and north.", { "Go West", "Go north", "Open Inventory", "Check Stats" });
                    int choice_A1;
                    printf("> ");
                    cin >> choice_A1;
                    switch (choice_A1) {
                        case 1: mainChar.cord = "A5"; break;
                        case 2: mainChar.cord = "A4"; break;
                        case 3: openInventory(); break;
                        case 4: checkStats(); break;
                        default: break;
                    }
                default:
                    break;
            }
        }
		if (cord == (string)"HP2"){
			choiceDialog("Narrator", "Do you want to teleport to the magic pool?", { "Yes", "No" });
			int aaaaa;
			cin >> aaaaa;
            switch (aaaaa) {
            case 1:
				typeText(" You teleported to the magic pool");
                mainChar.cord = "HP1";
                break;
            case 2:
                mainChar.cord = "A38";
                break;
            default: break;
            }
        }
		if (cord == (string)"L2") {
			choiceDialog("Narrator", "You see a pathway going west and east.", { "Go West", "Go East", "Open Inventory", "Check Stats" });
			int choice_A1;
			printf("> ");
			cin >> choice_A1;
			switch (choice_A1) {
			case 1: mainChar.cord = "P2"; break;
			case 2: mainChar.cord = "FB1"; break;
			case 3: openInventory(); break;
			case 4: checkStats(); break;
			default: break;
			}
		}
        //LT catecory
		if (cord == (string)"LT1") {
            callCombat(jew);
            bool hasBeenHere = hasBeenInLootroom("LT1");
            if (!hasBeenHere) {
                typeText("You have found 100 gold and a dagger", 45, 1000, 0);
                mainChar.inventory.push_back(dagger);
                mainChar.gold += 100;
                lootedRooms.push_back("LT1");
            }
            else {
                // do something else
                typeText("The room has been already looted");
            }
			choiceDialog("Narrator", "You see a pathway going north, south and west", { "Go North","Go South", "Go West", "Open Inventory", "Check Stats" });
			int choice_lt1;
			printf("> ");
			cin >> choice_lt1;
			switch (choice_lt1) {
			    case 1: mainChar.cord = "A3"; break;
			    case 2: mainChar.cord = "A4"; break;
			    case 3: mainChar.cord = "A6"; break;
			    case 4: openInventory(); break;
			    case 5: checkStats(); break;
			    default: break;
			}
		}
		if (cord == (string)"LT2") {
			callCombat(jew);
            bool hasBeenHere = hasBeenInLootroom("LT2");
            if (!hasBeenHere) {
                typeText("You have found 350 gold and left the room.", 45, 1000, 0);
                mainChar.gold += 350;
                lootedRooms.push_back("LT2");
            }
            else {
                // do something else
                typeText("The room has been already looted");
            }
            mainChar.cord = "A14";
		}
		if (cord == (string)"LT3") {
			callCombat(jew);
            bool hasBeenHere = hasBeenInLootroom("LT3");
            if (!hasBeenHere) {
                typeText("You have found 200 gold and left the room.", 45, 1000, 0);
                mainChar.gold += 350;
                lootedRooms.push_back("LT3");
            }
            else {
                // do something else
                typeText(" The room has been already looted");
            }
            mainChar.cord = "A42";
		}
		if (cord == (string)"LT4") {
			callCombat(jew);
            bool hasBeenHere = hasBeenInLootroom("LT4");
            if (!hasBeenHere) {
                typeText("You have found 500 gold and an iron sword.", 45, 1000, 0);
                mainChar.inventory.push_back(ironSword);
                mainChar.gold += 500;
                lootedRooms.push_back("LT4");
            }
            else {
                // do something else
                typeText(" The room has been already looted");
            }
            mainChar.cord = "A41";
		}
		if (cord == (string)"LT5") {
			callCombat(jew);
            bool hasBeenHere = hasBeenInLootroom("LT5");
            if (!hasBeenHere) {
                typeText("You have found 1000 gold and a scale sword.", 45, 1000, 0);;
                mainChar.inventory.push_back(scaleSword);
                mainChar.gold += 1000;
                lootedRooms.push_back("LT5");
            }
            else {
                // do something else
                typeText(" The room has been already looted");
            }
            mainChar.cord = "A40";
		}

        // A category
        if (cord == (string)"A1") { // t
			choiceDialog("Narrator", "You see a pathway going south and east", { "Go South", "Go East", "Open Inventory", "Check Stats" });
			int choice_A1;
			printf("> ");
			cin >> choice_A1;
            switch (choice_A1) {
                case 1: mainChar.cord = "N1"; break;
                case 2: mainChar.cord = "A2"; break;
                case 3: openInventory(); break;
                case 4: checkStats(); break;
                default: break;
            }
        }
        if (cord == (string)"A2") { // t
            choiceDialog("Narrator", "You see a pathway going south and west", { "Go South", "Go West", "Open Inventory", "Check Stats" });
            int choice_A2;
            printf("> ");
            cin >> choice_A2;
            switch (choice_A2) {
            case 1: mainChar.cord = "N1"; break;
            case 2: mainChar.cord = "A1"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A3") { // t
            choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A3;
            printf("> ");
            cin >> choice_A3;
            switch (choice_A3) {
            case 1: mainChar.cord = "N1"; break;
            case 2: mainChar.cord = "LT1"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A4") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A4;
            printf("> ");
            cin >> choice_A4;
            switch (choice_A4) {
            case 1: mainChar.cord = "LT1"; break;
            case 2: mainChar.cord = "HP1"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A5") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north, south, east and west", { "Go North", "Go South","Go East","Go West", "Open Inventory", "Check Stats" });
            int choice_A5;
            printf("> ");
            cin >> choice_A5;
            switch (choice_A5) {
            case 1: mainChar.cord = "A39"; break;
            case 2: mainChar.cord = "A11"; break;
            case 3: mainChar.cord = "HP1"; break;
            case 4: mainChar.cord = "A9"; break;
            case 5: openInventory(); break;
            case 6: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A6") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north, south, east and west", { "Go North", "Go South", "Go East", "Go West", "Open Inventory", "Check Stats" });
            int choice_A6;
            printf("> ");
            cin >> choice_A6;
            switch (choice_A6) {
            case 1: mainChar.cord = "A23"; break;
            case 2: mainChar.cord = "A39"; break;
            case 3: mainChar.cord = "LT1"; break;
            case 4: mainChar.cord = "A7"; break;
            case 5: openInventory(); break;
            case 6: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A7") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and east", { "Go North", "Go East", "Open Inventory", "Check Stats" });
            int choice_A7;
            printf("> ");
            cin >> choice_A7;
            switch (choice_A7) {
            case 1: mainChar.cord = "A8"; break;
            case 2: mainChar.cord = "A6"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A8") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A8;
            printf("> ");
            cin >> choice_A8;
            switch (choice_A8) {
            case 1: mainChar.cord = "S1"; break;
            case 2: mainChar.cord = "A7"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A9") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going south, east and west", { "Go South", "Go East", "Go West", "Open Inventory", "Check Stats" });
            int choice_A9;
            printf("> ");
            cin >> choice_A9;
            switch (choice_A9) {
            case 1: mainChar.cord = "A13"; break;
            case 2: mainChar.cord = "A5"; break;
            case 3: mainChar.cord = "A10"; break;
            case 4: openInventory(); break;
            case 5: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A10") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and east", { "Go North", "Go East", "Open Inventory", "Check Stats" });
            int choice_A10;
            printf("> ");
            cin >> choice_A10;
            switch (choice_A10) {
            case 1: mainChar.cord = "S2"; break;
            case 2: mainChar.cord = "A9"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A11") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and west", { "Go North", "Go West", "Open Inventory", "Check Stats" });
            int choice_A11;
            printf("> ");
            cin >> choice_A11;
            switch (choice_A11) {
            case 1: mainChar.cord = "A5"; break;
            case 2: mainChar.cord = "A12"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A12") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north, east and west", { "Go North", "Go East", "Go West", "Open Inventory", "Check Stats" });
            int choice_A12;
            printf("> ");
            cin >> choice_A12;
            switch (choice_A12) {
            case 1: mainChar.cord = "A13"; break;
            case 2: mainChar.cord = "A11"; break;
            case 3: mainChar.cord = "A14"; break;
            case 4: openInventory(); break;
            case 5: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A13") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A13;
            printf("> ");
            cin >> choice_A13;
            switch (choice_A13) {
            case 1: mainChar.cord = "A9"; break;
            case 2: mainChar.cord = "A12"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A14") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north, east and west", { "Go North", "Go East", "Go West", "Open Inventory", "Check Stats" });
            int choice_A14;
            printf("> ");
            cin >> choice_A14;
            switch (choice_A14) {
            case 1: mainChar.cord = "LT2"; break;
            case 2: mainChar.cord = "A12"; break;
            case 3: mainChar.cord = "A15"; break;
            case 4: openInventory(); break;
            case 5: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A15") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and east", { "Go North", "Go East", "Open Inventory", "Check Stats" });
            int choice_A15;
            printf("> ");
            cin >> choice_A15;
            switch (choice_A15) {
            case 1: mainChar.cord = "A16"; break;
            case 2: mainChar.cord = "A14"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A16") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north, south and west", { "Go North", "Go South", "Go West", "Open Inventory", "Check Stats" });
            int choice_A16;
            printf("> ");
            cin >> choice_A16;
            switch (choice_A16) {
            case 1: mainChar.cord = "Q4"; break;
            case 2: mainChar.cord = "A15"; break;
            case 3: mainChar.cord = "A18"; break;
            case 4: openInventory(); break;
            case 5: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A17") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A17;
            printf("> ");
            cin >> choice_A17;
            switch (choice_A17) {
            case 1: mainChar.cord = "Q4"; break;
            case 2: mainChar.cord = "S2"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A18") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A18;
            printf("> ");
            cin >> choice_A18;
            switch (choice_A18) {
            case 1: mainChar.cord = "Q5"; break;
            case 2: mainChar.cord = "A16"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A19") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A19;
            printf("> ");
            cin >> choice_A19;
            switch (choice_A19) {
            case 1: mainChar.cord = "A32"; break;
            case 2: mainChar.cord = "Q5"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A20") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A20;
            printf("> ");
            cin >> choice_A20;
            switch (choice_A20) {
            case 1: mainChar.cord = "Q3"; break;
            case 2: mainChar.cord = "Q5"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A21") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going south, east and west", { "Go South", "Go East", "Go West", "Open Inventory", "Check Stats" });
            int choice_A21;
            printf("> ");
            cin >> choice_A21;
            switch (choice_A21) {
            case 1: mainChar.cord = "A26"; break;
            case 2: mainChar.cord = "Q2"; break;
            case 3: mainChar.cord = "Q3"; break;
            case 4: openInventory(); break;
            case 5: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A22") { // t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north, south, east and west", { "Go North", "Go South", "Go East", "Go West", "Open Inventory", "Check Stats" });
            int choice_A22;
            printf("> ");
            cin >> choice_A22;
            switch (choice_A22) {
            case 1: mainChar.cord = "Q2"; break;
            case 2: mainChar.cord = "A24"; break;
            case 3: mainChar.cord = "S3"; break;
            case 4: mainChar.cord = "A25"; break;
            case 5: openInventory(); break;
            case 6: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A23") { //T
            callCombat(goblin);
			choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A23;
            printf("> ");
            cin >> choice_A23;
            switch (choice_A23) {
            case 1: mainChar.cord = "S3"; break;
            case 2: mainChar.cord = "A6"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A24") { //T
            callCombat(goblin);
			choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A24;
            printf("> ");
            cin >> choice_A24;
            switch (choice_A24) {
            case 1: mainChar.cord = "A22"; break;
            case 2: mainChar.cord = "S1"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A25") { //T
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A25;
            printf("> ");
            cin >> choice_A25;
            switch (choice_A25) {
            case 1: mainChar.cord = "P1"; break;
            case 2: mainChar.cord = "A22"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A26") { //T
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A26;
            printf("> ");
            cin >> choice_A26;
            switch (choice_A26) {
            case 1: mainChar.cord = "A21"; break;
            case 2: mainChar.cord = "P1"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A27") { //T
            callCombat(goblin);
			choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A27;
            printf("> ");
            cin >> choice_A27;
            switch (choice_A27) {
            case 1: mainChar.cord = "A28"; break;
            case 2: mainChar.cord = "S1"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A28") { //t
            callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north south and east", { "Go North","Go South", "Go East", "Open Inventory", "Check Stats" });
            int choice_A28;
            printf("> ");
            cin >> choice_A28;
            switch (choice_A28) {
            case 1: mainChar.cord = "P1"; break;
            case 2: mainChar.cord = "A29"; break;
            case 3: mainChar.cord = "A27"; break;
            case 4: openInventory(); break;
            case 5: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A29") { //t
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A29;
            printf("> ");
            cin >> choice_A29;
            switch (choice_A29) {
            case 1: mainChar.cord = "A28"; break;
            case 2: mainChar.cord = "Q4"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A30") { //T
						 
         callCombat(goblin);choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A30;
            printf("> ");
            cin >> choice_A30;
            switch (choice_A30) {
            case 1: mainChar.cord = "A20"; break;
            case 2: mainChar.cord = "P1"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A31") { //T
			 callCombat(goblin);
        choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A31;
            printf("> ");
            cin >> choice_A31;
            switch (choice_A31) {
            case 1: mainChar.cord = "A20"; break;
            case 2: mainChar.cord = "Q4"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A32") { // t
			callCombat(goblin);
        choiceDialog("Narrator", "You see a 4 doors and pathway going north and east", { "Go North", "Go East","Choose door 1", "Choose door 2", "Choose door 3", "Choose door 4", "Open Inventory", "Check Stats" });
            int choice_A32;
            printf("> ");
            cin >> choice_A32;
            switch (choice_A32) {
            case 1: mainChar.cord = "A45"; break;
            case 2: mainChar.cord = "A19"; break;
            case 3: mainChar.cord = "A34"; break;
            case 4: mainChar.cord = "A35"; break;
            case 5: mainChar.cord = "A36"; break;
            case 6: mainChar.cord = "A38"; break;;
            case 7: openInventory(); break;
            case 8: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A34") { //T
			 callCombat(goblin);
        choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A34;
            printf("> ");
            cin >> choice_A34;
            switch (choice_A34) {
            case 1: mainChar.cord = "M1"; break;
            case 2: mainChar.cord = "A32"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A35") { //T
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A35;
            printf("> ");
            cin >> choice_A35;
            switch (choice_A35) {
            case 1: mainChar.cord = "M2"; break;
            case 2: mainChar.cord = "A32"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A36") {  //T
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A36;
            printf("> ");
            cin >> choice_A36;
            switch (choice_A36) {
            case 1: mainChar.cord = "M3"; break;
            case 2: mainChar.cord = "A32"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A38") { //T
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A38;
            printf("> ");
            cin >> choice_A38;
            switch (choice_A38) {
            case 1: mainChar.cord = "HP2"; break;
            case 2: mainChar.cord = "A32"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A39") { //T
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A39;
            printf("> ");
            cin >> choice_A39;
            switch (choice_A39) {
            case 1: mainChar.cord = "A6"; break;
            case 2: mainChar.cord = "A5"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A40") { //T
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A40;
            printf("> ");
            cin >> choice_A40;
            switch (choice_A40) {
            case 1: mainChar.cord = "LT5"; break;
            case 2: mainChar.cord = "M3"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A41") { //T
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A41;
            printf("> ");
            cin >> choice_A41;
            switch (choice_A41) {
            case 1: mainChar.cord = "LT4"; break;
            case 2: mainChar.cord = "M2"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A42") { //T
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A42;
            printf("> ");
            cin >> choice_A42;
            switch (choice_A42) {
            case 1: mainChar.cord = "LT3"; break;
            case 2: mainChar.cord = "M1"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A43") { //T
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A43;
            printf("> ");
            cin >> choice_A43;
            switch (choice_A43) {
            case 1: mainChar.cord = "S4"; break;
            case 2: mainChar.cord = "F1"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A44") { //T
			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going west and east", { "Go West", "Go East", "Open Inventory", "Check Stats" });
            int choice_A44;
            printf("> ");
            cin >> choice_A44;
            switch (choice_A44) {
            case 1: mainChar.cord = "F1"; break;
            case 2: mainChar.cord = "P2"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
        if (cord == (string)"A45") {//T

			callCombat(goblin);
            choiceDialog("Narrator", "You see a pathway going north and south", { "Go North", "Go South", "Open Inventory", "Check Stats" });
            int choice_A45;
            printf("> ");
            cin >> choice_A45;
            switch (choice_A45) {
            case 1: mainChar.cord = "P2"; break;
            case 2: mainChar.cord = "A32"; break;
            case 3: openInventory(); break;
            case 4: checkStats(); break;
            default: break;
            }
        };
    }
}