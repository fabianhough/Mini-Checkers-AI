#include <iostream>
#include <chrono>
#include "cBoard.h"
#include "Game.h"

#define CUTOFF 10
#define EASY 2
#define MEDIUM 5
#define HARD 17

int maxLevel = 0;
int totalNodes = 0;
int maxPrune = 0;
int minPrune = 0;
double timer = 0;
double maxTime = 0;
int cutoff = CUTOFF;
bool ord = true;

void playCheckers();
bool playerMove(bool player, cBoard *cgame, Game* game);

void ABSearch(cBoard* &cgame);
int maxValue(cBoard* cgame, int alpha, int beta, int level);
int minValue(cBoard* cgame, int alpha, int beta, int level);

void AIStats();
void test(cBoard* cgame, bool player);

void playerTurn(cBoard* cgame, Game* game);
void aiTurn(cBoard* &cgame, Game* game);

void intro();

int main() 
{
	playCheckers();

	return 0;
}

void playCheckers()
{
	intro();

	std::cout << std::endl << "Loading Game..." << std::endl;


	cBoard* cgame = new cBoard();
	Game *game = nullptr;
	game = new Game();

	game->init("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, false);


	while (game->running())
	{
		//game->handleEvents();
		game->update();
		game->render(cgame);

		std::cout << std::endl << std::endl;

		if (ord)
		{
			playerTurn(cgame, game);
			aiTurn(cgame, game);
		}
		else
		{
			aiTurn(cgame, game);
			playerTurn(cgame, game);
		}

		if (cgame->isEnd())
		{
			std::cout << std::endl << std::endl << "Game Finished" << std::endl;
			int result = cgame->eval();
			if (result > 0)
				std::cout << "AI Won!" << std::endl;
			else if (result < 0)
				std::cout << "Player Won!" << std::endl;
			else
				std::cout << "Draw!" << std::endl;
			
			break;
		}
	}

	int v;
	std::cout << "Enter 0 to continue" << std::endl;
	std::cin >> v;
	if (v == 0)
		game->clean();
}

bool playerMove(bool player, cBoard *cgame, Game* game)
{
	char piece, cmp;
	if (player)
	{
		std::cout << "Player Move" << std::endl;
		piece = 'B';
	}
	else
	{
		std::cout << "AI Move" << std::endl;
		piece = 'W';
	}
	int x, y, newx, newy;
	game->mouseSelect(x, y);
	cmp = cgame->get_index(y, x);
	if ((cmp != '-') && (cmp != '_') && (cmp == piece))
	{
		game->renderMoves(cgame, player, x, y);
		game->mouseSelect(newx, newy);
		if (!cgame->validMove(player, x, y, newx, newy))
		{
			game->render(cgame);
			return false;
		}
		else
		{
			cgame->movePiece(player, x, y, newx, newy);
			std::cout << "Old Coordinates:\t" << x << "\t" << y << std::endl;
			std::cout << "New Coordinates:\t" << newx << "\t" << newy << std::endl;
			cgame->printBoard();
			std::cout << std::endl;
			return true;
		}
	}
	else
	{
		std::cout << "Invalid Piece" << std::endl;
		return false;
	}
}


void ABSearch(cBoard* &cgame)
{
	maxLevel = 0;
	totalNodes = 1;
	maxPrune = 0;
	minPrune = 0;

	auto start = std::chrono::high_resolution_clock::now();
	int v = maxValue(cgame, -12, 12, 0);
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> dur = stop - start;
	timer = dur.count();
	if (timer > maxTime)
		maxTime = timer;

	cBoard* temp = new cBoard(cgame->get_next());
	delete cgame;
	cgame = temp;
}

int maxValue(cBoard* cgame, int alpha, int beta, int level)
{
	maxLevel = level;
	totalNodes++;
	if (cgame->isEnd())
		return cgame->util();
	if (level == cutoff)
		return cgame->eval();

	//std::cout << level << std::endl;
	int v = -12;
	int cmp;
	std::queue<cBoard*> actions;
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
			cgame->del_next();
			cgame->set_next(temp);
		}
		delete temp;
		actions.pop();
		if (v >= beta)
		{
			maxPrune++;
			while (!actions.empty())
			{
				temp = actions.front();
				delete temp;
				actions.pop();
			}
			return v;
		}
		
		if (v > alpha)
			alpha = v;
	}
	return v;
}

