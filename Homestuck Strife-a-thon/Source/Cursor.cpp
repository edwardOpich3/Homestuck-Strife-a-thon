#include "..\Header\Cursor.h"



Cursor::Cursor()
{
}

Cursor::Cursor(int x, int y, ALLEGRO_BITMAP *sprite)
{
	Cursor::x = x;
	Cursor::y = y;
	Cursor::sprite = sprite;
	Cursor::width = al_get_bitmap_width(Cursor::sprite);
	Cursor::height = al_get_bitmap_height(Cursor::sprite);
	selection = 0;
	offset = 0;
	offsetLimit = 2;
}


Cursor::~Cursor()
{
}
