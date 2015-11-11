#ifndef DOOR_H
#define DOOR_H
using namespace std;
#include<iostream>
#include<SDL.h>
#include "Render.h"

class Door
{
private:
	
	float initX = 0;
	float initY = 0;
	SDL_Surface * sprite;

public:
	Door(float x, float y, Render* renderer);
	SDL_Rect* spriteRect;
	bool open = false;
	void Draw(Render* renderer);
	bool CheckCollision(SDL_Rect* playerRect);
};

#endif
