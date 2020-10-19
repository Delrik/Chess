#include "Pieces.h"

string Piece::getName(short id)
{
	switch (id)
	{
	case 1:
		return "King";
	case 2:
		return "Queen";
	case 3:
		return "Rook";
	case 4:
		return "Bishop";
	case 5:
		return "Knight";
	case 6:
		return "Pawn";
	default:
		return "Not a piece";
	}
}

Piece::Piece(short id)
{
	position = { -1,-1 };
	this->id = id;
	this->name = getName(id);
}

vector<pair<short, short>> Piece::availableToKill()
{
	return availableMoves();
}

void Piece::moveIt(pair<short,short> pos)
{
	position = pos;
	isMoved = true;
}

King::King(bool isMoved) : Piece(1)
{
	this->isMoved = isMoved;
	stepSize = 1;
}

vector<pair<short, short>> King::availableMoves()
{
	return { {0,-1},{0,1},{1,0},{-1,0},{1,1},{-1,-1},{-1,1},{1,-1} };
}

Queen::Queen() : Piece(2)
{
	stepSize = 8;
}

vector<pair<short, short>> Queen::availableMoves()
{
	return { {0,-1},{0,1},{1,0},{-1,0},{1,1},{-1,-1},{-1,1},{1,-1} };
}

Rook::Rook(bool isMoved) : Piece(3)
{
	this->isMoved = isMoved;
	stepSize = 8;
}

vector<pair<short, short>> Rook::availableMoves()
{
	return { {0,-1},{0,1},{1,0},{-1,0} };
}

Bishop::Bishop() : Piece(4)
{
	stepSize = 8;
}

vector<pair<short, short>> Bishop::availableMoves()
{
	return { {1,1},{-1,-1},{-1,1},{1,-1} };
}

Knight::Knight() : Piece(5)
{
	stepSize = 1;
}

vector<pair<short, short>> Knight::availableMoves()
{
	return { {1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2} };
}

Pawn::Pawn(bool isMoved) : Piece(6)
{
	this->isMoved = isMoved;
	if (isMoved) stepSize = 1;
	else stepSize = 2;
}

vector<pair<short, short>> Pawn::availableMoves()
{
	return { {1,0} };
}

vector<pair<short, short>> Pawn::availableToKill()
{
	return { {1,1},{1,-1} };
}

void Pawn::moveIt(pair<short, short> pos)
{
	position = pos;
	if (!isMoved) {
		isMoved = true;
		stepSize = 1;
	}
}
