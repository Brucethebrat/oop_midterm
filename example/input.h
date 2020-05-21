#pragma once

#include "defs.h"
#include "character.h"

enum Keys { Key_Down, Key_Left, Key_Right, Key_Up, Key_None };
enum Op { Op_Down, Op_Up };
std::vector<InputEvent> inputLog;
int MAX_KEYS = 0;
int PRESS_KEY_NUM = 0;
Keys TIMER_SWITCH = Key_None;


struct InputEvent {
	Keys k;
	Op o;
};

void handleEvent(SDL_Event& e);
