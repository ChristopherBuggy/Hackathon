
#include "Platform.h"
#include "Render.h"
#include <iostream>
using namespace std;

static const float SCALE = 30.f;

Platform::Platform(int x1, int y1, int width1, int height1, b2World& World, Render* renderer)
{
	x = x1;
	y = y1;
	width = width1;
	height = height1;
	imageRect.x = x;
	imageRect.y = y;
	imageRect.w = width;
	imageRect.h = height;
	world = &World;

	SDL_Surface * sprite;
	std::string basepath(SDL_GetBasePath());

	std::string imagePath = basepath + "platform.bmp";
	sprite = SDL_LoadBMP(imagePath.c_str());
	spriteRect = renderer->AddSurfaceToRenderer(sprite, x, y, 1);
	spriteRect->w = width;
	spriteRect->h = height;
	CreateBody();

	spriteRect->x = ((playerBody->GetPosition().x) * SCALE) - 38;
	spriteRect->y = ((playerBody->GetPosition().y) * SCALE) + 19;
}

void Platform::CreateBody()
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(x / SCALE, y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = world->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((100.f / 2) / SCALE, (10.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);

	playerBody = Body;
}

void Platform::Update()
{
	imageRect.x = playerBody->GetPosition().x;
	imageRect.y = playerBody->GetPosition().y;
	imageRect.w = 100;
	imageRect.h = 10;
}


//void Platform::Update()
//{
//	imageRect.x = ((playerBody->GetPosition().x) * SCALE) - 23;
//	imageRect.y = ((playerBody->GetPosition().y) * SCALE) + 19;
//	imageRect.w = 100;
//	imageRect.h = 10;
//}