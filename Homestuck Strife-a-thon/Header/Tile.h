#pragma once
#include <allegro5/allegro.h>

// This class represents a tile used to make up a stage.

class Tile
{
public:
	int type;	// The type of tile.
	int x, y;	// The x and y position of the tile.
	int dimension;	// The tile's size; 16, 32, 64, or 128.

	Tile();
	Tile(int type, int x, int y, int dimension);	// Use this constructor.
	~Tile();
};

