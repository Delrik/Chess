#include "ChessBoard.h"

void ChessBoard::swapPiece(pair<short, short> x, pair<short, short> y)
{
	board[y.first][y.second] = board[x.first][x.second];
	board[x.first][x.second] = 0;
	if (pieces.find(y) != pieces.end()) {
		pieces.erase(y);
	}
	pieces.insert(make_pair(y, (*pieces.find(x)).second));
	pieces.erase(x);
}

bool ChessBoard::sameSign(short x, short y)
{
	return (x < 0) == (y < 0);
}

bool ChessBoard::isLegitPair(pair<short, short> x)
{
	if (x.first > 7 || x.second > 7 || x.first < 0 || x.second < 0) return false;
	return true;
}

bool ChessBoard::isPassant()
{
	if (lastMove.first == make_pair((short)-1, (short)-1) || lastMove.second == make_pair((short)-1, (short)-1)) return false;
	if (pieces[lastMove.second]->id != 6) return false;
	if (abs(lastMove.first.first - lastMove.second.first) != 2) return false;
	return true;
}

bool ChessBoard::movePiece(pair<short, short> from, pair<short, short> to)
{
	if (isGameEnded != 0) throw string("Game over.");
	if (waitingForPromotion) throw string("Promote your pawn at the end of the board please.");
	//isItTheLegitMove & Finding the way.
	if (!isLegitPair(from)) throw string("Wrong cell.");
	if (!isLegitPair(to)) throw string("Wrong cell.");
	bool legitMove = false;
	if (from == to) throw string("This is not the move. I cannot count it if you did not move your piece.");
	auto it = pieces.find(from);
	if (it == pieces.end()) throw string("There are no piece");
	if (board[from.first][from.second] > 0 && !firstPlayerTurn) throw string("This is not your piece. Choose another color.");
	if (board[from.first][from.second] < 0 && firstPlayerTurn) throw string("This is not your piece. Choose another color.");
	vector<pair<short, short>> moves;
	pair<short, short> way;
	short isCheck = willBeCheck(make_pair(from, to));
	if ((isCheck == 1 || isCheck == 2) && firstPlayerTurn) throw string("Checkmate on you. Defend your king.");
	if ((isCheck == -1 || isCheck == 2) && !firstPlayerTurn) throw string("Checkmate on you. Defend your king.");
	isCheck = willBeCheck(make_pair(from, from));
	//Looking for a castling
	if (isCastling(from, to) && !(firstPlayerTurn && (isCheck == 1 || isCheck == 2)) && !(!firstPlayerTurn && (isCheck == -1 || isCheck == 2))) {
		if (to.second > from.second) way = { 0,1 }; else way = { 0,-1 };
		//Looking for an obstacles on the way
		for (pair<short, short> i = { from.first + way.first,from.second + way.second }; i != to; i = { i.first + way.first,i.second + way.second }) {
			if (board[i.first][i.second] != 0) throw string("You cannot do the castling when there are pieces on king's/rook's way.");
		}
		isCheck = willBeCheck(make_pair(from, make_pair(from.first, from.second + 2 * way.second)));
		if ((firstPlayerTurn && isCheck != 1 && isCheck != 2) || (!firstPlayerTurn && isCheck != -1 && isCheck != 2)) {
			swapPiece(from, { from.first,from.second + 2 * way.second });
			pieces[{ from.first, from.second + 2 * way.second }]->moveIt({ from.first,from.second + 2 * way.second });
			swapPiece(to, { from.first,from.second + way.second });
			pieces[{ from.first, from.second + way.second }]->moveIt({ from.first,from.second + way.second });
			firstPlayerTurn = !firstPlayerTurn;
			lastMove = { from,to };
			isCheckMate();
			return true;
		}
		throw string("You cannot do the castling when will be putted into checkmate after the it.");
	}
	if (board[to.first][to.second] != 0) {
		if (sameSign(board[from.first][from.second], board[to.first][to.second])) throw string("You cannot beat your own piece.");
		moves = (*it).second->availableToKill(firstPlayerTurn);
	}
	else {
		moves = (*it).second->availableMoves(firstPlayerTurn);
	}
	//Looking for en passant
	if (isPassant() && (pieces[from]->id == 6)) {
		if (firstPlayerTurn) {
			if (to == make_pair((short)(lastMove.second.first + 1),lastMove.second.second)) {
				swapPiece(lastMove.second, to);
				pieces[to]->moveIt(to);
				swapPiece(from, to);
				pieces[to]->moveIt(to);
				firstPlayerTurn = !firstPlayerTurn;
				lastMove = { from,to };
				isCheckMate();
				return true;
			}
		}
		else {
			if (to == make_pair((short)(lastMove.second.first - 1), lastMove.second.second)) {
				swapPiece(lastMove.second, to);
				pieces[to]->moveIt(to);
				swapPiece(from, to);
				pieces[to]->moveIt(to);
				firstPlayerTurn = !firstPlayerTurn;
				lastMove = { from,to };
				isCheckMate();
				return true;
			}
		}
	}
	if ((*it).second->id != 5) {
		if (abs(to.first - from.first) > (*it).second->stepSize || abs(to.second - from.second) > (*it).second->stepSize) throw string("Selected piece cannpt pass this distance.\nThe king moves exactly one square horizontally, vertically, or diagonally. A special move with the king known as castling is allowed only once per player, per game\nA rook moves any number of vacant squares horizontally or vertically. It also is moved when castling.\nA bishop moves any number of vacant squares diagonally.\nThe queen moves any number of vacant squares horizontally, vertically, or diagonally.\nA pawn moves straight forward one square, if that square is vacant. If it has not yet moved, a pawn also has the option of moving two squares straight forward, provided both squares are vacant. Pawns cannot move backwards.\nPawns are the only pieces that capture differently from how they move. A pawn can capture an enemy piece on either of the two squares diagonally in front of the pawn (but cannot move to those squares if they are vacant).");
		for (pair<short, short> move : moves) {
			if (from.first - to.first == 0) {
				if (move.first == 0 && sameSign(move.second, to.second - from.second)) {
					legitMove = true;
					way = move;
					break;
				}
			}
			if (from.second - to.second == 0) {
				if (move.second == 0 && sameSign(move.first, to.first - from.first)) {
					legitMove = true;
					way = move;
					break;
				}
			}
			if (from.first - to.first == 0 || from.second - to.second == 0 || move.first == 0 || move.second == 0) continue;
			if ((to.second - from.second) / (to.first - from.first) == move.second / move.first) {
				if (sameSign(to.second - from.second, move.second) && sameSign(to.first - from.first, move.first)) {
					legitMove = true;
					way = move;
					break;
				}
			}
		}
		if (!legitMove) throw string("Selected piece cannot move in this direction.\nThe king moves exactly one square horizontally, vertically, or diagonally. A special move with the king known as castling is allowed only once per player, per game\nA rook moves any number of vacant squares horizontally or vertically. It also is moved when castling.\nA bishop moves any number of vacant squares diagonally.\nThe queen moves any number of vacant squares horizontally, vertically, or diagonally.\nA pawn moves straight forward one square, if that square is vacant. If it has not yet moved, a pawn also has the option of moving two squares straight forward, provided both squares are vacant. Pawns cannot move backwards.\nPawns are the only pieces that capture differently from how they move. A pawn can capture an enemy piece on either of the two squares diagonally in front of the pawn (but cannot move to those squares if they are vacant).");
		//Looking for obstacles on the way
		for (pair<short, short> i = { from.first + way.first,from.second + way.second }; i != to; i = { i.first + way.first,i.second + way.second }) {
			if (board[i.first][i.second] != 0) throw string("You cannot move your piece through other pieces.");
		}
		//Moving
		swapPiece(from, to);
		pieces[to]->moveIt(to);
		if (pieces[to]->id == 6 && (to.first == 0 || to.first == 7)) waitingForPromotion = true;
		firstPlayerTurn = !firstPlayerTurn;
	}
	else {
		//isItTheLegitMove
		for (pair<short, short> move : moves) {
			if (to.first - from.first == move.first && to.second - from.second == move.second) {
				legitMove = true;
				break;
			}
		}
		if (!legitMove) throw string("This is the wrong way. The knight cannot move in this direction. Knight has L-pattern in movement and can jump over other pieces.");
		//Moving
		swapPiece(from, to);
		pieces[to]->moveIt(to);
		firstPlayerTurn = !firstPlayerTurn;
	}
	lastMove = { from,to };
	isCheckMate();
	return true;
}

