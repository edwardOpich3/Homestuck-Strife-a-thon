#pragma once
#include "Header/BoundingBox.h"
#include "allegro5\allegro.h"

// A class to represent a cursor.

class Cursor
{
public:
	ALLEGRO_BITMAP *sprite;	// The cursor's sprite
	int selection;			// The cursor's current selection.

	int x, y, width, height;	// The sprite's x position, y position, width, and height of the sprite.

	Cursor(int x, int y, ALLEGRO_BITMAP *sprite);	// Use this constructor.
	Cursor();
	~Cursor();
};

