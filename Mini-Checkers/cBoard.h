#pragma once
#include <iostream>
#include <queue>

class cBoard
{
private:
	char** board;
	int pPieces;
	int aPieces;

	int util;
	cBoard* next;
	bool endGame;
public:
	cBoard();
	~cBoard();
	cBoard(const cBoard* lhs);

	char** get_board() const;
	char get_index(int i, int j) const;
	int get_pPieces() const;
	int get_aPieces() const;
	int get_util() const;
	bool get_end() const;
	cBoard* get_next() const;

	void set_util(int newutil);
	void set_next(cBoard* newnext);
	void del_next();

	void printBoard();
	void movePiece(bool player, int x, int y, int newx, int newy);
	bool validMove(bool player, int x, int y, int newx, int newy);
	void countPieces();
	bool isEnd();
	int utilEval();
	bool availMoves(bool player);
	bool availJump(bool player);
	void genActions(bool player, std::queue <cBoard*>& actions);
};

