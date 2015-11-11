#pragma once
#ifndef FIREBALL_H
#define FIREBALL_H

using namespace std;
#include<iostream>
#include<Box2D\Box2D.h>
#include<SDL.h>
#include "Render.h"

class Fireball
{
public:
	Fireball(float x, float y, b2World& World, Render* renderer);
	void Update(int dir);
	void CreateBody();
	void RemoveSurface();
	bool CheckLife();
	bool CheckCollision(SDL_Rect* playerRect);


private:
	b2World* world;
	b2Body* fireballBody;
	SDL_Surface * sprite;

	float initX = 0;
	float initY = 0;

	SDL_Rect* spriteRect;
};

#endif

