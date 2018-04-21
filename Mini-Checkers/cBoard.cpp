#include "cBoard.h"

#define BLANK_CHAR '_'		//Invalid Space
#define EMPTY_CHAR '-'		//Empty Space
#define AI_PIECE 'W'		//AI Piece, White
#define PLAYER_PIECE 'B'	//Player Piece, Black


//Default constructor
//Creates an initial game state
cBoard::cBoard()
{
	board = new char*[6];		//Setting up new board
	char piece;					//Temporary Piece for player
	pPieces = 6;				//Sets initial human player pieces
	aPieces = 6;				//Sets initial AI player Pieces
	endGame = false;			//Game is not terminal
	next = nullptr;				//No next best move calculated
	
	//Initializes all pieces on the board
	//Runs through line by line
	for (int i = 0; i < 6; i++) 
	{
		board[i] = new char[6];		

		//Current line is at the top, AI Pieces
		if (i <= 1)
			piece = AI_PIECE;
		//Current line is at the bottom, Human Pieces
		else if (i >= 4)
			piece = PLAYER_PIECE;
		//Current line is neither, Empty Pieces
		else
			piece = EMPTY_CHAR;

		//Runs through each horizontal location
		for (int j = 0; j < 6; j++) 
		{
			//Places pieces depending on row/line, column, and current piece
			//Fills the rest with invalid spaces
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
}

//Destructor
cBoard::~cBoard()
{
	//Deletes each sub-array
	for (int i = 0; i < 6; i++)
		delete[] board[i];
	delete[] board;			//Deletes primary board array
	if (next != nullptr)	//Deletes the next best cBoard if it isn't empty
		delete next;
}

//Copy Constructor
cBoard::cBoard(const cBoard* rhs) 
{
	board = new char*[6];						//Initializes new board variable
	for (int i = 0; i < 6; i++)
	{
		board[i] = new char[6];					//Creates the sub-array
		for (int j = 0; j < 6; j++)				//Copies all values in sub-array
			board[i][j] = rhs->get_index(i, j);
	}
	pPieces = rhs->get_pPieces();				//Copies current piece numbers
	aPieces = rhs->get_aPieces();
	endGame = rhs->get_end();					//Copies endgame status
	next = nullptr;								//Sets next best action to null
}


//Getters

//Returns board
char** cBoard::get_board() const
{
	return board;
}

//Returns character at index
char cBoard::get_index(int i, int j) const
{
	return board[i][j];
}

//Returns the amount of AI Pieces
int cBoard::get_pPieces() const
{
	return pPieces;
}

//Returns the amount of human player Pieces
int cBoard::get_aPieces() const
{
	return aPieces;
}

//Returns endgame status
bool cBoard::get_end() const
{
	return endGame;
}

//Returns next best action cBoard
cBoard* cBoard::get_next() const
{
	return next;
}


//Setters

//Sets the next variable to the given cBoard
void cBoard::set_next(cBoard* newnext)
{
	next = new cBoard(newnext);
}

//Deletes the next var
void cBoard::del_next()
{
	delete next;
	next = nullptr;
}


//Prints the board to the console
void cBoard::printBoard() 
{
	for (int i = 0; i < 6; i++) 
	{
		for (int j = 0; j < 6; j++)
			std::cout << board[i][j];
		std::cout << std::endl;
	}
}

//Moves a given players piece to given coordinates
void cBoard::movePiece(bool player, int x, int y, int newx, int newy) 
{
	//Checks to see if the given move is valid
	if (this->validMove(player, x, y, newx, newy)) 
	{
		//Checks to see if there are any jump moves available to current player
		if (availJump(player))
		{
			//Returns if jump moves are available, but current move is not a jump move
			if (abs(newy - y) != 2)
				return;
		}
		if (abs(newy - y) == 2)									//Checks if it is a jump
			board[(newy + y) / 2][(newx + x) / 2] = EMPTY_CHAR;	//Removes 'jumped' piece
		
		char piece = board[y][x];								//Temporary placeholder for the piece
		board[y][x] = EMPTY_CHAR;								//Removes piece at old location
		board[newy][newx] = piece;								//Sets piece at new location
	}

	this->countPieces();										//Counts all current pieces

	return;
}

//Validates moves by a specific player
bool cBoard::validMove(bool player, int x, int y, int newx, int newy) {
	char piece;		//Temporary comparison variable for the player piece

	//Checks if it is AI or human player
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
	return true;
}

//Counts total amount of both players' pieces
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

//Checks to see if end condition is met
bool cBoard::isEnd()
{
	int temp = eval();		//Eval Function sets an end true in the running
	//Returns true if endgame flag, or if no available moves for both sides
	if (endGame || (!availMoves(true) && !availMoves(false)))
		return true;
	else
		return false;
}

int cBoard::eval()
{
	int value = 0;							//Evaluation return value
	int pBehind = 0, aBehind = 0;			//'Backline' of each player
	int pLine = 0, aLine = 5;				//'Frontline' of each player
	bool pFlag = false, aFlag = false;		//Flags to identify line locations


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

int cBoard::util()
{
	countPieces();

	if (aPieces == 0)
		return -12;
	else if (pPieces == 0)
		return 12;
	else
	{
		if (aPieces > pPieces)
			return 12;
		else if (pPieces > aPieces)
			return -12;
		else
			return 0;
	}

	return 0;
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