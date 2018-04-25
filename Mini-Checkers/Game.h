#pragma once
#include <iostream>
#include "SDL.h"				//SDL2 Header
#include "SDL_image.h"			//Header for using images
#include "cBoard.h"				//cBoard class
#include "TextureManager.h"		//Engine image loader
#undef main

//Game Engine class
class Game
{
private:
	bool isRunning;				//Game state is currently running
	SDL_Window *window;			//Game Engine window
	SDL_Renderer *renderer;		//Game Engine renderer

public:
	Game();		//Constructor
	~Game();	//Destructor

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);	//Initializes Game Engine

	SDL_Window* get_window() { return window; }						//Returns the Game window object

	void mouseSelect(int &x, int &y);								//Allows player to select piece on board

	void render(cBoard *cgame);										//Renders the window
	void renderMoves(cBoard *cgame, bool player, int x, int y);		//Renders the moves available to a selected piece
	void renderJumps(cBoard *cgame, bool player);					//Renders all jump moves available
	void clean();													//Cleans the Game Engine and window

	bool running() { return isRunning; }							//Validates that the Game Engine is running
};

