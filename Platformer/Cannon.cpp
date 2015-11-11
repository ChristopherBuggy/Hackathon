#include "Cannon.h"
static const float SCALE = 30.f;

Cannon::Cannon(float x, float y, b2World& World, Render* renderer, float d)
{
	initX = x;
	initY = y;

	dir = d;

	std::string basepath(SDL_GetBasePath());
	std::string imagePath;

	if (dir == 1)
	{
		imagePath = basepath + "cannon2.bmp";
	}
	else
	{
		imagePath = basepath + "cannon.bmp";
	}

	sprite = SDL_LoadBMP(imagePath.c_str());

	if (spriteRect == NULL)
	{
		spriteRect = renderer->AddSurfaceToRenderer(sprite, initX, initY, 0.5f);
	}

	else
	{

	}

	fireballs.push_back(new Fireball(825, y - 15, World, renderer));
	fireballs.push_back(new Fireball(550, y - 15, World, renderer));
	fireballs.push_back(new Fireball(275, y - 15, World, renderer));
	fireballs.push_back(new Fireball(0, y - 15, World, renderer));
}

void Cannon::Update()
{
	for (int i = 0; i < fireballs.size(); i++)
	{
		fireballs.at(i)->Update(dir);
		if (fireballs.at(i)->CheckLife())
		{
			if (dir == 2)
			{
				fireballs.at(i)->fireballBody->SetTransform(b2Vec2(initX / SCALE, (initY - 15) / SCALE), 0); // = new Fireball(825, 379, World, renderer);
			}
			if (dir == 1)
			{
				fireballs.at(i)->fireballBody->SetTransform(b2Vec2(initX / SCALE, (initY - 15) / SCALE), 0);//new Fireball(-275, 379, World, renderer);
			}
		}
	}
}