#ifndef Platform_H
#define Platform_H

using namespace std;

#include<iostream>
#include "SDL.h"
#include "Box2D/Box2D.h"
#include "Render.h"

class Platform
{
private:

public:
	Platform(int x, int y, int width, int height, b2World& World, Render* renderer);
	float x;
	float y;
	int width;
	int height;
	SDL_Surface *image;
	SDL_Texture *imageTexture;
	SDL_Rect imageRect;

	SDL_Rect* spriteRect;

	void CreateBody();

	b2Body* playerBody;

	b2World* world;

	void Update();
};


#endif
