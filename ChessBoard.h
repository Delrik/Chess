#pragma once
#include "Player.h"
#include <map>

class ChessBoard {
private:
	bool firstPlayerTurn;
	void swapPiece(pair<short, short> x, pair<short, short> y);
	static bool sameSign(short x, short y) {
		return (x < 0) == (y < 0);
	}
	static bool isLegitPair(pair<short, short> x);
public:
	int board[8][8];
	map<pair<short, short>, Piece*> pieces;
	Player *player1, *player2;
	bool movePiece(pair<short,short> from, pair<short, short> to);
	ChessBoard();
	~ChessBoard();
};