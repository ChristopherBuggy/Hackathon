#ifndef Render_H
#define Render_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <SDL_thread.h>



class Render {
private:
	std::vector<SDL_Texture*> textures;
	std::vector<SDL_Rect> dstRects;
	TTF_Font *font = TTF_OpenFont("C:\\segoe.ttf", 28);
	SDL_Color textColor = { 0, 0, 0 };
	SDL_Surface* fpsSurface = TTF_RenderText_Solid(font, "0", textColor);
	SDL_Texture* fpsTex = NULL;
	SDL_Rect fpsRect;
	int renderIndex = 0;

public:
	SDL_Renderer *ren;
	SDL_Rect source;
	Render(SDL_Window *win) {
		dstRects.reserve(100);
		//creating a renderer that is linked to the render class 
		ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
		if (ren == nullptr) {
			SDL_DestroyWindow(win);
			QuitWithError("SDL_CreateRenderer Error: ");
		}
		//set background color
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	}

	void SetFps(float fps)
	{
		fpsSurface = TTF_RenderText_Solid(font, std::to_string((int)fps).c_str(), textColor);
		fpsTex = SDL_CreateTextureFromSurface(ren, fpsSurface);
		int iW, iH;
		SDL_QueryTexture(fpsTex, NULL, NULL, &iW, &iH);
		fpsRect.x = 20;
		fpsRect.y = 20;
		fpsRect.w = iW;
		fpsRect.h = iH;
	}

	void Start()
	{
		renderIndex = 0;
		SDL_RenderClear(ren);
	}

	void End(SDL_Rect src, SDL_Rect dst, SDL_Texture &leftTexture, SDL_Texture &rightTexture, SDL_Texture &stillTexture, int sprite, int dir, bool moving)
	{
		if (&leftTexture != NULL && &rightTexture != NULL && &stillTexture != NULL)
		{
			if (dir == 1 && dir != 2)
			{
				source = { sprite * 20, 0, 20, 40 };
				SDL_RenderCopy(ren, &leftTexture, &source, &dst);
			}

			if (dir == 2 && dir != 1)
			{
				source = { sprite * 20, 0, 20, 40 };
				SDL_RenderCopy(ren, &rightTexture, &source, &dst);
			}

		}

		if (moving == false)
		{
			source = { sprite * 20, 0, 20, 40 };
			SDL_RenderCopy(ren, &stillTexture, &source, &dst);
		}

		SDL_RenderPresent(ren);
	}

	bool RenderNext(int y)
	{
		if (renderIndex < textures.size()) {
			SDL_Rect rec;
			rec.x = dstRects.at(renderIndex).x;
			rec.y = dstRects.at(renderIndex).y - y + 500;
			rec.w = dstRects.at(renderIndex).w;
			rec.h = dstRects.at(renderIndex).h;
			SDL_RenderCopy(ren, textures[renderIndex], NULL, &rec);
			renderIndex++;
		}
		if (renderIndex >= textures.size())
			return true;
		return false;
	}

	void Update(SDL_Rect src, SDL_Rect dst, SDL_Texture &leftTexture, SDL_Texture &rightTexture, SDL_Texture &stillTexture, int sprite, int dir, bool moving, int y)
	{
		SDL_RenderClear(ren);

		int numberTextures = textures.size();
		for (int i = 0; i < numberTextures; i++) {
			SDL_Delay(1);
			SDL_Rect rec;
			rec.x = dstRects.at(i).x;
			rec.y = dstRects.at(i).y - y + 500;
			rec.w = dstRects.at(i).w;
			rec.h = dstRects.at(i).h;
			SDL_RenderCopy(ren, textures[i], NULL, &rec);
		}
		SDL_RenderCopy(ren, fpsTex, NULL, &fpsRect);

		if (&leftTexture != NULL && &rightTexture != NULL && &stillTexture != NULL)
		{
			if (dir == 1 && dir != 2)
			{
				source = { sprite * 20, 0, 20, 40 };
				SDL_RenderCopy(ren, &leftTexture, &source, &dst);
			}

			if (dir == 2 && dir != 1)
			{
				source = { sprite * 20, 0, 20, 40 };
				SDL_RenderCopy(ren, &rightTexture, &source, &dst);
			}

		}

		if (moving == false)
		{
			source = { sprite * 20, 0, 20, 40 };
			SDL_RenderCopy(ren, &stillTexture, &source, &dst);
		}

		SDL_RenderPresent(ren);
	}

