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

	int get_pPieces();
	int get_aPieces();

	void printBoard();
	void movePiece(bool player, int x, int y, int newx, int newy);
	bool validMove(bool player, int x, int y, int newx, int newy);
	void countPieces();
	int isEnd();
};

