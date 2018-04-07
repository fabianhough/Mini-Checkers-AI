#include <iostream>
#include "cBoard.h"
#include "Game.h"
using namespace std;

#define CUTOFF 50;

void playCheckers();
bool playerMove(bool player, cBoard *cgame, Game* game);


int maxValue(cBoard* cgame, int alpha, int beta);
int minValue(cBoard* cgame, int alpha, int beta);

void test(cBoard* cgame, bool player);

int main() 
{
	playCheckers();

	return 0;
}

void playCheckers()
{
	cBoard* cgame = new cBoard();
	Game *game = nullptr;
	game = new Game();

	game->init("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, false);

	//Ask if player wants to move first or second

	while (game->running())
	{
		//game->handleEvents();
		game->update();
		game->render(cgame);

		if (cgame->availMoves(true))
		{
			game->renderJumps(cgame, true);
			test(cgame, true);
			while (!playerMove(true, cgame, game))	//Human Player Move
			{
				game->renderJumps(cgame, true);
				cout << "Invalid play" << endl;
			}
			cout << "Current utility:\t" << cgame->utilEval() << endl << endl;
			game->render(cgame);
		}
		else
			cout << "Player Move Forfeited, No Moves Available!" << endl;
		
		if (cgame->availMoves(false))
		{
			//Remove when adding AI
			game->renderJumps(cgame, false);
			test(cgame, false);
			while (!playerMove(false, cgame, game))	//AI Player Move
			{
				game->renderJumps(cgame, false);
				cout << "Invalid play" << endl;
			}
			cout << "Current utility:\t" << cgame->utilEval() << endl << endl;
		}
		else
			cout << "AI Move Forfeited, No Moves Available!" << endl;

		if (cgame->isEnd())
		{
			cout << endl << endl << "Game Finished" << endl;
			int result = cgame->utilEval();
			if (result > 0)
				cout << "AI Won!" << endl;
			else if (result < 0)
				cout << "Player Won!" << endl;
			else
				cout << "Draw!" << endl;
			
			break;
		}
	}

	game->clean();
}

bool playerMove(bool player, cBoard *cgame, Game* game)
{
	char piece, cmp;
	if (player)
	{
		cout << "Player Move" << endl;
		piece = 'B';
	}
	else
	{
		cout << "AI Move" << endl;
		piece = 'W';
	}
	int x, y, newx, newy;
	game->mouseSelect(x, y);
	cmp = cgame->get_index(y, x);
	if ((cmp != '-') && (cmp != '_') && (cmp == piece))
	{
		game->renderMoves(cgame, player, x, y);

		cout << "Old Coordinates:\t" << x << "\t" << y << endl;

		game->mouseSelect(newx, newy);
		cout << "New Coordinates:\t" << newx << "\t" << newy << endl;
		if (!cgame->validMove(player, x, y, newx, newy))
		{
			game->render(cgame);
			return false;
		}
		else
		{
			cgame->movePiece(player, x, y, newx, newy);
			cgame->printBoard();
			cout << endl;
			return true;
		}
	}
	else
	{
		cout << "Invalid Piece" << endl;
		return false;
	}
}


cBoard* ABSearch(cBoard* cgame)
{
	return cgame;
}

int maxValue(cBoard* cgame, int alpha, int beta)
{
	if (cgame->isEnd())
		return cgame->get_util();
	int v = -12;


	return v;
}

int minValue(cBoard* cgame, int alpha, int beta)
{
	if (cgame->isEnd())
		return cgame->get_util();
	int v = 12;



	return v;
}

void test(cBoard* cgame, bool player) {
	queue<cBoard*> actions;

	cgame->genActions(player, actions);

	cBoard* temp;
	int size = actions.size();
	for (int i = 0; i < size; i++)
	{
		temp = actions.front();
		temp->printBoard();
		cout << endl;
		actions.pop();
	}
}