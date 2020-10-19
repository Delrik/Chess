#include "ChessBoard.h"

void ChessBoard::swapPiece(pair<short, short> x, pair<short, short> y)
{
	board[y.first][y.second] = board[x.first][x.second];
	board[x.first][x.second] = 0;
	if (pieces.find(y) != pieces.end()) {
		//delete ((*pieces.find(y)).second);
		pieces.erase(y);
	}
	pieces.insert(make_pair(y, (*pieces.find(x)).second));
	pieces.erase(x);
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
	//isItTheLegitMove & Finding the way.
	if (!isLegitPair(from)) return false;
	if (!isLegitPair(to)) return false;
	bool legitMove = false;
	if (from == to) return false;
	auto it = pieces.find(from);
	if (it == pieces.end()) return false;
	if (board[from.first][from.second] > 0 && !firstPlayerTurn) return false;
	if (board[from.first][from.second] < 0 && firstPlayerTurn) return false;
	vector<pair<short, short>> moves;
	pair<short, short> way;
	//Looking for castling
	if (isCastling(from, to)) {
		if (to.second > from.second) way = { 0,1 }; else way = { 0,-1 };
		//Looking for obstacles on the way
		for (pair<short, short> i = { from.first + way.first,from.second + way.second }; i != to; i = { i.first + way.first,i.second + way.second }) {
			if (board[i.first][i.second] != 0) return false;
		}
		swapPiece(from, { from.first,from.second + 2 * way.second });
		pieces[{ from.first, from.second + 2 * way.second }]->moveIt({ from.first,from.second + 2 * way.second });
		swapPiece(to, { from.first,from.second + way.second });
		pieces[{ from.first, from.second + way.second }]->moveIt({ from.first,from.second + way.second });
		firstPlayerTurn = !firstPlayerTurn;
		lastMove = { from,to };
		return true;
	}
	if (board[to.first][to.second] != 0) {
		if (sameSign(board[from.first][from.second], board[to.first][to.second])) return false;
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
				return true;
			}
		}
	}
	if ((*it).second->id != 5) {
		if (abs(to.first - from.first) > (*it).second->stepSize || abs(to.second - from.second) > (*it).second->stepSize) return false;
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
		if (!legitMove) return false;
		//Looking for obstacles on the way
		for (pair<short, short> i = { from.first + way.first,from.second + way.second }; i != to; i = { i.first + way.first,i.second + way.second }) {
			if (board[i.first][i.second] != 0) return false;
		}
		//Moving
		swapPiece(from, to);
		pieces[to]->moveIt(to);
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
		if (!legitMove) return false;
		//Moving
		swapPiece(from, to);
		pieces[to]->moveIt(to);
		firstPlayerTurn = !firstPlayerTurn;
	}
	lastMove = { from,to };
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
}

ChessBoard::~ChessBoard()
{
	delete player1;
	delete player2;
	/*
	for (auto el : pieces) {
		delete el.second;
	}*/
}
