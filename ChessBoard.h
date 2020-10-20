#pragma once
#include "Player.h"
#include <map>

class ChessBoard {
private:
	Player *player1, *player2;
	pair<pair<short, short>, pair<short, short>> lastMove;
	void swapPiece(pair<short, short> x, pair<short, short> y);
	static int sign(int value);
	static bool sameSign(short x, short y);
	static bool isLegitPair(pair<short, short> x);
	bool isPassant();
	bool isPassant(pair<short, short> pawn);
	bool isCastling(pair<short, short> from, pair<short, short> to);
	map<pair<short, short>, Piece*> pieces;
	bool fromToObstacles(pair<short, short> from, pair<short, short> to, pair<short, short> way);
	//Will be the check if you're going to do the move m(-1 - check to the first player, 0 - no check, 1 - check to the second player, 2 - check for both players)
	short willBeCheck(pair<pair<short,short>,pair<short,short>> m);
	short isCheckMate();
public:
	vector<pair<short, short>> availableToMove(pair<short, short> piece);
	static bool difSign(short x, short y);
	short isGameEnded;
	bool waitingForPromotion;
	void promoteLastPawn(short id);
	bool firstPlayerTurn;
	int board[8][8];
	bool movePiece(pair<short, short> from, pair<short, short> to);
	ChessBoard();
	~ChessBoard();
};