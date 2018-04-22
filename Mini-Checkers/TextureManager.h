#pragma once
#include "Game.h"

//Texture managing class, for ease of use in Game class
class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);	//Texture loader
};

