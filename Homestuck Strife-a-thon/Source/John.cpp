#include "..\Header\John.h"



John::John(ALLEGRO_BITMAP *sprite) : Character(sprite)
{
	collisionBox = BoundingBox(66, 73, 87, 115);
	walkSpeed = 3;
	runSpeed = 6;
	jumpSquat = 6;
	grndAcc = 1.0;
	runAcc = 1.5;
	friction = 0.25;
	jump = -20.0;
	shortJump = -15.0;

	gravity = 1.0;
	airDrag = 0.05;
	airAcc = 0.5;
	mass = 20;
	airSpeed = 4;
	fallSpeed = 8;
	fastfallSpeed = 12;

	mirrorOffset = 37;
}

John::John()
{
}

John::~John()
{
}