#pragma once
#include <string>
#include <vector>

using namespace std;

class Piece {
protected:
	static string getName(short id);
public:
	short id;
	string name;
	short stepSize;
	bool isMoved;
	pair<short, short> position;
	Piece(short id);
	virtual vector<pair<short, short>> availableMoves() = 0;
	virtual vector<pair<short, short>> availableToKill();
	virtual void moveIt(pair<short,short> pos);
};
class King : public Piece {
public:
	King(bool isMoved = false);
	vector<pair<short, short>> availableMoves() override;
};
class Queen : public Piece {
public:
	Queen();
	vector<pair<short, short>> availableMoves() override;
};
class Rook : public Piece {
public:
	Rook(bool isMoved = false);
	vector<pair<short, short>> availableMoves() override;
};
class Bishop : public Piece {
public:
	Bishop();
	vector<pair<short, short>> availableMoves() override;
};
class Knight : public Piece {
public:
	Knight();
	vector<pair<short, short>> availableMoves() override;
};
class Pawn : public Piece {
public:
	Pawn(bool isMoved = false);
	vector<pair<short, short>> availableMoves() override;
	vector<pair<short, short>> availableToKill() override;
	void moveIt(pair<short,short> pos) override;
};