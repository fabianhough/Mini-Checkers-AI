#pragma once
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "cBoard.h"
#include "TextureManager.h"
#undef main

class Game
{
private:
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;

public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void mouseSelect(int &x, int &y);
	void update();
	void render(cBoard *cgame);
	void renderMoves(cBoard *cgame, bool player, int x, int y);
	void renderJumps(cBoard *cgame, bool player);
	void clean();

	bool running() { return isRunning; }



};

