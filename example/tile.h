#pragma once

enum Movable { MovEvent, MovMovable, MovUnmovable };

struct Tile {
	Movable mov;
	Room* nxtRoom;
	int nxtX, nxtY;
};


bool movable(Tile& t, Character& ch);
void onPressed(Tile& t, Character& ch);