#include <iostream>
#include "cBoard.h"
#include "Game.h"
using namespace std;

#define CUTOFF 10

int maxLevel = 0;
int totalNodes = 0;
int maxPrune = 0;
int minPrune = 0;

void playCheckers();
bool playerMove(bool player, cBoard *cgame, Game* game);

void ABSearch(cBoard* &cgame);
int maxValue(cBoard* cgame, int alpha, int beta, int level);
int minValue(cBoard* cgame, int alpha, int beta, int level);

void AIStats();
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
			while (!playerMove(true, cgame, game))	//Human Player Move
			{
				game->renderJumps(cgame, true);
				cout << "Invalid play" << endl;
			}
			game->render(cgame);
		}
		else
			cout << "Player Move Forfeited, No Moves Available!" << endl;
		
		if (cgame->availMoves(false))
		{
			cout << "AI is thinking..." << endl;
			ABSearch(cgame);
			AIStats();
			game->render(cgame);
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


void ABSearch(cBoard* &cgame)
{
	maxLevel = 0;
	totalNodes = 1;
	maxPrune = 0;
	minPrune = 0;

	int v = maxValue(cgame, -12, 12, 0);
	cBoard* temp = new cBoard(cgame->get_next());
	delete cgame;
	cgame = temp;
}

int maxValue(cBoard* cgame, int alpha, int beta, int level)
{
	totalNodes++;
	if (cgame->isEnd() || (level == CUTOFF))
		return cgame->utilEval();

	maxLevel = level;
	cout << level << endl;
	int v = -12;
	int cmp;
	queue<cBoard*> actions;
	cgame->genActions(false, actions);
	cBoard* temp;
	while (!actions.empty())
	{
		temp = actions.front();
		cmp = minValue(temp, alpha, beta, level + 1);
		temp->del_next();
		if (cmp > v)
		{
			v = cmp;
			cgame->set_next(temp);
		}
		actions.pop();
		if (v >= beta)
		{
			maxPrune++;
			return v;
		}
		
		if (v > alpha)
			alpha = v;
	}
	return v;
}

int minValue(cBoard* cgame, int alpha, int beta, int level)
{
	totalNodes++;
	if (cgame->isEnd() || (level == CUTOFF))
		return cgame->utilEval();

	maxLevel = level;
	cout << level << endl;
	int v = 12;
	int cmp;
	queue<cBoard*> actions;
	cgame->genActions(true, actions);
	cBoard* temp;
	while (!actions.empty())
	{
		temp = actions.front();
		cmp = maxValue(temp, alpha, beta, level + 1);
		temp->del_next();
		if (cmp < v)
		{
			v = cmp;
			cgame->set_next(temp);
		}
		actions.pop();
		if (v <= alpha)
		{
			minPrune++;
			return v;
		}

		if (v < beta)
			beta = v;
	}
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

void AIStats() 
{
	cout << endl << "AI Statistics:" << endl;
	cout << "Max Depth:\t" << maxLevel << endl;
	cout << "Total Nodes:\t" << totalNodes << endl;
	cout << "Max Prune:\t" << maxPrune << endl;
	cout << "Min Prune:\t" << minPrune << endl;
}