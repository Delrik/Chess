#include "Game.h"

void Game::setDefaultColorForCell(pair<short, short> cell)
{
	short i = cell.first;
	short j = cell.second;
	if (i % 2 == j % 2) {
		rectangles[i][j].setFillColor(Color(255, 206, 159));
	}
	else {
		rectangles[i][j].setFillColor(Color(210, 139, 72));
	}
}

void Game::selectCell(pair<short, short> cell)
{
	if (game.firstPlayerTurn) {
		if (game.board[cell.first][cell.second] > 0) {
			selectedCell = cell;
			isSelected = true;
			rectangles[cell.first][cell.second].setFillColor(Color::Magenta);
			needToRedraw = true;
		}
	}
	else {
		if (game.board[cell.first][cell.second] < 0) {
			selectedCell = cell;
			isSelected = true;
			rectangles[cell.first][cell.second].setFillColor(Color::Magenta);
			needToRedraw = true;
		}
	}
}

void Game::initBoard()
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (i % 2 == j % 2) {
				rectangles[i][j].setFillColor(Color(255, 206, 159));
			}
			else {
				rectangles[i][j].setFillColor(Color(210, 139, 72));
			}
			rectangles[i][j].setPosition(j * 64, i * 64);
			rectangles[i][j].setSize(Vector2f(64, 64));
		}
	}
}
void Game::loadTextures()
{
	for (int i = 0; i < 6; i++) {
		textures.push_back(Texture());
		textures.back().loadFromFile("images/" + Piece::getName(i + 1) + ".png", IntRect(0, 0, 512, 512));
	}
	for (int i = 0; i < 6; i++) {
		textures.push_back(Texture());
		textures.back().loadFromFile("images/" + Piece::getName(i + 1) + "-2.png", IntRect(0, 0, 512, 512));
	}
}

void Game::loadSprites()
{
	sprites.clear();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (game.board[i][j] != 0) {
				if (game.board[i][j] > 0) sprites.push_back(Sprite((textures[game.board[i][j] - 1])));
				else sprites.push_back(Sprite(textures[abs(game.board[i][j]) + 6 - 1]));
				sprites.back().setScale(0.125f, 0.125f);
				sprites.back().setPosition(j * 64, i * 64);
			}
		}
	}
}

void Game::redrawWindow(RenderWindow& window)
{
	loadSprites();
	//drawClearBoard(window);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			window.draw(rectangles[i][j]);
		}
	}
	for (int i = 0; i < sprites.size(); i++) {
		window.draw(sprites[i]);
	}
	window.display();
}
Game::Game()
{
	RenderWindow window(sf::VideoMode(512, 512), "Chess", Style::Titlebar | Style::Close);
	window.setFramerateLimit(60);
	window.clear(Color::White);
	window.setActive(true);

	selectedCell = { -1,-1 };
	isSelected = false;

	initBoard();
	loadTextures();
	needToRedraw = true;

	while (window.isOpen()) {
		if (needToRedraw) {
			redrawWindow(window);
			needToRedraw = false;
		}
		Vector2i mousePos = Mouse::getPosition(window);
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					if (!isSelected) {
						selectCell({ mousePos.y / 64, mousePos.x / 64 });
					}
					else {
						if (game.movePiece(selectedCell, { mousePos.y / 64, mousePos.x / 64 }));
						setDefaultColorForCell(selectedCell);
						isSelected = false;
						selectedCell = { -1,-1 };
						needToRedraw = true;
					}
				}
				if (event.mouseButton.button == Mouse::Right) {
					if (isSelected) {
						setDefaultColorForCell(selectedCell);
						isSelected = false;
						selectedCell = { -1,-1 };
						needToRedraw = true;
					}
				}
			}
		}
	}
}