int minValue(cBoard* cgame, int alpha, int beta, int level)
{
	maxLevel = level;
	totalNodes++;
	if (cgame->isEnd())
		return cgame->util();
	if (level == cutoff)
		return cgame->eval();

	//std::cout << level << std::endl;
	int v = 12;
	int cmp;
	std::queue<cBoard*> actions;
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
			cgame->del_next();
			cgame->set_next(temp);
		}
		delete temp;
		actions.pop();
		if (v <= alpha)
		{
			minPrune++;
			while (!actions.empty())
			{
				temp = actions.front();
				delete temp;
				actions.pop();
			}
			return v;
		}

		if (v < beta)
			beta = v;
	}
	return v;
}

void test(cBoard* cgame, bool player) {
	std::queue<cBoard*> actions;

	cgame->genActions(player, actions);

	cBoard* temp;
	int size = actions.size();
	for (int i = 0; i < size; i++)
	{
		temp = actions.front();
		temp->printBoard();
		std::cout << std::endl;
		actions.pop();
	}
}

void AIStats() 
{
	std::cout << std::endl << "AI Statistics:" << std::endl;
	std::cout << "Max Depth:\t" << maxLevel << std::endl;
	std::cout << "Total Nodes:\t" << totalNodes << std::endl;
	std::cout << "Max Prune:\t" << maxPrune << std::endl;
	std::cout << "Min Prune:\t" << minPrune << std::endl;
	std::cout << std::endl;
	std::cout << "Time:\t\t" << timer / 1000 << " s" << std::endl;
	std::cout << "Max Time:\t" << maxTime / 1000 << " s" << std::endl;
}

void playerTurn(cBoard* cgame, Game* game)
{
	if (cgame->availMoves(true))
	{
		game->renderJumps(cgame, true);
		while (!playerMove(true, cgame, game))	//Human Player Move
		{
			game->renderJumps(cgame, true);
			std::cout << "Invalid play" << std::endl;
		}
		game->render(cgame);
	}
	else
		std::cout << "Player Move Forfeited, No Moves Available!" << std::endl;
}

void aiTurn(cBoard* &cgame, Game* game)
{
	if (cgame->availMoves(false))
	{
		std::cout << std::endl << "AI is thinking..." << std::endl;
		ABSearch(cgame);
		cgame->printBoard();
		AIStats();
		game->render(cgame);
	}
	else
		std::cout << "AI Move Forfeited, No Moves Available!" << std::endl;
}

void intro()
{
	std::cout << "Welcome to AI Mini-Checkers!" << std::endl;
	std::cout << "Please choose level of difficulty (1, 2, or 3):" << std::endl;

	int ans = 0;
	while ((ans != 1) && (ans != 2) && (ans != 3))
	{
		std::cin >> ans;
		switch (ans)
		{
		case 1:
			std::cout << "EASY Selected..." << std::endl;
			cutoff = EASY;
			break;
		case 2:
			std::cout << "MEDIUM Selected..." << std::endl;
			cutoff = MEDIUM;
			break;
		case 3:
			std::cout << "HARD Selected..." << std::endl;
			cutoff = HARD;
			break;
		default:
			std::cout << "Please choose a valid selection:" << std::endl;
			break;
		}
	}

	int ordans = 0;
	std::cout << std::endl << "Are you going first or second? (1 or 2):" << std::endl;
	while ((ordans != 1) && (ordans != 2))
	{
		std::cin >> ordans;
		switch (ordans)
		{
		case 1:
			std::cout << "Player Moves First!" << std::endl;
			ord = true;
			break;
		case 2:
			std::cout << "AI Moves First!" << std::endl;
			ord = false;
			break;
		default:
			std::cout << "Please enter a valid answer:" << std::endl;
			break;
		}
	}
}