	//void UpdateThreaded(void* data)
	//{
	//	SDL_RenderClear(ren);
	//	RenderData rd = *((RenderData *)data);
	//	int numberTextures = textures.size();
	//	for (int i = 0; i < 500; i++)
	//	for (int i = 0; i < numberTextures; i++) {
	//		SDL_Rect rec;
	//		rec.x = dstRects.at(i).x;
	//		rec.y = dstRects.at(i).y - rd.y + 500;
	//		rec.w = dstRects.at(i).w;
	//		rec.h = dstRects.at(i).h;
	//		SDL_RenderCopy(ren, textures[i], NULL, &rec);
	//	}
	//	SDL_RenderCopy(ren, fpsTex, NULL, &fpsRect);

	//	if (rd.leftTexture != NULL && rd.rightTexture != NULL && rd.stillTexture != NULL)
	//	{
	//		if (rd.dir == 1 && rd.dir != 2)
	//		{
	//			source = { rd.sprite * 20, 0, 20, 40 };
	//			SDL_RenderCopy(ren, rd.leftTexture, &source, &rd.dst);
	//		}

	//		if (rd.dir == 2 && rd.dir != 1)
	//		{
	//			source = { rd.sprite * 20, 0, 20, 40 };
	//			SDL_RenderCopy(ren, rd.rightTexture, &source, &rd.dst);
	//		}

	//	}

	//	if (rd.moving == false)
	//	{
	//		source = { rd.sprite * 20, 0, 20, 40 };
	//		SDL_RenderCopy(ren, rd.stillTexture, &source, &rd.dst);
	//	}

	//	SDL_RenderPresent(ren);
	//}


	void DrawMenuScene()
	{
		//clear the screen
		SDL_RenderClear(ren);

		//copy the textures to the renderer and give them a destination
		int numberTextures = textures.size();
		for (int i = 0; i < numberTextures; i++) {
			SDL_RenderCopy(ren, textures[i], NULL, &dstRects[i]);
		}

		//update the screen with rendering operations
		SDL_RenderPresent(ren);


	}

	void RemoveSurface(SDL_Surface surface)
	{

	}
	

	//adding texture to the renderer with the position
	SDL_Rect* AddSurfaceToRenderer(SDL_Surface *bmp, float x, float y, float scale) {
		//getting the width / height from the texture
		SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, bmp);
		SDL_FreeSurface(bmp);
		if (texture == nullptr) {
			QuitWithError("SDL_CreateTextureFromSurface Error: ");
		}
		int iW, iH;
		SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);

		SDL_Rect dstRect;
		dstRect.x = x;
		dstRect.y = y;
		dstRect.w = iW * scale;
		dstRect.h = iH * scale;

		int rectPos = dstRects.size();
		dstRects.push_back(dstRect);
		textures.push_back(texture);

		return &dstRects[rectPos];
	}

	//rendering texture
	SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
		SDL_Color color, int fontSize, SDL_Renderer *renderer)
	{
		//Open the font
		TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
		if (font == nullptr) {
			logSDLError("TTF_OpenFont");
			return nullptr;
		}
		//We need to first render to a surface as that's what TTF_RenderText
		//returns, then load that surface into a texture
		SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
		if (surf == nullptr) {
			TTF_CloseFont(font);
			logSDLError("TTF_RenderText");
			return nullptr;
		}
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
		if (texture == nullptr) {
			logSDLError("CreateTexture");
		}
		//Clean up the surface and font
		SDL_FreeSurface(surf);
		TTF_CloseFont(font);
		return texture;
	}

	void logSDLError(const std::string &msg) {
		//std::cout << msg << " error: " << SDL_GetError() << std::endl;

	}

	int QuitWithError(const std::string &msg) {
		//std::cout << msg << " error: " << SDL_GetError() << std::endl;
		SDL_Delay(1000); //give user a chance to read error
		SDL_Quit();
		return 1;
	}

};
#endif