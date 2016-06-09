#pragma once
#include <allegro5/allegro.h>

class Tile
{
public:
	int type;
	int x, y;
	int dimension;

	Tile();
	Tile(int type, int x, int y, int dimension);
	~Tile();
};