bool ChessBoard::isCastling(pair<short, short> from, pair<short, short> to)
{
	if (board[from.first][from.second] == 1 && board[to.first][to.second] == 3 && pieces[from]->isMoved == false && pieces[to]->isMoved == false) {
		return true;
	}
	if (board[from.first][from.second] == -1 && board[to.first][to.second] == -3 && pieces[from]->isMoved == false && pieces[to]->isMoved == false) {
		return true;
	}
	return false;
}

short ChessBoard::willBeCheck(pair<pair<short, short>, pair<short, short>> m)
	{
	bool firstPlayerCheck, secondPlayerCheck;
	firstPlayerCheck = secondPlayerCheck = false;
	int fromBuf = board[m.first.first][m.first.second], toBuf = board[m.second.first][m.second.second];
	board[m.first.first][m.first.second] = 0;
	board[m.second.first][m.second.second] = fromBuf;
	for (auto piece : pieces) {
		int killStepSize;
		if (piece.second->id == 6) killStepSize = 1;
		else killStepSize = piece.second->stepSize;
		vector<pair<short,short>> moves = piece.second->availableToKill(board[piece.first.first][piece.first.second]>0);
		for (pair<short, short> move : moves) {
			for (int i = 1; i <= killStepSize; i++) {
				if (!isLegitPair({ piece.first.first + move.first * i,piece.first.second + move.second * i })) break;
				if (board[piece.first.first + move.first * i][piece.first.second + move.second * i] != 0) {
					if (!firstPlayerCheck && board[piece.first.first + move.first * i][piece.first.second + move.second * i] == 1) {
						if (board[piece.first.first][piece.first.second] < 0) {
							firstPlayerCheck = true;
						}
					}
					if (!secondPlayerCheck && board[piece.first.first + move.first * i][piece.first.second + move.second * i] == -1) {
						if (board[piece.first.first][piece.first.second] > 0) {
							secondPlayerCheck = true;
						}
					}
					break;
				}
			}
		}
	}
	board[m.first.first][m.first.second] = fromBuf;
	board[m.second.first][m.second.second] = toBuf;
	if (firstPlayerCheck && secondPlayerCheck) return 2;
	if (firstPlayerCheck) return 1;
	if (secondPlayerCheck) return -1;
	return 0;
}

