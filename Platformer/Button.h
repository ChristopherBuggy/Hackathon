#ifndef BUTTON_H
#define BUTTON_H
using namespace std;
#include<iostream>
#include<Box2D\Box2D.h>
#include<SDL.h>
#include "Render.h"

class Button
{
private:
	b2World* world;
	b2Body* buttonBody;
	SDL_Rect* spriteRect;
	float initX = 0;
	float initY = 0;
	bool onlyOnce;

public:
	Button(float x, float y, b2World& World, Render* renderer);
	void CreateBody();
	bool CheckCollision(SDL_Rect* playerRect);
	void Update();
	void setOnce(bool b);
};

#endif