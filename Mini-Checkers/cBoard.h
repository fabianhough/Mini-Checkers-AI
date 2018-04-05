#pragma once
class cBoard
{
private:
	char** board;
	int pPieces;
	int aPieces;
public:
	cBoard();
	~cBoard();

	char** get_board();
	char get_index(int i, int j);
	int get_pPieces();
	int get_aPieces();

	void printBoard();
	void movePiece(bool player, int x, int y, int newx, int newy);
	bool validMove(bool player, int x, int y, int newx, int newy);
	void countPieces();
	int isEnd();
};

