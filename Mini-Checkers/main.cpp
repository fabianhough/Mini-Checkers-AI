//Fabian Hough
//fh914
//CS-GY 6613 - Artificial Intelligence
//Project - AI Mini Checkers

/*
Simple (modified) Mini-Checker Game with Alpha-Beta Pruning Algorithm AI
Has GUI, and 3 levels of difficulty, all aiming to complete searches within a span of 15 seconds
(Search times are tested on an i5-6600K Intel Processor, as such, search times on other processors may vary)

Modified rules as follows:
- 6x6 board
- No Kings -> No backwards movement
- Jumps MUST be taken if they are available

Uses SDL2 for GUI
*/


#include <iostream>			//Standard IO operations
#include <chrono>			//Header for use in time tracking
#include "cBoard.h"			//Checker-Board class 
#include "Game.h"			//Game Engine Class; uses SDL2

//Cutoff definitions
#define CUTOFF 17			//Default Cutoff value for testing - Current value is max time under 15s for i5-6600K Processor
#define EASY 1				//Easy cutoff value
#define MEDIUM 5			//Medium cutoff value
#define HARD 16				//Hard cutoff value
int cutoff = CUTOFF;		//Global cutoff value for the AB Search

//Statistics Variables
int maxLevel = 0;			//Max depth of the current search	
int totalNodes = 0;			//Total nodes generated in the current search
int maxPrune = 0;			//Total times search is pruned in the Max function
int minPrune = 0;			//Total times search is pruned in the Min function
double timer = 0;			//Timer for total execution of search
double maxTime = 0;			//Max time taken across all searches

//Checkers Game Setup
void playCheckers();											//Primary function for playing Checkers Game
void intro(bool &ord);											//Intro/Player setup

//Human Player Turns
void playerTurn(cBoard* cgame, Game* game);						//Determines what happens when it is a player's turn
bool playerMove(bool player, cBoard *cgame, Game* game);		//How a human player moves; can be used for two human players

//AI Player Turns
void aiTurn(cBoard* &cgame, Game* game);						//Dictates the actions the AI player takes
void ABSearch(cBoard* &cgame);									//Initial starting/Root node of AB Search
int maxValue(cBoard* cgame, int alpha, int beta, int level);	//Max function
int minValue(cBoard* cgame, int alpha, int beta, int level);	//Min function
void AIStats();													//Outputs AI statistics from last search

//Main - left mostly empty for testing
int main() 
{
	playCheckers();	//Executes Checkers Game

	return 0;
}

