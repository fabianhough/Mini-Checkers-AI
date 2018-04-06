#include "Game.h"
#include "TextureManager.h"

SDL_Texture* boardTex;
SDL_Texture* playerPieceTex;
SDL_Texture* AIPieceTex;
SDL_Texture* validSpaceTex;


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
	validSpaceTex = TextureManager::LoadTexture("assets/ValidSquare.png", renderer);


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

			return;
		}
		else if (event.type == SDL_QUIT)
		{
			isRunning = false;
			clean();
			exit(0);
			return;
		}
	}
}

void Game::update()
{
	
}

void Game::render(cBoard *cgame)
{
	SDL_RenderClear(renderer);
	//Add things to render
	SDL_RenderCopy(renderer, boardTex, NULL, NULL);

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (cgame->get_index(i, j) == 'B')
				SDL_RenderCopy(renderer, playerPieceTex, NULL, &destRA[i][j]);
			else if (cgame->get_index(i, j) == 'W')
				SDL_RenderCopy(renderer, AIPieceTex, NULL, &destRA[i][j]);
		}
	}
	
	SDL_RenderPresent(renderer);
}

void Game::renderMoves(cBoard *cgame, bool player, int x, int y)
{
	SDL_RenderClear(renderer);
	//Add things to render
	SDL_RenderCopy(renderer, boardTex, NULL, NULL);

	SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[y][x]);

	int posxl, posxr, posxl2, posxr2;
	int posy1, posy2;
	if (player)
	{
		posy1 = y - 1;
		posy2 = y - 2;
	}
	else
	{
		posy1 = y + 1;
		posy2 = y + 2;
	}
	posxl = x - 1;
	posxr = x + 1;
	posxl2 = x - 2;
	posxr2 = x + 2;

	if (cgame->validMove(player, x, y, posxl, posy1))
		SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[posy1][posxl]);
	if (cgame->validMove(player, x, y, posxr, posy1))
		SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[posy1][posxr]);
	if (cgame->validMove(player, x, y, posxl2, posy2))
		SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[posy2][posxl2]);
	if (cgame->validMove(player, x, y, posxr2, posy2))
		SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[posy2][posxr2]);


	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (cgame->get_index(i, j) == 'B')
				SDL_RenderCopy(renderer, playerPieceTex, NULL, &destRA[i][j]);
			else if (cgame->get_index(i, j) == 'W')
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