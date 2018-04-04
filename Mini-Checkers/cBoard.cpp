#include "cBoard.h"
#include <iostream>
using namespace std;

const char BLANK_CHAR = '_';
const char EMPTY_CHAR = '-';
const char AI_PIECE = 'W';
const char PLAYER_PIECE = 'B';

/* 
Things to finish:
- Finish end condition checker
- Add Utility value function
- Add copy constructor
- Add getter for board

Goal: Complete coding for the entire checkers board
Functionality needed:
- Set up board
- Copy board
- Move pieces (in a valid way)
- Check for Win conditions
- Check for Utility Value -> can use win condition in Utility value function
*/


cBoard::cBoard()
{
	board = new char*[6];
	char piece;
	for (int i = 0; i < 6; i++)
		board[i] = new char[6];

	for (int i = 0; i < 6; i++) 
	{
		if (i <= 1)
			piece = AI_PIECE;
		else if (i >= 4)
			piece = PLAYER_PIECE;
		else
			piece = EMPTY_CHAR;

		for (int j = 0; j < 6; j++) 
		{
			if (i % 2 == 0) {
				if (j % 2 == 0)
					board[i][j] = BLANK_CHAR;
				else
					board[i][j] = piece;
			}
			else 
			{
				if (j % 2 == 1)
					board[i][j] = BLANK_CHAR;
				else
					board[i][j] = piece;
			}
		}
	}
	this->countPieces();
}


cBoard::~cBoard()
{
	for (int i = 0; i < 6; i++)
		delete[] board[i];
	delete[] board;
}

int cBoard::get_pPieces() 
{
	return pPieces;
}
int cBoard::get_aPieces() 
{
	return aPieces;
}

void cBoard::printBoard() 
{
	for (int i = 0; i < 6; i++) 
	{
		for (int j = 0; j < 6; j++)
			cout << board[i][j];
		cout << endl;
	}
}

void cBoard::movePiece(bool player, int x, int y, int newx, int newy) {
	if (player) 
	{	//Adjusts Player position
		y = 5 - y;
		newy = 5 - newy;
	}

	if (validMove(player, x, y, newx, newy)) 
	{
		if (abs(newy - y) == 2) //Checks if it is a jump
			board[(newy + y) / 2][(newx + x) / 2] = EMPTY_CHAR;	//Removes 'jumped' piece
		
		char piece = board[y][x];	//Temporary placeholder for the piece
		board[y][x] = EMPTY_CHAR;	//Removes piece at old location
		board[newy][newx] = piece;	//Sets piece at new location
	}

	this->countPieces();

	return;
}

bool cBoard::validMove(bool player, int x, int y, int newx, int newy) {
	//Checks to see if coord is in board, or is invalid piece
	if ((x >= 6) || (y >= 6) || (board[y][x] == EMPTY_CHAR) || (board[y][x] == BLANK_CHAR))
		return false;
	//Checks for new location validity
	else if ((newx >= 6) || (newy >= 6) || (board[newy][newx] != EMPTY_CHAR))
		return false;
	//Checks to see if move is within bounds
	//Additionally checks jump conditions
	else if (((newx != x + 1) && (newx != x - 1) && (newx != x + 2) && (newx != x - 2))
		|| (!player && (newy != y + 1) && (newy != y + 2))
		|| (player && (newy != y - 1) && (newy != y - 2)))
		return false;
	//Checks if it is a jump
	else if (abs(newy - y) == 2) 
	{
		int remx = (newx + x) / 2;	//'Jumped' Piece coord
		int remy = (newy + y) / 2;
		if (board[remy][remx] == board[y][x]) 
		{	//Checks to see if jumping over own piece
			return false;
		}
	}
	else
		return true;
}

void cBoard::countPieces() {
	int tempP = 0;
	int tempA = 0;
	for (int i = 0; i < 6; i++) 
	{
		for (int j = 0; j < 6; j++) 
		{
			if (board[i][j] == AI_PIECE)
				tempA++;
			else if (board[i][j] == PLAYER_PIECE)
				tempP++;
		}
	}
	pPieces = tempP;
	aPieces = tempA;
}

int cBoard::isEnd() {
	if (pPieces == 0)
		return 1;
	else if (aPieces == 0)
		return -1;
	else 
	{
		int temp = 0;
		for (int i = 0; i < 6; i++) 
		{
			for (int j = 0; j < 6; j++) 
			{

			}
		}



	}


		
	return 0;
}