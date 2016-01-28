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
#include <InputHandler.h>   
#include <Render.h>
#include <Player.h>
#include <Level.h>
#include <vector>
#include <Fireball.h>
#include <MenuScene.h>
#include <Cannon.h>
#include <Button.h>
#include <Door.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>

static const float SCALE = 30.f;


void logSDLError(const std::string &msg) {
	//std::cout << msg << " error: " << SDL_GetError() << std::endl;
}



static SDL_mutex* mutex = SDL_CreateMutex();

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
struct RenderData
{
	Render* ren;
	int y;
	RenderData(Render* r, int yPos) : ren(r), y(yPos) {}
};

std::vector<b2Vec2> timeArray(60);

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
InputHandler inputHandler = InputHandler();
SDL_sem *sem = NULL;

int threadFunction(void* data)
{
	RenderData rd = *((RenderData *)data);
	bool drawing = true;
	while (drawing)
	{
		SDL_Delay(1);
		SDL_SemWait(sem);
		drawing = !rd.ren->RenderNext(rd.y);
		SDL_SemPost(sem);
	}
	return 0;
}

int main(int, char**) {


	sem = SDL_CreateSemaphore(1);

	//initialise SDL system and the video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		QuitWithError("SDL_Init Error: ");

	}

	//Create a Window
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		QuitWithError("SDL_CreateWindow Error: ");

	}

	if (TTF_Init() == -1)
	{
		QuitWithError("TTF_Init Error: ");
	}

	//SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);

	Mix_Music *music = NULL;
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1;
	std::string basepath(SDL_GetBasePath());
	music = Mix_LoadMUS((basepath + "background.wav").c_str());
	Mix_PlayMusic(music, -1);

	Render* renderer = new Render(win);

	//Main loop flag
	bool quit = false;
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);

	string bgPath = basepath + "background2.bmp";
	SDL_Surface* back = SDL_LoadBMP(bgPath.c_str());
	SDL_Rect* backGroundRect = renderer->AddSurfaceToRenderer(back, 0, -600, 1.0f);

	Level level = Level(World, renderer);
	Button button = Button(880, 39, World, renderer);
	Door door = Door(900, 0, renderer);
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
	cannons.push_back(new Cannon(170, 358, World, renderer, 1));
	cannons.push_back(new Cannon(1010, 158, World, renderer, 2));
	MenuScene* menu = new MenuScene(1200, 100, renderer);
	SDL_Event e;
	bool threading = false;
	std::vector<float> fps;
	uint32 lastTicks = 0;
	SDL_Thread* thread1 = 0;
	SDL_Thread* thread2 = 0;

	//game loop
	while (!quit) {

		World.Step(1 / 60.f, 6, 2);
		while (SDL_PollEvent(&e) != 0) {
			if (inputHandler.CheckInput(SDLK_ESCAPE, e)) {
				quit = true;
			}
			if (inputHandler.CheckInput(SDLK_t, e)) {
				threading = !threading;
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

			SDL_Rect rec(player.spriteRect);
			rec.y = player.GetY();

			for (int j = 0; j < cannons.size(); j++)
			{
				for (int i = 0; i < cannons.at(j)->fireballs.size(); i++)
				{
					if (cannons.at(j)->fireballs.at(i)->CheckCollision(&rec) == true)
					{
						std::cout << "Collision Detected!" << std::endl;
						player.Respawn();
						button.setOnce(false);
						button.buttonBody->SetTransform(b2Vec2(880 / SCALE, 39 / SCALE), 0);
						door.spriteRect->x = -1000;
						door.spriteRect->y = -1000;
						player.prevPosX.clear();
						player.prevPosY.clear();
						player.count = 0;
					}
				}
			}

			button.Update();

			if (button.CheckCollision(&rec) == true)
			{
				std::cout << "Collision Detected!" << std::endl;
				button.collision = true;
				button.spriteRect->x = -2000;
				button.spriteRect->y = -2000;
				button.buttonBody->SetTransform(b2Vec2(-2000, -2000), 0);
				//door.Draw(renderer);
			}
			if (door.CheckCollision(&rec) == true)
			{
				button.buttonBody->SetTransform(b2Vec2(880 / SCALE, 39 / SCALE), 0);
				std::cout << "Collision Detected!" << std::endl;
				player.Respawn();
				button.setOnce(false);
				door.spriteRect->x = -1000;
				door.spriteRect->y = -1000;
				player.prevPosX.clear();
				player.prevPosY.clear();
				player.count = 0;
				menu->playBool = false;
				menu->quitBool = false;
				menu->backGroundRect->x = 0;
				menu->current = 0;
				button.collision = false;
			}
			if (button.collision == false)
			{
				door.DrawCage(renderer);
			}
			if (button.collision == true)
			{
				door.DrawNoCage(renderer);
			}

			int ticks = SDL_GetTicks();
			int seconds = ticks / 50;
			int sprite = seconds % 8;

			int ticksSince = ticks - lastTicks;
			fps.insert(fps.begin(), ticksSince == 0 ? 1000 : (1000 / (ticks - lastTicks)));
			if (fps.size() > 6)
				fps.pop_back();

			float fpsAvg = 0;
			for each (float f in fps)
			{
				fpsAvg += f;
			}
			fpsAvg /= fps.size();

			lastTicks = ticks;

			SDL_SetWindowTitle(win, std::to_string((int)fps[0]).c_str());
			//renderer->SetFps(fpsAvg);

			renderer->Start();

			if (thread1 != 0)
			{
				SDL_WaitThread(thread1, NULL);
				thread1 = 0;
			}
			if (thread2 != 0)
			{
				SDL_WaitThread(thread1, NULL);
				thread2 = 0;
			}
			if (threading)
			{
				renderer->End(player.srcRect, player.dstRect, *player.LeftTexture, *player.RightTexture, *player.StandTexture, sprite, dir, player.moving);
				renderer->Start();
				thread1 = SDL_CreateThread(threadFunction, "thread1", &RenderData(renderer, player.GetY()));
				thread2 = SDL_CreateThread(threadFunction, "thread2", &RenderData(renderer, player.GetY()));
			}
			else
				renderer->Update(player.srcRect, player.dstRect, *player.LeftTexture, *player.RightTexture, *player.StandTexture, sprite, dir, player.moving, player.GetY());

			player.dstRect.w = player.spriteRect.w;
			player.dstRect.h = player.spriteRect.h;
			player.dstRect.x = player.spriteRect.x;
			player.dstRect.y = player.spriteRect.y + 5;
		}

		if (menu->quitBool == true)
		{
			quit = true;
		}
	}
	SDL_DestroyMutex(mutex);
	SDL_DestroyRenderer(renderer->ren);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
