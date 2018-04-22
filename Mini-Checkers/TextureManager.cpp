#include "TextureManager.h"

//Texture loader
//Loads textures from image file into a texture object
SDL_Texture* TextureManager::LoadTexture(const char* fileName, SDL_Renderer* ren)
{
	SDL_Surface* tempSurface = IMG_Load(fileName);						//Temporary surface to 'paste' image file
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);	//Creates a texture object using image file
	SDL_FreeSurface(tempSurface);										//Empties temporary surface

	return tex;															//Returns new texture object
}