short ChessBoard::isCheckMate()
{
	short check = willBeCheck(make_pair(make_pair(0, 0), make_pair(0, 0)));
	short checkBuf;
	short killStepSize;
	if (firstPlayerTurn) {
		if (check == 1 || check == 2) {
			for (auto piece : pieces) {
				if (board[piece.first.first][piece.first.second] < 0) break;
				if (piece.second->id == 6) killStepSize = 1;
				else killStepSize = piece.second->stepSize;
				vector<pair<short, short>> moves = piece.second->availableToKill(true);
				for (pair<short, short> move : moves) {
					for (int i = 1; i <= killStepSize; i++) {
						if (!isLegitPair({ piece.first.first + move.first * i,piece.first.second + move.second * i })) break;
						if (board[piece.first.first + move.first * i][piece.first.second + move.second * i] != 0) {
							if (board[piece.first.first + move.first * i][piece.first.second + move.second * i] < 0) {
								checkBuf = willBeCheck(make_pair(piece.first, make_pair(piece.first.first + move.first * i, piece.first.second + move.second * i)));
								if (checkBuf != 1 && checkBuf != 2) return 0;
							}
							break;
						} 
					}
				}
			}
			for (auto piece : pieces) {
				if (board[piece.first.first][piece.first.second] < 0) break;
				vector<pair<short, short>> moves = piece.second->availableMoves(true);
				for (pair<short, short> move : moves) {
					for (int i = 1; i <= piece.second->stepSize; i++) {
						if (!isLegitPair({ piece.first.first + move.first * i,piece.first.second + move.second * i })) break;
						if (board[piece.first.first + move.first * i][piece.first.second + move.second * i] == 0) {
							checkBuf = willBeCheck(make_pair(piece.first, make_pair(piece.first.first + move.first * i, piece.first.second + move.second * i)));
							if (checkBuf != 1 && checkBuf != 2) return 0;
						}
						else break;
					}
				}
			}
			isGameEnded = -1;
			return -1;
		}
	}
	else {
		if (check == -1 || check == 2) {
			for (auto piece : pieces) {
				if (board[piece.first.first][piece.first.second] > 0) break;
				if (piece.second->id == 6) killStepSize = 1;
				else killStepSize = piece.second->stepSize;
				vector<pair<short, short>> moves = piece.second->availableToKill(false);
				for (pair<short, short> move : moves) {
					for (int i = 1; i <= killStepSize; i++) {
						if (!isLegitPair({ piece.first.first + move.first * i,piece.first.second + move.second * i })) break;
						if (board[piece.first.first + move.first * i][piece.first.second + move.second * i] != 0) {
							if (board[piece.first.first + move.first * i][piece.first.second + move.second * i] > 0) {
								checkBuf = willBeCheck(make_pair(piece.first, make_pair(piece.first.first + move.first * i, piece.first.second + move.second * i)));
								if (checkBuf != -1 && checkBuf != 2) return 0;
							}
							break;
						}
					}
				}
			}
			for (auto piece : pieces) {
				vector<pair<short, short>> moves = piece.second->availableMoves(false);
				for (pair<short, short> move : moves) {
					if (board[piece.first.first][piece.first.second] > 0) break;
					for (int i = 1; i <= piece.second->stepSize; i++) {
						if (!isLegitPair({ piece.first.first + move.first * i,piece.first.second + move.second * i })) break;
						if (board[piece.first.first + move.first * i][piece.first.second + move.second * i] == 0) {
							checkBuf = willBeCheck(make_pair(piece.first, make_pair(piece.first.first + move.first * i, piece.first.second + move.second * i)));
							if (checkBuf != -1 && checkBuf != 2) return 0;
						}
						else break;
					}
				}
			}
			isGameEnded = 1;
			return 1;
		}
	}
	return 0;
}

