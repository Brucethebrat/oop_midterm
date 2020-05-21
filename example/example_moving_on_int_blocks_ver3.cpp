/* Alphar: 2020.04.19 */

#include "defs.h"
#include "tile.h"
#include "room.h"
#include "image.h"
#include "character.h"
#include "input.h"

using namespace std;


const int CH01_FRAME = 16;
const int BLOCKNUM = 3;

const int VELOCITY = 5;
const int CH01_WIDTH = 50;
const int CH01_HEIGHT = 100;

//Initialize the velocity, position and direction
//int CH01_VELX = 0;
//int CH01_VELY = 0;
ImageData ch01;
int CH01_POSX = 350;	//WIDTH / 2; 375
int CH01_POSY = 200;	//HEIGHT / 2; 225

//int DISTANCE = 0;	//when moving, if DISTANCE hasn't reach 50, then keep walking

//when true, timer run, when false, timer stop, used in handle event, when moving: true, stop: false
enum Drct {Drct_Down, Drct_Left, Drct_Right, Drct_Up, Drct_None};


Drct curDrct = Drct_None;


/*
struct ImageData
{
	char path[100];
	SDL_Texture *texture;
	int width;
	int height;
};
*/



int initSDL(); // Starts up SDL and creates window
void closeSDL(); // Frees media and shuts down SDL



Uint32 aniAction(Uint32 interval, void* param); // Timer callback function
//void move(char matrix[10][15]);
void show_map(char matrix[10][15], ImageData blocks[BLOCKNUM]);


SDL_Window *window = NULL; // The window we'll be rendering to
SDL_Renderer *renderer = NULL; // The window renderer

/*
// Current displayed texture
SDL_Texture *bkTexture = NULL;
SDL_Texture *pikaTexture = NULL;
SDL_Texture *spTexture = NULL;
*/

int initSDL()
{
	// Initialize SDL	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		// Error Handling		
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	// Create window	
	// SDL_WINDOWPOS_UNDEFINED: Used to indicate that you don't care what the window position is.
	window = SDL_CreateWindow("Blue Demon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 2;
	}

	// Initialize PNG loading	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image failed: %s\n", IMG_GetError());
		return 3;
	}

	// Create renderer	
	// VSync allows the rendering to update at the same time as when your monitor updates during vertical refresh.
	// For this tutorial it will make sure the animation doesn't run too fast. 
	// Most monitors run at about 60 frames per second and that's the assumption we're making here. 
	// If you have a different monitor refresh rate, that would explain why the animation is running too fast or slow.
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 3;
	}

	return 0;
}


void closeSDL()
{
	//meaning couldn't understand
	/*	
	// Free loaded image
	SDL_DestroyTexture(bkTexture);
	SDL_DestroyTexture(pikaTexture);
	*/

	// Destroy renderer	
	// Destroy window	
	// Quit Image subsystem
	// Quit SDL subsystems
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}


Uint32 aniAction(Uint32 interval, void *param)
{
	int *index = (int *)param;
	//printf("switch in timer = %d\n", TIMER_SWITCH);
	if (curDrct != Drct_None) {
		//index[1] = (index[1] + 1) % PFRAME;
		//index[2] = (index[2] + 1) % SFRAME;
		index[2] = (index[2] + 1) % (CH01_FRAME / 4) + curDrct * 4;
		//printf("index[2] = %d", index[2]);
		//printf("direction = %d", DIRECTION);

		return interval;
	}

	else {
		//index[2] = 0 + DIRECTION[0] * 4;
		index[2] = index[2] / 4 * 4;
		return interval;
	}	
}


Drct fetchDrct() {
	bool released[4] = { false };
	for (int i = inputLog.size() - 1; i >= 0; i--) {
		if (inputLog[i].o == Op_Down && !released[inputLog[i].k]) {
			switch (inputLog[i].k) {
				case Key_Down: return Drct_Down;
				case Key_Up: return Drct_Up;
				case Key_Left: return Drct_Left;
				case Key_Right: return Drct_Right;
			}
		}
		else if (inputLog[i].o == Op_Up) {
			released[inputLog[i].k] = true;
			
			bool allReleased = true;
			for (int i = 0; i < 4; i++) {
				if (!released[i]) {
					allReleased = false;
					break;
				}
			}

			if (allReleased) {
				return Drct_None;
			}
		}
	}
	return Drct_None;
}


