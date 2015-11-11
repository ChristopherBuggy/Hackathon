#include "Cannon.h"

Cannon::Cannon(float x, float y, Render* renderer)
{
	initX = x;
	initY = y;

	std::string basepath(SDL_GetBasePath());

	std::string imagePath = basepath + "cannon.bmp";
	sprite = SDL_LoadBMP(imagePath.c_str());

	if (spriteRect == NULL)
	{
		spriteRect = renderer->AddSurfaceToRenderer(sprite, initX, initY, 0.5f);
	}

	else
	{

	}
}