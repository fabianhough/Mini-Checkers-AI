#include "Game.h"

//Textures for all images
SDL_Texture* boardTex;			//Board
SDL_Texture* playerPieceTex;	//Human Pieces - Black
SDL_Texture* AIPieceTex;		//AI Pieces- White
SDL_Texture* validSpaceTex;		//Texture for all valid move areas
SDL_Texture* jumpSpaceTex;		//Texture for jump moves that must be taken


SDL_Rect** destRA;				//Destination area for textures, matches board

//Constructor, doesn't do anything
Game::Game()
{
}

//Destructor, doesn't do anything
Game::~Game()
{
}

//Initializes Game Engine, and window
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;							//Flag for fullscreen indication

	//Sets screen to fullsize
	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;

	//Initilizes everything if not initialized already
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialized..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);		//Creates new game window
		if (window)
			std::cout << "Window Created..." << std::endl;

		renderer = SDL_CreateRenderer(window, -1, 0);							//Creates texture renderer

		//Renderer is active
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);				//Sets limits of renderer
			std::cout << "Renderer Created..." << std::endl;
		}

		isRunning = true;														//Game is running
	}
	//Failed initializing engine
	else
		isRunning = false;														//Game is not running

	//Initializes all textures
	boardTex = TextureManager::LoadTexture("assets/cboard.png", renderer);
	playerPieceTex = TextureManager::LoadTexture("assets/BlackPiece.png", renderer);
	AIPieceTex = TextureManager::LoadTexture("assets/WhitePiece.png", renderer);
	validSpaceTex = TextureManager::LoadTexture("assets/ValidSquare.png", renderer);
	jumpSpaceTex = TextureManager::LoadTexture("assets/JumpSquare.png", renderer);


	destRA = new SDL_Rect*[6];					//Initializes destination areas
	for (int i = 0; i < 6; i++)
		destRA[i] = new SDL_Rect[6];

	//Initializes destination areas to match size of window
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

//Allows player to select a space on the board
void Game::mouseSelect(int &x, int &y)
{
	SDL_Event event;								//Event handler
	//Continues until valid input is received
	while (1)
	{
		SDL_PollEvent(&event);						//Checks queue of events

		//Player has clicked on the screen
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			int tempx, tempy;						//Coordinates of mouse
			SDL_GetMouseState(&tempx, &tempy);		//Sets coordinates of click area

			x = tempx / 120;						//Updates pointers to game for board variable
			y = tempy / 120;

			return;									//Returns once valid input is given
		}
		//Exit button has been pressed
		else if (event.type == SDL_QUIT)
		{
			isRunning = false;						//Game is not running anymore
			clean();								//Cleans Game Engine
			//exit(0);
			return;									//Returns
		}
	}
}

//Renders current game board
void Game::render(cBoard *cgame)
{
	SDL_RenderClear(renderer);							//Clears all render objects
	SDL_RenderCopy(renderer, boardTex, NULL, NULL);		//Initializes renderer again with board

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			//Adds human piece to renderer if piece is found
			if (cgame->get_index(i, j) == 'B')
				SDL_RenderCopy(renderer, playerPieceTex, NULL, &destRA[i][j]);
			//Adds AI piece to renderer if piece is found
			else if (cgame->get_index(i, j) == 'W')
				SDL_RenderCopy(renderer, AIPieceTex, NULL, &destRA[i][j]);
		}
	}
	
	SDL_RenderPresent(renderer);						//Renders all textures
}

//Renders current game board, with all valid moves shown after player has selected valid piece
void Game::renderMoves(cBoard *cgame, bool player, int x, int y)
{
	SDL_RenderClear(renderer);										//Clears renderer
	SDL_RenderCopy(renderer, boardTex, NULL, NULL);					//Initializes board texture
	SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[y][x]);	//Initializes valid space texture on chosen piece

	int posxl, posxr, posxl2, posxr2;		//New x positions
	int posy1, posy2;						//New y positions

	//Sets values of all new position directions based on current piece selected
	//Human
	if (player)
	{
		posy1 = y - 1;
		posy2 = y - 2;
	}
	//AI
	else
	{
		posy1 = y + 1;
		posy2 = y + 2;
	}
	posxl = x - 1;
	posxr = x + 1;
	posxl2 = x - 2;
	posxr2 = x + 2;

	//Jump moves available
	if (cgame->availJump(player))
	{
		//Jump in the left direction, adds valid space texture
		if (cgame->validMove(player, x, y, posxl2, posy2))
			SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[posy2][posxl2]);
		//Jump in the right direction, adds valid space texture
		if (cgame->validMove(player, x, y, posxr2, posy2))
			SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[posy2][posxr2]);
	}
	//Jump moves not available
	else
	{
		//Move in the left direction, adds valid space texture
		if (cgame->validMove(player, x, y, posxl, posy1))
			SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[posy1][posxl]);
		//Move in the right direction, adds valid space texture
		if (cgame->validMove(player, x, y, posxr, posy1))
			SDL_RenderCopy(renderer, validSpaceTex, NULL, &destRA[posy1][posxr]);
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			//Human pieces, add texture matching piece
			if (cgame->get_index(i, j) == 'B')
				SDL_RenderCopy(renderer, playerPieceTex, NULL, &destRA[i][j]);
			//AI pieces, add texture matching piece
			else if (cgame->get_index(i, j) == 'W')
				SDL_RenderCopy(renderer, AIPieceTex, NULL, &destRA[i][j]);
		}
	}

	SDL_RenderPresent(renderer);		//Renders all textures
}

//Renders current game board, with 
void Game::renderJumps(cBoard *cgame, bool player)
{
	SDL_RenderClear(renderer);							//Clears renderer
	SDL_RenderCopy(renderer, boardTex, NULL, NULL);		//Adds board texture

	int newi;											//New y position
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			//Human
			if (player)
				newi = i - 2;
			//AI
			else
				newi = i + 2;

			//Jump in left position is valid, adds jump space textures
			if (cgame->validMove(player, j, i, j - 2, newi))
			{
				SDL_RenderCopy(renderer, jumpSpaceTex, NULL, &destRA[i][j]);
				SDL_RenderCopy(renderer, jumpSpaceTex, NULL, &destRA[newi][j - 2]);
			}
			//Jump in right position is valid, adds jump space textures
			if (cgame->validMove(player, j, i, j + 2, newi))
			{
				SDL_RenderCopy(renderer, jumpSpaceTex, NULL, &destRA[i][j]);
				SDL_RenderCopy(renderer, jumpSpaceTex, NULL, &destRA[newi][j + 2]);
			}

			//Human pieces, add texture
			if (cgame->get_index(i, j) == 'B')
				SDL_RenderCopy(renderer, playerPieceTex, NULL, &destRA[i][j]);
			//AI pieces, add texture
			else if (cgame->get_index(i, j) == 'W')
				SDL_RenderCopy(renderer, AIPieceTex, NULL, &destRA[i][j]);
		}
	}

	SDL_RenderPresent(renderer);						//Renders all textures
}

//Cleans game, closes window
void Game::clean()
{
	SDL_DestroyWindow(window);					//Destroys the game window
	SDL_DestroyRenderer(renderer);				//Destroys the renderer
	SDL_Quit();									//'Quits' the game, and ends all SDL2 processes
	std::cout << "Game Cleaned" << std::endl;
}