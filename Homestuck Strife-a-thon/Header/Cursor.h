#pragma once
#include "Header/BoundingBox.h"
#include "allegro5\allegro.h"
class Cursor
{
public:
	ALLEGRO_BITMAP *sprite;
	int selection;

	int x, y, width, height;

	Cursor(int x, int y, ALLEGRO_BITMAP *sprite);
	Cursor();
	~Cursor();
};