//Sets up and plays a game of AI Mini Checkers
void playCheckers()
{
	bool ord;		//Determines which player moves first
	intro(ord);		//Executes the intro and setup sequence for the game

	std::cout << std::endl << "Loading Game..." << std::endl;


	cBoard* cgame = new cBoard();		//New Checker Board
	Game *game = nullptr;				//New Game Engine
	game = new Game();

	game->init("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, false);	//Initializes the game engine


	while (game->running())	//Continues as long as the game is still running
	{	
		game->render(cgame);		//Renders the game window

		std::cout << std::endl << std::endl;

		//Human Player goes first
		if (ord)
		{
			playerTurn(cgame, game);
			aiTurn(cgame, game);
		}
		//AI Player goes first
		else
		{
			aiTurn(cgame, game);
			playerTurn(cgame, game);
		}

		//If the game has ended
		if (cgame->isEnd())
		{
			std::cout << std::endl << std::endl << "Game Finished" << std::endl;
			int result = cgame->util();		//Finds the end state of the game board

			//AI has won (util = 12)
			if (result > 0)
				std::cout << "AI Won!" << std::endl;
			//Player has won (util = -12)
			else if (result < 0)
				std::cout << "Player Won!" << std::endl;
			//Draw (util = 0)
			else
				std::cout << "Draw!" << std::endl;
			
			break;	//Exit loop
		}
	}

	system("pause");	//Pauses the program so that the last image of the board is still available after completion
	game->clean();		//"Cleans" the game engine window
}

//Intro sequence for the Checkers Game
//Player can determine difficulty level
//Player can determine move order
void intro(bool &ord)
{
	std::cout << "Welcome to AI Mini-Checkers!" << std::endl;
	std::cout << "Please choose level of difficulty (1, 2, or 3):" << std::endl;

	int ans = 0;	//Variable to hold difficulty choice
	while ((ans != 1) && (ans != 2) && (ans != 3))	//Continues until valid choice
	{
		std::cin >> ans;	//Intakes answer
		switch (ans)
		{
		case 1:		//1 is selected
			std::cout << "EASY Selected..." << std::endl;
			cutoff = EASY;	//Sets cutoff to new value
			break;
		case 2:		//2 is selected
			std::cout << "MEDIUM Selected..." << std::endl;
			cutoff = MEDIUM;
			break;
		case 3:		//3 is selected
			std::cout << "HARD Selected..." << std::endl;
			cutoff = HARD;
			break;
		default:	//Invalid answer
			std::cout << "Please choose a valid selection:" << std::endl;
			break;
		}
	}

	int ordans = 0;		//Variable to hold order answer
	std::cout << std::endl << "Are you going first or second? (1 or 2):" << std::endl;
	while ((ordans != 1) && (ordans != 2))	//Continues until valid choice
	{
		std::cin >> ordans;		//Intakes answer
		switch (ordans)
		{
		case 1:		//1 is selected
			std::cout << "Player Moves First!" << std::endl;
			ord = true;		//Sets the player going first
			break;
		case 2:		//2 is selected
			std::cout << "AI Moves First!" << std::endl;
			ord = false;	//Sets the AI going first
			break;
		default:	//Invalid answer
			std::cout << "Please enter a valid answer:" << std::endl;
			break;
		}
	}
}

//Player turn function
//Renders the game, all jumps, and allows player to move pieces
void playerTurn(cBoard* cgame, Game* game)
{
	//Only executes if there are any available moves from the player
	if (cgame->availMoves(true))
	{
		game->renderJumps(cgame, true);					//Renders the valid jump locations on the board
		while (!playerMove(true, cgame, game))			//Human Player Move
		{
			game->renderJumps(cgame, true);				//Re-renders the valid jumps
			std::cout << "Invalid play" << std::endl;
		}
		game->render(cgame);							//Renders the updated game board
	}
	//No available moves to the player
	else
		std::cout << "Player Move Forfeited, No Moves Available!" << std::endl;
}

//Function that handles players moves
//Returns a true value if a move is successful, false if otherwise
bool playerMove(bool player, cBoard *cgame, Game* game)
{
	char piece, cmp;		//Placeholders for current player piece for comparison

	//Human Player Move
	if (player)
	{
		std::cout << "Player Move" << std::endl;
		piece = 'B';		//Black Piece
	}
	//AI Player Move (for manual entry)
	else
	{
		std::cout << "AI Move" << std::endl;
		piece = 'W';		//White Piece
	}
	int x, y, newx, newy;			//Position Variables
	game->mouseSelect(x, y);		//Allows player to select a piece on the board 
	cmp = cgame->get_index(y, x);	//Finds the piece at the location specified on the cBoard

	//Valid player-matched piece
	if ((cmp != '-') && (cmp != '_') && (cmp == piece))
	{
		game->renderMoves(cgame, player, x, y);				//Renders the moves available for piece
		game->mouseSelect(newx, newy);						//Player selects new location

		//Invalid Move
		if (!cgame->validMove(player, x, y, newx, newy))
		{
			game->render(cgame);	//Re-renders original game board
			return false;			//Returns false b/c of invalid move
		}
		//Valid Move
		else if(cgame->movePiece(player, x, y, newx, newy))	//Moves piece on cBoard object
		{
			std::cout << "Old Coordinates:\t" << x << "\t" << y << std::endl;
			std::cout << "New Coordinates:\t" << newx << "\t" << newy << std::endl;
			cgame->printBoard();	//Prints the console version of the board
			std::cout << std::endl;
			return true;			//Returns true
		}
	}
	//Piece is not player-matched piece
	else
	{
		std::cout << "Invalid Piece" << std::endl;
		return false;				//Returns false
	}
}


//Function that executes AI player moves and actions
void aiTurn(cBoard* &cgame, Game* game)
{
	//Only executes if there are any available moves from the AI
	if (cgame->availMoves(false) && (cgame->isEnd() != true))
	{
		std::cout << std::endl << "AI is thinking..." << std::endl;
		ABSearch(cgame);						//Executes the AB Search, and replaces cBoard with new one
		cgame->printBoard();					//Prints the resultant cBoard to the console
		AIStats();								//Prints statistics to the console
		SDL_Delay(1000);						//Delays rendering to show AI movement
		game->render(cgame);					//Re-renders the current game board
		SDL_ShowWindow(game->get_window());		//Re-shows window
		SDL_RaiseWindow(game->get_window());	//Re-focuses window
	}
	//No available moves to the AI
	else
		std::cout << "AI Move Forfeited, No Moves Available!" << std::endl;
}

//Alpha-Beta Search Algorithm
//Takes in a cBoard object, and searches for next ideal move using max and min value functions
//Sets the result to the "next" var in the cBoard object, and replaces the original
void ABSearch(cBoard* &cgame)
{
	maxLevel = 0;		//Reset all AI statistics global variables
	totalNodes = 1;
	maxPrune = 0;
	minPrune = 0;

	auto start = std::chrono::high_resolution_clock::now();			//Starts the clock

	int v = maxValue(cgame, -12, 12, 0);							//Searches starting from Max value (AI)

	auto stop = std::chrono::high_resolution_clock::now();			//Stops the clock
	std::chrono::duration<double, std::milli> dur = stop - start;	//Calculates the duration in milliseconds
	timer = dur.count();											//Sets the global timer to the new duration
	//Changes maxTime to match timer if new record
	if (timer > maxTime)
		maxTime = timer;

	cBoard* temp = new cBoard(cgame->get_next());					//Sets the new game board to temp var
	delete cgame;													//Deletes the old game board
	cgame = temp;													//Sets the old pointer to the new game board
}

//Max value function for the AB Search
//Attempts to find the ideal move for the Max (AI) player
int maxValue(cBoard* cgame, int alpha, int beta, int level)
{
	//Updating global variables, maxLevel and totalNodes
	if (level > maxLevel)
		maxLevel = level;
	totalNodes++;

	//Returns Utility value if the game is in an end state
	if (cgame->isEnd())
		return cgame->util();
	
	//Returns a value based on Eval function if cutoff depth is reached
	if (level == cutoff)
		return cgame->eval();

	//std::cout << level << std::endl;
	int v = -12;						//Initial v value
	int cmp;							//Comparison value for v
	std::queue<cBoard*> actions;		//Queue of actions that Max can take
	cgame->genActions(false, actions);	//Generates all actions that Max can take
	cBoard* temp;						//Temporary cBoard var for manipulation

	//Continues until all actions explored, or pruned
	while (!actions.empty())
	{
		temp = actions.front();								//Compares the front action in actions
		cmp = minValue(temp, alpha, beta, level + 1);		//Calls Min function with the current action
		temp->del_next();									//Deletes the 'next' action after current action

		//Action is most ideal so far
		if (cmp > v)
		{
			v = cmp;					//v value is changed to match current action
			cgame->del_next();			//Deletes the 'old' best action
			cgame->set_next(temp);		//Sets current action as best action (makes copy)
		}
		delete temp;					//Deletes current action
		actions.pop();					//Removes action pointer from queue

		//Pruning
		if (v >= beta)
		{
			maxPrune++;					//Global pruning updated

			//Deletes remnant actions in the queue
			while (!actions.empty())
			{
				temp = actions.front();
				delete temp;
				actions.pop();
			}
			return v;					//Returns v value, cutting search short
		}
		
		//Sets the best current value to beat
		if (v > alpha)
			alpha = v;
	}
	return v;							//Returns best v
}

//Min value function for the AB Search
//Attempts to find the ideal move for the Min (Human) player
int minValue(cBoard* cgame, int alpha, int beta, int level)
{
	//Updating global variables, maxLevel and totalNodes
	if (level > maxLevel)
		maxLevel = level;
	totalNodes++;

	//Returns Utility value if the game is in an end state
	if (cgame->isEnd())
		return cgame->util();

	//Returns a value based on Eval function if cutoff depth is reached
	if (level == cutoff)
		return cgame->eval();

	//std::cout << level << std::endl;
	int v = 12;							//Initial v value
	int cmp;							//Comparison value for v
	std::queue<cBoard*> actions;		//Queue of actions that Min can take
	cgame->genActions(true, actions);	//Generates all actions that Min can take
	cBoard* temp;						//Temporary cBoard var for manipulation

	//Continues until all actions explored, or pruned
	while (!actions.empty())
	{
		temp = actions.front();								//Compares the front action in actions
		cmp = maxValue(temp, alpha, beta, level + 1);		//Calls Max function with the current action
		temp->del_next();									//Deletes the 'next' action after current action

		//Action is most ideal so far
		if (cmp < v)
		{
			v = cmp;					//v value is changed to match current action
			cgame->del_next();			//Deletes the 'old' best action
			cgame->set_next(temp);		//Sets current action as best action (makes copy)
		}
		delete temp;					//Deletes current action
		actions.pop();					//Removes action pointer from queue

		//Pruning
		if (v <= alpha)
		{
			minPrune++;					//Global pruning updated

			//Deletes remnant actions in the queue
			while (!actions.empty())
			{
				temp = actions.front();
				delete temp;
				actions.pop();
			}
			return v;					//Returns v value, cutting search short
		}

		//Sets the best current value to beat
		if (v < beta)
			beta = v;
	}
	return v;							//Returns best v
}

//Prints to console the list of AI Statistics
//Includes: Max Depth, Total Nodes Generated, Pruned Nodes, and Time
void AIStats() 
{
	std::cout << std::endl << "AI Statistics:" << std::endl;
	std::cout << "Max Depth:\t" << maxLevel << std::endl;
	std::cout << "Total Nodes:\t" << totalNodes << std::endl;
	std::cout << "Max Prune:\t" << maxPrune << std::endl;
	std::cout << "Min Prune:\t" << minPrune << std::endl;
	std::cout << std::endl;

	//Divides all timer values by 1000 to match seconds
	std::cout << "Time:\t\t" << timer / 1000 << " s" << std::endl;
	std::cout << "Max Time:\t" << maxTime / 1000 << " s" << std::endl;
}