void ChessBoard::promoteLastPawn(short id)
{
	switch (id)
	{
	case 2:
		pieces[lastMove.second] = new Queen();
		break;
	case 3:
		pieces[lastMove.second] = new Rook();
		break;
	case 4:
		pieces[lastMove.second] = new Bishop();
		break;
	case 5:
		pieces[lastMove.second] = new Knight();
		break;
	default:
		throw "Wrong id in the promotion";
		break;
	}
	if (!firstPlayerTurn)board[lastMove.second.first][lastMove.second.second] = pieces[lastMove.second]->id;
	else board[lastMove.second.first][lastMove.second.second] = -pieces[lastMove.second]->id;
	waitingForPromotion = false;
}

ChessBoard::ChessBoard()
{
	player1 = new Player(true);
	player2 = new Player(false);
	board[0][0] = board[0][7] = 3;
	pieces.insert(make_pair(make_pair(0, 0), player1->rooks.front()));
	pieces.insert(make_pair(make_pair(0, 7), player1->rooks.back()));
	pieces.insert(make_pair(make_pair(7, 0), player2->rooks.front()));
	pieces.insert(make_pair(make_pair(7, 7), player2->rooks.back()));
	board[0][1] = board[0][6] = 5;
	pieces.insert(make_pair(make_pair(0, 1), player1->knights.front()));
	pieces.insert(make_pair(make_pair(0, 6), player1->knights.back()));
	pieces.insert(make_pair(make_pair(7, 1), player2->knights.front()));
	pieces.insert(make_pair(make_pair(7, 6), player2->knights.back()));
	board[0][2] = board[0][5] = 4;
	pieces.insert(make_pair(make_pair(0, 2), player1->bishops.front()));
	pieces.insert(make_pair(make_pair(0, 5), player1->bishops.back()));
	pieces.insert(make_pair(make_pair(7, 2), player2->bishops.front()));
	pieces.insert(make_pair(make_pair(7, 5), player2->bishops.back()));
	board[0][3] = 2;
	pieces.insert(make_pair(make_pair(0, 3), player1->queens.front()));
	pieces.insert(make_pair(make_pair(7, 3), player2->queens.front()));
	board[0][4] = 1;
	pieces.insert(make_pair(make_pair(0, 4), player1->kings.front()));
	pieces.insert(make_pair(make_pair(7, 4), player2->kings.front()));

	for (int i = 0; i < 8; i++) {
		board[1][i] = 6;
		pieces.insert(make_pair(make_pair(1, i), player1->pawns[i]));
		board[6][i] = -6;
		pieces.insert(make_pair(make_pair(6, i), player2->pawns[i]));
		board[7][i] = -board[0][i];
	}
	for (int i = 2; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = 0;
		}
	}
	firstPlayerTurn = true;
	lastMove = { {-1,-1},{-1,-1} };
	waitingForPromotion = false;
	isGameEnded = 0;
}

ChessBoard::~ChessBoard()
{
	delete player1;
	delete player2;
}
