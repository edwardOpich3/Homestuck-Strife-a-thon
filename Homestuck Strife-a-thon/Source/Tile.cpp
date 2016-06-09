#include "..\Header\Tile.h"



Tile::Tile()
{
}


Tile::Tile(int type, int x, int y, int dimension)
{
	Tile::type = type;
	Tile::x = x;
	Tile::y = y;
	Tile::dimension = dimension;
}


Tile::~Tile()
{
}
