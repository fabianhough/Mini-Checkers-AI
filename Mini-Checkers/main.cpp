#include <iostream>
#include "cBoard.h"
#include "Game.h"
using namespace std;

void playCheckers();
bool playerMove(bool player, cBoard cgame, Game* game);

int main() 
{
	playCheckers();

	/*

	cgame.movePiece(true, 3, 1, 5, 2);

	cgame.movePiece(true, 5, 1, 4, 2);

	cgame.movePiece(true, 4, 0, 5, 1);

	cgame.movePiece(false, 4, 1, 3, 2);

	cgame.movePiece(true, 2, 2, 4, 4);

	cgame.movePiece(false, 5, 0, 3, 2);

	cgame.movePiece(true, 2, 0, 0, 2);

	while (game->running())
	{
		game->handleEvents();
		game->update();
		game->render(cgame.get_board());
	}

	game->clean();*/



	return 0;
}

void playCheckers()
{
	cBoard cgame;
	Game *game = nullptr;
	game = new Game();

	game->init("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, false);

	while (game->running())
	{
		//game->handleEvents();
		game->update();
		game->render(cgame);


		int x, y, newx, newy;
		game->mouseSelect(x, y);
		cout << "Old Coordinates:\t" << x << "\t" << y << endl;
		if ((x < 6) && (y < 6))
		{
			game->mouseSelect(newx, newy);
			cout << "New Coordinates:\t" << newx << "\t" << newy << endl;
			if ((newx < 6) && (newy < 6))
			{
				cgame.movePiece(true, x, y, newx, newy);
				cgame.printBoard();
				cout << endl;

			}
		}

		/*
		while (!playerMove(true, cgame, game))
		{
			cout << "Invalid play" << endl;
		}
		cout << "Before Render" << endl;
		/*
		game->render(cgame.get_board());
		cout << "After Render" << endl;
		while (!playerMove(false, cgame, game))
		{
			cout << "Invalid play" << endl;
		}
		*/
	}

	game->clean();
}

bool playerMove(bool player, cBoard cgame, Game* game)
{
	if (player)
		cout << "Player Move" << endl;
	else
		cout << "AI Move" << endl;
	int x, y, newx, newy;
	game->mouseSelect(x, y);
	cout << "Old Coordinates:\t" << x << "\t" << y << endl;
	if ((x < 6) && (y < 6))
	{
		game->mouseSelect(newx, newy);
		cout << "New Coordinates:\t" << newx << "\t" << newy << endl;
		if ((newx < 6) && (newy < 6))
		{
			cgame.movePiece(player, x, y, newx, newy);
			cgame.printBoard();
			cout << endl;
			return true;
		}
	}
	return false;
}