#include "cBoard.h"

#define BLANK_CHAR '_'
#define EMPTY_CHAR '-'
#define AI_PIECE 'W'
#define PLAYER_PIECE 'B'

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
	util = 0;
	endGame = false;
	next = nullptr;

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
	if (next != nullptr)
		delete next;
}

cBoard::cBoard(const cBoard* rhs) 
{
	board = new char*[6];
	for (int i = 0; i < 6; i++)
	{
		board[i] = new char[6];
		for (int j = 0; j < 6; j++)
			board[i][j] = rhs->get_index(i, j);
	}
	pPieces = rhs->get_pPieces();
	aPieces = rhs->get_aPieces();
	util = rhs->get_util();
	endGame = rhs->get_end();
	next = nullptr;
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

int cBoard::get_util() const
{
	return util;
}

bool cBoard::get_end() const
{
	return endGame;
}

cBoard* cBoard::get_next() const
{
	return next;
}


void cBoard::set_util(int newutil)
{
	util = newutil;
}

void cBoard::set_next(cBoard* newnext)
{
	next = new cBoard(newnext);
}


void cBoard::del_next()
{
	delete next;
	next = nullptr;
}

void cBoard::printBoard() 
{
	for (int i = 0; i < 6; i++) 
	{
		for (int j = 0; j < 6; j++)
			std::cout << board[i][j];
		std::cout << std::endl;
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
	char piece;
	if (player)
		piece = PLAYER_PIECE;
	else
		piece = AI_PIECE;

	//Checks to see if coord is in board
	if ((x >= 6) || (x < 0) || (y >= 6) || (y < 0))
		return false;

	//Check to see if piece selected matches player
	else if (board[y][x] != piece)
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

	else if ((abs(newy - y) == 1) && availJump(player))
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

bool cBoard::isEnd()
{
	if (endGame || (!availMoves(true) && !availMoves(false)))
		return true;
	else
		return false;
}

int cBoard::utilEval()
{
	int value = 0;
	int pBehind = 0, aBehind = 0;
	int pLine = 0, aLine = 5;
	bool pFlag = false, aFlag = false;

	while (pLine < 6)
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
	if ((pBehind == 0) && (aBehind == 0))
		endGame = true;

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

bool cBoard::availJump(bool player)
{
	char piece;
	if (player)
		piece = PLAYER_PIECE;
	else
		piece = AI_PIECE;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (board[i][j] == piece)
			{
				int newi;
				if (player)
					newi = i - 2;
				else
					newi = i + 2;
				if (validMove(player, j, i, j - 2, newi) || validMove(player, j, i, j + 2, newi))
					return true;
			}
		}
	}

	return false;
}


void cBoard::genActions(bool player, std::queue <cBoard*>& actions)
{
	int posxl, posxr, posxl2, posxr2;
	int posy1, posy2;
	bool jump = availJump(player);
	cBoard* temp;

	char piece;
	if (player)
		piece = PLAYER_PIECE;
	else
		piece = AI_PIECE;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (board[i][j] == piece)
			{
				if (jump)
				{
					if (player)
						posy2 = i - 2;
					else
						posy2 = i + 2;
					posxl2 = j - 2;
					posxr2 = j + 2;

					if (validMove(player, j, i, posxl2, posy2))
					{
						temp = new cBoard(this);
						temp->movePiece(player, j, i, posxl2, posy2);
						actions.push(temp);
					}
					if (validMove(player, j, i, posxr2, posy2))
					{
						temp = new cBoard(this);
						temp->movePiece(player, j, i, posxr2, posy2);
						actions.push(temp);
					}
				}
				else
				{
					if (player)
						posy1 = i - 1;
					else
						posy1 = i + 1;
					posxl = j - 1;
					posxr = j + 1;
					if (validMove(player, j, i, posxl, posy1))
					{
						temp = new cBoard(this);
						temp->movePiece(player, j, i, posxl, posy1);
						actions.push(temp);
					}
					if (validMove(player, j, i, posxr, posy1))
					{
						temp = new cBoard(this);
						temp->movePiece(player, j, i, posxr, posy1);
						actions.push(temp);
					}
				}
			}
		}
	}
}