#include <InputHandler.h>

InputHandler::InputHandler() { }

InputHandler::~InputHandler() { }

//E.g - void gameLoop() {
//			if (inputHandler.CheckInput(SDLK_a))
//				move player left 
bool InputHandler::CheckInput(SDL_Keycode key, SDL_Event event) {
		if (event.key.keysym.sym == key && event.type == SDL_KEYDOWN)
			return true;
		else
			return false;
}