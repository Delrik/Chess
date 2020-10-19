#pragma once
#include "ChessBoard.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Game {
private:
	vector<Sprite> sprites;
	vector<Texture> textures;
	RectangleShape rectangles[8][8];
	void setDefaultColorForCell(pair<short, short> cell);
	pair<short, short> selectedCell;
	void selectCell (pair<short, short> cell);
	bool isSelected;
	bool needToRedraw;
	void initBoard();
	void loadTextures();
	void loadSprites();
	void redrawWindow(RenderWindow& window);
	ChessBoard game;
	void drawPromotion(RenderWindow& window);
public:
	Game();
};