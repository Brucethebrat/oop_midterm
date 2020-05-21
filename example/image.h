#pragma once

#include "defs.h"

struct ImageData
{
	char path[100];
	SDL_Texture *texture;
	int width;
	int height;
	int num;
	int wn;
	int hn;
};

ImageData loadTexture(char *path, int num, int hn, int wn, bool ckEnable, Uint8 r, Uint8 g, Uint8 b);

void imgRender(SDL_Renderer *renderer, ImageData img, int posX, int posY, int frame);

void showScreen()