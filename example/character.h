#pragma once

#include "room.h"

enum Drct { Drct_Down, Drct_Left, Drct_Right, Drct_Up, Drct_None };

struct Character {
	int posX, posY;
	int volX, volY;
	Drct d;
	ImageData image;
};

void move(Character& ch);

void attemptMove(Character& ch, const Room& room);

bool onIntTile(Character& ch, const Room& room);
