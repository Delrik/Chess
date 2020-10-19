#include <iostream>
#include "ChessBoard.h"

int main() {
	ChessBoard* a;
	short aa, bb, c, d;
	a = new ChessBoard();
	while (true) {
		system("cls");
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				cout << a->board[i][j] << "\t";
			}
			cout << endl;
		}
		cin >> aa >> bb >> c >> d;
		a->movePiece({ aa,bb }, { c,d });
	}
	return 0;
}