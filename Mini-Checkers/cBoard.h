#pragma once
#include <iostream>
#include <queue>

//Checker Board class, that contains a board and can move pieces
class cBoard
{
private:
	char** board;		//Checker Board representation
	int pPieces;		//Total human player pieces
	int aPieces;		//Total AI pieces

	cBoard* next;		//Next best move, represented in cBoard
	bool endGame;		//Indicates whether it is a terminal state
public:
	cBoard();							//Default constructor
	~cBoard();							//Destructor
	cBoard(const cBoard* lhs);			//Copy Constructor

	//Getters
	char** get_board() const;			
	char get_index(int i, int j) const;
	int get_pPieces() const;
	int get_aPieces() const;
	bool get_end() const;
	cBoard* get_next() const;

	//Setters
	void set_next(cBoard* newnext);		//Setter that also makes a copy
	void del_next();					//'Setter' that deletes the current next value

	void printBoard();												//Prints the current board to the console
	bool movePiece(bool player, int x, int y, int newx, int newy);	//Moves a player's piece
	bool validMove(bool player, int x, int y, int newx, int newy);	//Validates a player's move
	void countPieces();												//Counts total pieces for both players
	bool isEnd();													//Checks to see if end conditions are met
	int eval();														//Evaluation function for non-terminal states
	int util();														//Utility function for terminal states
	bool availMoves(bool player);									//Checks to see if there are any available moves for the current player
	bool availJump(bool player);									//Checks to see if there are any available jumps for the current player
	void genActions(bool player, std::queue <cBoard*>& actions);	//Generates all actions a player can take
};

