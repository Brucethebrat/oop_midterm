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

//�g�@�Ӧp�Gplayer ����Y��W�ɡA�I�sonPress��function