#include "Header\BoundingBox.h"




BoundingBox::BoundingBox(int x, int y, int width, int height, int type)
{
	BoundingBox::x = x;
	BoundingBox::y = y;
	BoundingBox::width = width;
	BoundingBox::height = height;
	BoundingBox::type = type;
}

BoundingBox::BoundingBox()
{

}

BoundingBox::~BoundingBox()
{
}
