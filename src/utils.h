#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

using namespace std;

void typeText(string text, int delay = 45, int endDelay = 50, int startDelay = 0);
void choiceDialog(string talker, string text, vector<string> choices, int delay = 45, int endDelay = 50, int startDelay = 0);
void typeDialog(string talker, string text, int delay = 45, int endDelay = 50, int startDelay = 0);
void makePage(string topMessage, vector<string> bottomMessages);
void resizeConsole(int width, int height);
void clearWnd();
int getRandom(int min, int max);
bool shouldCall(int prob);

#endif
