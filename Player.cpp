#include "Player.h"

Player::Player(bool isItFirstPlayer)
{
	kings = { new King() };
	queens.push_back(new Queen());
	rooks = { new Rook(),new Rook() };
	bishops = { new Bishop(), new Bishop() };
	knights = { new Knight(), new Knight() };
	pawns = { new Pawn(), new Pawn(), new Pawn(), new Pawn(), new Pawn(), new Pawn(), new Pawn(), new Pawn() };
	if (isItFirstPlayer) firstPlayer = 1;
	else firstPlayer = -1;
}

Player::~Player()
{
	for (int i = 0; i < kings.size(); i++) delete kings[i];
	for (int i = 0; i < queens.size(); i++) delete queens[i];
	for (int i = 0; i < rooks.size(); i++) delete rooks[i];
	for (int i = 0; i < bishops.size(); i++) delete bishops[i];
	for (int i = 0; i < knights.size(); i++) delete knights[i];
	for (int i = 0; i < pawns.size(); i++) delete pawns[i];
}