bool is_floor(Room curRoom) {
	if (curDrct == Drct_Down) {
		printf("%c\n", matrix[CH01_POSY / 50 + 1][CH01_POSX / 50]);
		if (matrix[CH01_POSY / 50 + 1][CH01_POSX / 50] == '1')
			return true;
	}
	else if (curDrct == Drct_Left) {
		printf("%c\n", matrix[CH01_POSY / 50][(CH01_POSX - 5) / 50]);
		if (matrix[CH01_POSY / 50][CH01_POSX / 50 - 1] == '1')
			return true;
	}
	else if (curDrct == Drct_Right) {
		printf("%c\n", matrix[CH01_POSY / 50][CH01_POSX / 50 + 1]);
		if (matrix[CH01_POSY / 50][CH01_POSX / 50 + 1] == '1')
			return true;
	}
	else if (curDrct == Drct_Up) {
		printf("%c\n", matrix[(CH01_POSY - 5) / 50][CH01_POSX / 50]);
		if (matrix[CH01_POSY / 50 - 1][CH01_POSX / 50] == '1')
			return true;
	}
	else
		return false;
}


void move(int& curX, int& curY, int x, int y) {
	curX += x;
	curY += y;
}


bool onIntBlock(int x, int y) {
	return x % 50 == 0 && y % 50 == 0;
}


void attempMove(char matrix[10][15])
{
	//static Drct curDrct = Drct_None;
	if (curDrct == Drct_None) {
		curDrct = fetchDrct();
		if (!is_floor(matrix)) {
			curDrct = Drct_None;
			return;
		}
		if (curDrct == Drct_None)
			return;
	}

	
	switch (curDrct) {
		case Drct_Down:
			move(CH01_POSX, CH01_POSY, 0, 5);
			break;
		case Drct_Up:
			move(CH01_POSX, CH01_POSY, 0, -5);
			break;
		case Drct_Left:
			move(CH01_POSX, CH01_POSY, -5, 0);
			break;
		case Drct_Right:
			move(CH01_POSX, CH01_POSY, 5, 0);
			break;
	}

	if (onIntBlock(CH01_POSX, CH01_POSY)) {
		curDrct = Drct_None;
	}

	/*if (MAX_KEYS == 2) {
		if (TIMER_SWITCH != down && DIRECTION[1] == down && CH01_POSY % 50 != 0) {
			CH01_POSY += VELOCITY;
			return;
		}
		else if (TIMER_SWITCH != up && DIRECTION[1] == up && CH01_POSY % 50 != 0) {
			CH01_POSY += VELOCITY;
			return;
		}
		else if (TIMER_SWITCH != right && DIRECTION[1] == right && CH01_POSX % 50 != 0) {
			CH01_POSY += VELOCITY;
			return;
		}
		else if (TIMER_SWITCH != left && DIRECTION[1] == left && CH01_POSX % 50 != 0) {
			CH01_POSY -= VELOCITY;
			return;
		}
	}
	if (MAX_KEYS == 1) {
		if (UN_DOWN && CH01_POSY % 50 != 0){
			CH01_POSY += VELOCITY;
			if (CH01_POSY % 50 == 0) UN_DOWN = false;
			return;
		}
		//else if (TIMER_SWITCH != up && DIRECTION[1] == up && CH01_POSY % 50 != 0) {
		else if (UN_UP && CH01_POSY % 50 != 0) {
			CH01_POSY -= VELOCITY;
			if (CH01_POSY % 50 == 0) UN_UP = false;
			return;
		}
		//else if (TIMER_SWITCH != left && DIRECTION[1] == left && CH01_POSX % 50 != 0) {
		else if (UN_LEFT && CH01_POSX % 50 != 0) {
			CH01_POSX -= VELOCITY;
			if (CH01_POSX % 50 == 0) UN_LEFT = false;
			return;
		}
		//else if (TIMER_SWITCH != right && DIRECTION[1] == right && CH01_POSX % 50 != 0) {
		else if (UN_RIGHT && CH01_POSX % 50 != 0) {
			CH01_POSX += VELOCITY;
			if (CH01_POSX % 50 == 0) UN_RIGHT = false;
			return;
		}
	}
	

	if (TIMER_SWITCH == right) {
		if (matrix[CH01_POSY / 50][CH01_POSX / 50 + 1] == '1')
			CH01_POSX += VELOCITY;
		else{}
	}
	else if (TIMER_SWITCH == left) {
		if (matrix[CH01_POSY / 50][(CH01_POSX - 5) / 50] == '1')
			CH01_POSX -= VELOCITY;
		else {}
	}
	else if (TIMER_SWITCH == down) {
		if (matrix[CH01_POSY / 50 + 1][CH01_POSX / 50] == '1')
			CH01_POSY += VELOCITY;
		else {}
	}
	else if (TIMER_SWITCH == up) {
		if (matrix[(CH01_POSY - 5) / 50][CH01_POSX / 50] == '1')
			CH01_POSY -= VELOCITY;
		else {}
	}*/
}

