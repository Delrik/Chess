#pragma once
#include "Pieces.h"
class Player {
public:
	//1 - first player, -1 - second player
	short firstPlayer;
	//Pieces
	vector<Piece*> kings;
	vector<Piece*> queens;
	vector<Piece*> rooks;
	vector<Piece*> bishops;
	vector<Piece*> knights;
	vector<Piece*> pawns;
	Player(bool isItFirstPlayer);
	~Player();
};