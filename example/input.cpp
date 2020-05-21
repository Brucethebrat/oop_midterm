#include "input.h"


void handleEvent(SDL_Event& e)
{
	// The method for "Debunce"(反彈跳時間) 

	// If a key was pressed
	// repeat: non-zero if this is a key repeat
	// https://wiki.libsdl.org/SDL_KeyboardEvent
	//if (e.type == SDL_KEYDOWN)
	/*if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{

		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
			case SDLK_UP:
				//if ((CH01_POSY <= 0) || (CH01_POSY + CH01_HEIGHT >= HEIGHT)) //53 是CH01的實際高度，留一點空間在CH01與底部之間
				//	TIMER_SWITCH = false; break;
				TIMER_SWITCH = up;
				PRESS_KEY_NUM += 1;
				MAX_KEYS += 1;
				UN_UP = false;
				DIRECTION[1] = DIRECTION[0];
				DIRECTION[0] = up;
				//printf("uping SWITCH = %d\n", TIMER_SWITCH);
				break;
			case SDLK_DOWN:
				//if ((CH01_POSY <= 0) || (CH01_POSY + CH01_HEIGHT >= HEIGHT)) //53 是CH01的實際高度，留一點空間在CH01與底部之間
				//	TIMER_SWITCH = false; break;
				TIMER_SWITCH = down;
				PRESS_KEY_NUM += 1;
				MAX_KEYS += 1;
				UN_DOWN = false;
				DIRECTION[1] = DIRECTION[0];
				DIRECTION[0] = down;
				break;
			case SDLK_LEFT:
				TIMER_SWITCH = left;
				PRESS_KEY_NUM += 1;
				MAX_KEYS += 1;
				UN_LEFT = false;
				DIRECTION[1] = DIRECTION[0];
				DIRECTION[0] = left;
				break;
			case SDLK_RIGHT:
				TIMER_SWITCH = right;
				PRESS_KEY_NUM += 1;
				MAX_KEYS += 1;
				UN_RIGHT = false;
				DIRECTION[1] = DIRECTION[0];
				DIRECTION[0] = right;
				break;
		}
	}
	//If a key was released
	//else if (e.type == SDL_KEYUP)
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
			case SDLK_UP:
				TIMER_SWITCH = none;
				PRESS_KEY_NUM -= 1;
				UN_UP = true;
				DIRECTION[1] = none;//DIRECTION[0];
				break;//CH01_VELY += VELOCITY; break;
			case SDLK_DOWN:
				TIMER_SWITCH = none;
				PRESS_KEY_NUM -= 1;
				UN_DOWN = true;
				DIRECTION[1] = none;//DIRECTION[0];
				break;// CH01_VELY -= VELOCITY; break;
			case SDLK_LEFT:
				TIMER_SWITCH = none;
				PRESS_KEY_NUM -= 1;
				UN_LEFT = true;
				DIRECTION[1] = none;//DIRECTION[0];
				break;// CH01_VELX += VELOCITY; break;
			case SDLK_RIGHT:
				TIMER_SWITCH = none;
				PRESS_KEY_NUM -= 1;
				UN_RIGHT = true;
				DIRECTION[1] = none;//DIRECTION[0];
				break;// CH01_VELX -= VELOCITY; break;
		}
		if (PRESS_KEY_NUM == 0) MAX_KEYS = 0;
	}*/
	if (e.key.repeat != 0)	return;
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_UP:
			inputLog.push_back(InputEvent{ Key_Up, Op_Down });
			break;
		case SDLK_DOWN:
			inputLog.push_back(InputEvent{ Key_Down, Op_Down });
			break;
		case SDLK_LEFT:
			inputLog.push_back(InputEvent{ Key_Left, Op_Down });
			break;
		case SDLK_RIGHT:
			inputLog.push_back(InputEvent{ Key_Right, Op_Down });
			break;
		}
	}
	else if (e.type == SDL_KEYUP) {
		switch (e.key.keysym.sym) {
		case SDLK_UP:
			inputLog.push_back(InputEvent{ Key_Up, Op_Up });
			break;
		case SDLK_DOWN:
			inputLog.push_back(InputEvent{ Key_Down, Op_Up });
			break;
		case SDLK_LEFT:
			inputLog.push_back(InputEvent{ Key_Left, Op_Up });
			break;
		case SDLK_RIGHT:
			inputLog.push_back(InputEvent{ Key_Right, Op_Up });
			break;
		}
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
