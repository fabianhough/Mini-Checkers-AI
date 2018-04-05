#include "Game.h"
#include "TextureManager.h"

SDL_Texture* boardTex;
SDL_Texture* playerPieceTex;
SDL_Texture* AIPieceTex;


SDL_Rect** destRA;


Game::Game()
{
}


Game::~Game()
{
}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;


	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialized..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
			std::cout << "Window Created..." << std::endl;

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created..." << std::endl;
		}

		isRunning = true;
	}
	else
		isRunning = false;

	boardTex = TextureManager::LoadTexture("assets/cboard.png", renderer);
	playerPieceTex = TextureManager::LoadTexture("assets/BlackPiece.png", renderer);
	AIPieceTex = TextureManager::LoadTexture("assets/WhitePiece.png", renderer);


	destRA = new SDL_Rect*[6];
	for (int i = 0; i < 6; i++)
		destRA[i] = new SDL_Rect[6];

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			destRA[i][j].w = 120;
			destRA[i][j].h = 120;
			destRA[i][j].x = j*120;
			destRA[i][j].y = i*120;
		}
	}
	

}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::mouseSelect(int &x, int &y)
{
	SDL_Event event;
	while (1)
	{
		SDL_PollEvent(&event);

		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			int tempx, tempy;
			SDL_GetMouseState(&tempx, &tempy);

			x = tempx / 120;
			y = tempy / 120;

			break;
		}
		else if (event.type == SDL_QUIT)
		{
			isRunning = false;
			break;
		}
	}
}

void Game::update()
{
	
}

void Game::render(cBoard cgame)
{
	SDL_RenderClear(renderer);
	//Add things to render
	SDL_RenderCopy(renderer, boardTex, NULL, NULL);

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (cgame.get_index(i, j) == 'B')
				SDL_RenderCopy(renderer, playerPieceTex, NULL, &destRA[i][j]);
			else if (cgame.get_index(i, j) == 'W')
				SDL_RenderCopy(renderer, AIPieceTex, NULL, &destRA[i][j]);
		}
	}
	
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}