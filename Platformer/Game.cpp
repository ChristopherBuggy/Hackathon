/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <Box2D/Box2D.h>
#include <stdio.h>
#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h>
#include <InputHandler.h>       // Added by Alison
#include <Render.h> //Added by Jonathan
#include <Player.h>
#include <Level.h>
#include <vector>
#include <Fireball.h>
#include <MenuScene.h>
#include <Cannon.h>
#include <Button.h>
#include <Door.h>

static const float SCALE = 30.f;


void logSDLError(const std::string &msg) {
	//std::cout << msg << " error: " << SDL_GetError() << std::endl;
}



int QuitWithError(const std::string &msg) {
	//std::cout << msg << " error: " << SDL_GetError() << std::endl;
	SDL_Delay(1000); //give user a chance to read error
	SDL_Quit();
	return 1;
}


/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/

std::vector<b2Vec2> timeArray(60);

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
InputHandler inputHandler = InputHandler();

int main(int, char**) {


	//initialise SDL system and the video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		QuitWithError("SDL_Init Error: ");

	}

	//Create a Window
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		QuitWithError("SDL_CreateWindow Error: ");

	}

	Render* renderer = new Render(win);

	//Main loop flag
	bool quit = false;
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);

	Level level = Level(World, renderer);
	Button button = Button(195, 338, World, renderer);
	Door door = Door(1125, 219, renderer);
	Player player = Player(100, 500, World, renderer);
	//vector<Fireball*> fireballs;
	//fireballs.push_back(new Fireball(825, 379, World, renderer));
	//fireballs.push_back(new Fireball(550, 379, World, renderer));
	//fireballs.push_back(new Fireball(275, 379, World, renderer));
	//fireballs.push_back(new Fireball(0, 379, World, renderer));
	//Fireball* fireball1 = new Fireball(825, 379, World, renderer);
	//Fireball* fireball2 = new Fireball(550, 379, World, renderer);
	//Fireball* fireball3 = new Fireball(275, 379, World, renderer);
	//Fireball* fireball4 = new Fireball(0, 379, World, renderer);
	vector<Cannon*> cannons;
	cannons.push_back(new Cannon(810, 358, World, renderer, 2));
	cannons.push_back(new Cannon(200, 200, World, renderer, 1));
	MenuScene* menu = new MenuScene(1200, 100, renderer);
	SDL_Event e;

	//game loop
	while (!quit) {

		World.Step(1 / 60.f, 6, 2);
		while (SDL_PollEvent(&e) != 0) {
			if (inputHandler.CheckInput(SDLK_ESCAPE, e)) {
				quit = true;
			}
		}
		if (menu->playBool == false && menu->quitBool == false) {
			renderer->DrawMenuScene();
			menu->Update(renderer);
		}
		
		if (menu->playBool == true)
		{
			//PLAY GAME STATE
			int dir = player.Move(inputHandler, e);

			for (int i = 0; i < cannons.size(); i++)
			{
				cannons.at(i)->Update();
			}

			for (int j = 0; j < cannons.size(); j++)
			{
				for (int i = 0; i < cannons.at(j)->fireballs.size(); i++)
				{
					if (cannons.at(j)->fireballs.at(i)->CheckCollision(player.spriteRect) == true)
					{
						std::cout << "Collision Detected!" << std::endl;
						player.Respawn();
						button.setOnce(false);
						button.buttonBody->SetTransform(b2Vec2(195 / SCALE, 338 / SCALE), 0);
						door.spriteRect->x = -1000;
						door.spriteRect->y = -1000;
						player.prevPosX.clear();
						player.prevPosY.clear();
						player.count = 0;
					}
				}
			}

			
			button.Update();

			
			if (button.CheckCollision(player.spriteRect) == true)
			{
				std::cout << "Collision Detected!" << std::endl;
				button.spriteRect->x = -2000;
				button.spriteRect->y = -2000;
				button.buttonBody->SetTransform(b2Vec2(-2000, -2000), 0);
				door.Draw(renderer);
			}
			if (door.CheckCollision(player.spriteRect) == true)
			{
				std::cout << "Collision Detected!" << std::endl;
				player.Respawn();
				button.setOnce(false);
				door.spriteRect->x = -1000;
				door.spriteRect->y = -1000;
				button.buttonBody->SetTransform(b2Vec2(195 / SCALE, 338 / SCALE), 0);
				player.prevPosX.clear();
				player.prevPosY.clear();
				player.count = 0;
				menu->playBool = false;
				menu->quitBool = false;
				menu->backGroundRect->x = 0;
				menu->current = 0;
			}

			int ticks = SDL_GetTicks();
			int seconds = ticks / 50;
			int sprite = seconds % 8;
			renderer->Update(player.srcRect, player.dstRect, *player.LeftTexture, *player.RightTexture, *player.StandTexture, sprite, dir, player.moving);
			player.dstRect.w = player.spriteRect->w;
			player.dstRect.h = player.spriteRect->h;
			player.dstRect.x = player.spriteRect->x;
			player.dstRect.y = player.spriteRect->y + 5;
		}

		if (menu->quitBool == true)
		{
			quit = true;
		}
	}
	SDL_DestroyRenderer(renderer->ren);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
