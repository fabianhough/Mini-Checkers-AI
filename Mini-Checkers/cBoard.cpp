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
	pPieces = 6;
	aPieces = 6;

	for (int i = 0; i < 6; i++) 
	{
		board[i] = new char[6];

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

cBoard::cBoard(const cBoard &rhs) 
{
	board = new char*[6];
	for (int i = 0; i < 6; i++)
	{
		board[i] = new char[6];
		for (int j = 0; j < 6; j++)
			board[i][j] = rhs.get_index(i, j);
	}
	pPieces = rhs.get_pPieces();
	aPieces = rhs.get_aPieces();
}

char** cBoard::get_board() const
{
	return board;
}

char cBoard::get_index(int i, int j) const
{
	return board[i][j];
}

int cBoard::get_pPieces() const
{
	return pPieces;
}
int cBoard::get_aPieces() const
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


	if (this->validMove(player, x, y, newx, newy)) 
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
	if ((x >= 6) || (x < 0) || (y >= 6) || (y < 0)
		|| (board[y][x] == EMPTY_CHAR) || (board[y][x] == BLANK_CHAR))
		return false;

	//Checks for new location validity
	else if ((newx >= 6) || (newx < 0) || (newy >= 6) || (newy < 0)
		|| (board[newy][newx] != EMPTY_CHAR))
		return false;

	//Checks to see if move is within bounds
	//Additionally checks jump conditions
	else if (((abs(newx - x) != 1) && (abs(newx - x) != 2))
		|| (abs(newy - y) != 1) && (abs(newy - y) != 2))
		return false;

	//Checks if player is moving in the right direction
	else if ((player && ((newy - y) > 0)) || (!player && ((newy - y) < 0)))
		return false;

	//Checks to see if trying to make a single move when a jump move is possible
	else if ((abs(newy - y) == 1)
		&& (validMove(player, x, y, x + 2, 2 * newy - y) || validMove(player, x, y, x - 2, 2 * newy - y)))
		return false;

	//Checks if it is a jump
	else if (abs(newy - y) == 2)
	{
		int remx = (newx + x) / 2;	//'Jumped' Piece coord
		int remy = (newy + y) / 2;
		if ((board[remy][remx] == board[y][x]) || (board[remy][remx] == EMPTY_CHAR))
		{	//Checks to see if jumping over own piece or empty space
			//Statement also checks to see if making invalid move (i.e. x + 1, y + 2)
			return false;
		}
	}

	//Returns true if valid move
	else
		return true;
}

void cBoard::countPieces()
{
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

int cBoard::isEnd()
{
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

int cBoard::utilEval()
{
	int value = 0;
	int pBehind = 0, aBehind = 0;
	int pLine = 0, aLine = 5;
	bool pFlag = false, aFlag = false;

	while (pLine <= aLine)
	{
		for (int j = 0; j < 6; j++)
		{
			if (!aFlag && (board[pLine][j] == AI_PIECE))
			{
				aFlag = true;
				break;
			}
			else if (aFlag)
			{
				if (board[pLine][j] == PLAYER_PIECE)
						pBehind++;
			}
		}

		for (int k = 0; k < 6; k++)
		{
			if (!pFlag && (board[aLine][k] == PLAYER_PIECE))
			{
				pFlag = true;
				break;
			}
			else if (pFlag)
			{
				if (board[aLine][k] == AI_PIECE)
					aBehind++;
			}
		}

		pLine++;
		aLine--;
	}

	value = ((2 * aPieces - aBehind) - (2 * pPieces - pBehind));

	return value;
}

bool cBoard::availMoves(bool player)
{
	int posxl, posxr, posxl2, posxr2;
	int posy1, posy2;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (player && (board[i][j] == PLAYER_PIECE))
			{
				posy1 = i - 1;
				posy2 = i - 2;
				posxl = j - 1;
				posxr = j + 1;
				posxl2 = j - 2;
				posxr2 = j + 2;

				if (validMove(player, j, i, posxl, posy1) || validMove(player, j, i, posxr, posy1)
					|| validMove(player, j, i, posxl2, posy2) || validMove(player, j, i, posxr2, posy2))
					return true;
			}
			else if (!player && (board[i][j] == AI_PIECE))
			{
				posy1 = i + 1;
				posy2 = i + 2;
				posxl = j - 1;
				posxr = j + 1;
				posxl2 = j - 2;
				posxr2 = j + 2;

				if (validMove(player, j, i, posxl, posy1) || validMove(player, j, i, posxr, posy1)
					|| validMove(player, j, i, posxl2, posy2) || validMove(player, j, i, posxr2, posy2))
					return true;

			}

		}
	}

	return false;
}