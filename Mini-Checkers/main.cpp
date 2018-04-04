#include <iostream>
#include "SDL.h"
#undef main
#include "cBoard.h"
using namespace std;

int main() 
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		600, 400, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);

	SDL_Delay(3000);


	while (true)
	{


	}




	/*
	cBoard game;
	game.printBoard();
	cout << endl << game.get_aPieces() << endl << game.get_pPieces() << endl;
	cout << endl;
	game.movePiece(true, 3, 1, 2, 2);
	cout << endl;
	game.printBoard();
	game.movePiece(true, 5, 1, 4, 2);
	cout << endl;
	game.printBoard();
	game.movePiece(true, 4, 0, 5, 1);
	cout << endl;
	game.printBoard();
	game.movePiece(false, 4, 1, 3, 2);
	cout << endl << game.get_aPieces() << endl << game.get_pPieces() << endl;
	cout << endl;
	game.printBoard();
	game.movePiece(true, 2, 2, 4, 4);
	cout << endl << game.get_aPieces() << endl << game.get_pPieces() << endl;
	cout << endl;
	game.printBoard();
	game.movePiece(false, 5, 0, 3, 2);
	cout << endl;
	game.printBoard();
	game.movePiece(true, 2, 0, 0, 2);
	cout << endl;
	game.printBoard();

	cout << endl << game.get_aPieces() << endl << game.get_pPieces() << endl;*/

	return 0;
}