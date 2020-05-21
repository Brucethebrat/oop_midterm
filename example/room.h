#pragma once

#include <vector>
#include "tile.h"
#include "character.h"

using std::vector;

struct Room {
	vector<vector<Tile>> tiles;
	Character player;
	Character oni;
};

//寫一個如果player 走到某格上時，呼叫onPress的function