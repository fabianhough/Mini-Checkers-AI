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
	cBoard(const cBoard &lhs);

	char** get_board() const;
	char get_index(int i, int j) const;
	int get_pPieces() const;
	int get_aPieces() const;

	void printBoard();
	void movePiece(bool player, int x, int y, int newx, int newy);
	bool validMove(bool player, int x, int y, int newx, int newy);
	void countPieces();
	int isEnd();
	int utilEval();
	bool availMoves(bool player);
};

