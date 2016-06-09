#include "..\Header\Cursor.h"



Cursor::Cursor()
{
}

Cursor::Cursor(int x, int y, ALLEGRO_BITMAP *sprite)
{
	Cursor::x = x;
	Cursor::y = y;
	Cursor::width = al_get_bitmap_width(sprite);
	Cursor::height = al_get_bitmap_height(sprite);
	Cursor::sprite = sprite;
	selection = 0;
}


Cursor::~Cursor()
{
}
