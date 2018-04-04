#include <iostream>
#include "cBoard.h"
using namespace std;

int main() {
	cBoard game;
	game.printBoard();
	cout << endl << game.get_aPieces() << endl << game.get_pPieces() << endl;
	cout << endl;
	game.movePiece(true, 3, 1, 2, 2);
	cout << endl;
	game.printBoard();
	game.movePiece(true, 5, 1, 4, 2);
	cout << endl;
	game.printBoard();
	game.movePiece(true, 4, 0, 5, 1);
	cout << endl;
	game.printBoard();
	game.movePiece(false, 4, 1, 3, 2);
	cout << endl << game.get_aPieces() << endl << game.get_pPieces() << endl;
	cout << endl;
	game.printBoard();
	game.movePiece(true, 2, 2, 4, 4);
	cout << endl << game.get_aPieces() << endl << game.get_pPieces() << endl;
	cout << endl;
	game.printBoard();
	game.movePiece(false, 5, 0, 3, 2);
	cout << endl;
	game.printBoard();
	game.movePiece(true, 2, 0, 0, 2);
	cout << endl;
	game.printBoard();

	cout << endl << game.get_aPieces() << endl << game.get_pPieces() << endl;

	return 0;
}