#pragma once

// A class to represent a simple collision rectangle.

class BoundingBox
{
public:
	int x;	// Horizontal origin
	int y;	// Vertical origin
	int width;	// Width of the rectangle
	int height;	// Height of the rectangle

	BoundingBox(int x, int y, int width, int height);	// Use this constructor
	BoundingBox();
	~BoundingBox();
};