///*
void show_map(char matrix[10][15], ImageData blocks[BLOCKNUM]) {
	int bg;
	int i = -50;
	int j = 0;
	for (int j = 0; j < 10; j++)
		for (int i = 0; i < 15; i++) {
			if (matrix[j][i] == '1') {
				imgRender(renderer, blocks[0], i * 50, j * 50, 0);
				//printf("put floor:(%d,%d)\t",i,j);
				//printf("put floor");
			}
			else if (matrix[j][i] == '2') {
				imgRender(renderer, blocks[1], i * 50, j * 50, 0);
				//printf("put wall");
			}
			else if (matrix[j][i] == '3') {
				imgRender(renderer, blocks[2], i * 50, j * 50, 0);
				//printf("put door");
			}
		}
	//SDL_RenderPresent(renderer);
}
//*/


// When using SDL, you have to use "int main(int argc, char* args[])"
// int main() and void main() are not allowed
int main(int argc, char* args[])
{
	// The working directory is where the vcxproj file is located.

	//еDид
	char ch01Path[100] = "../images/character01_pictures_37x53.png";
	int Index[3] = { 0,0,0 };
	
	//map
	/*
	char floor_path[100] = "../images/floor.jpg";
	char wall_path[100] = "../images/wall.jpg";
	char door_path[100] = "../images/door.jpg";
	*/
	//make blocks in an array, easier to pass to function
	char block_paths[BLOCKNUM][100] = { "../images/floor.jpg", "../images/wall.jpg", "../images/door.jpg" };
	char map_path[100] = "../map/default.txt";


	//ImageData ch01;//, sponge, kiss, pie;
	//ImageData floor, wall, door;
	ImageData blocks[BLOCKNUM];


	// Start up SDL and create window
	if (initSDL())
	{
		printf("Failed to initialize SDL!\n");
		return -1;
	}

	//ch01 = loadTexture(ch01Path, CH01_FRAME, 4, 4, true, 0xFF, 0xFF, 0xFF);
	ch01 = loadTexture(ch01Path, CH01_FRAME, 4, 4, false, NULL, NULL, NULL);
	/*
	floor = loadTexture(floor_path, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	wall = loadTexture(wall_path, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	door = loadTexture(door_path, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	*/
	int animationIndex[3] = { 0, 0, 0 };
	for (int x = 0; x < 3; x++) {
		blocks[x] = loadTexture(block_paths[x], 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	}


	FILE *map_default;
	errno_t err;
	err = fopen_s(&map_default, map_path, "r");
	if (map_default == NULL) {
		printf("fail to open map");
		return 1;
	}
	int i = 0, j = 0;
	char bg;
	char map_array[10][15];	// since reading the file only read once, so we need an matrix to save the data
	while ((bg = fgetc(map_default)) != EOF) {
		if (bg != ' ' && bg != '\n') {
			map_array[i][j] = bg;
			j++;
		}
		else if (bg == '\n') {
			i++; 
			j = 0;
		}
		else{}
	}

	SDL_TimerID timerID_ku = SDL_AddTimer(150, aniAction, &animationIndex);

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;		

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			// Handle input for the CH01
			handleEvent(e);
		}
		

		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		//show map
		//show_map(renderer, &map_default, blocks);
		show_map(map_array, blocks);
		//imgRender(renderer, blocks[0], 50, 50, 0);

		attempMove(map_array);
		//imgRender(renderer, ch01, CH01_POSX + (CH01_WIDTH - ch01.width) / 2, CH01_POSY + (CH01_HEIGHT - ch01.height) / 2, animationIndex[2]);
		imgRender(renderer, ch01, CH01_POSX + (CH01_WIDTH - ch01.width / 4) / 2, CH01_POSY + (CH01_HEIGHT - ch01.height / 4) / 4 * 3 - 50, animationIndex[2]);
		//imgRender(renderer, ch01, CH01_POSX + (CH01_WIDTH - ch01.width / 4) / 2, CH01_POSY-10 , animationIndex[2]);
		//imgRender(renderer, ch01, CH01_POSX, CH01_POSY, animationIndex[2]);
		//printf("CH01_WIDTH = %d, ch01.width = %d\n", CH01_WIDTH, ch01.width);
		//printf("CH01_HEIGHT = %d, ch01.height = %d\n", CH01_HEIGHT, ch01.height);

		// Update screen
		SDL_RenderPresent(renderer);
	}

	//Free resources and close SDL
	closeSDL();
	//close file
	fclose(map_default);

	return 0;  
}