#pragma once
#include "Player.h"
#include <map>

class ChessBoard {
private:
	void swapPiece(pair<short, short> x, pair<short, short> y);
	static bool sameSign(short x, short y) {
		return (x < 0) == (y < 0);
	}
	static bool isLegitPair(pair<short, short> x);
public:
	bool firstPlayerTurn;
	int board[8][8];
	map<pair<short, short>, Piece*> pieces;
	Player *player1, *player2;
	bool isPassant();
	pair<pair<short, short>, pair<short, short>> lastMove;
	bool movePiece(pair<short,short> from, pair<short, short> to);
	bool isCastling(pair<short, short> from, pair<short, short> to);
	ChessBoard();
	~ChessBoard();
};