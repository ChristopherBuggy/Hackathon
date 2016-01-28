#pragma once
#ifndef FIREBALL_H
#define FIREBALL_H

using namespace std;
#include<iostream>
#include<Box2D\Box2D.h>
#include<SDL.h>
#include <SDL_thread.h>
#include "Render.h"

class Fireball
{
public:
	Fireball(float x, float y, b2World& World, Render* renderer, float dir);
	void Update(int dir);
	void CreateBody();
	void RemoveSurface();
	bool CheckLife();
	bool CheckCollision(SDL_Rect* playerRect);
	b2Body* fireballBody;
	float initX = 0;
	SDL_mutex* mut;
	int move(void* ptr);


private:
	b2World* world;
	SDL_Surface * sprite;
	float dir;
	float initY = 0;

	SDL_Rect* spriteRect;
};

#endif

