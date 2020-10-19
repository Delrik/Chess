#pragma once
#include "Player.h"
#include <map>

class ChessBoard {
private:
	Player *player1, *player2;
	pair<pair<short, short>, pair<short, short>> lastMove;
	void swapPiece(pair<short, short> x, pair<short, short> y);
	static bool sameSign(short x, short y);
	static bool isLegitPair(pair<short, short> x);
	bool isPassant();
	bool isCastling(pair<short, short> from, pair<short, short> to);
	map<pair<short, short>, Piece*> pieces;
public:
	bool waitingForPromotion;
	void promoteLastPawn(short id);
	bool firstPlayerTurn;
	int board[8][8];
	bool movePiece(pair<short, short> from, pair<short, short> to);
	ChessBoard();
	~ChessBoard();
};