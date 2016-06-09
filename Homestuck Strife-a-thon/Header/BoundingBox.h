#pragma once
class BoundingBox
{
public:
	enum BOUNDINGBOXTYPE {HITBOX, HURTBOX, WINDBOX};
	int x;
	int y;
	int width;
	int height;
	int type;

	BoundingBox(int x, int y, int width, int height, int type);
	BoundingBox();
	~BoundingBox();
};

