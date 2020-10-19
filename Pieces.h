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
	virtual vector<pair<short, short>> availableMoves(bool firstPlayer = true) = 0;
	virtual vector<pair<short, short>> availableToKill(bool firstPlayer = true);
	virtual void moveIt(pair<short,short> pos);
};
class King : public Piece {
public:
	King(bool isMoved = false);
	vector<pair<short, short>> availableMoves(bool firstPlayer = true) override;
};
class Queen : public Piece {
public:
	Queen();
	vector<pair<short, short>> availableMoves(bool firstPlayer = true) override;
};
class Rook : public Piece {
public:
	Rook(bool isMoved = false);
	vector<pair<short, short>> availableMoves(bool firstPlayer = true) override;
};
class Bishop : public Piece {
public:
	Bishop();
	vector<pair<short, short>> availableMoves(bool firstPlayer = true) override;
};
class Knight : public Piece {
public:
	Knight();
	vector<pair<short, short>> availableMoves(bool firstPlayer = true) override;
};
class Pawn : public Piece {
public:
	Pawn(bool isMoved = false);
	vector<pair<short, short>> availableMoves(bool firstPlayer = true) override;
	vector<pair<short, short>> availableToKill(bool firstPlayer = true) override;
	void moveIt(pair<short,short> pos) override;
};