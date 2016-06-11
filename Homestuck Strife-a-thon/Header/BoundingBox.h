#pragma once
class BoundingBox
{
public:
	int x;
	int y;
	int width;
	int height;

	BoundingBox(int x, int y, int width, int height);
	BoundingBox();
	~BoundingBox();
};

