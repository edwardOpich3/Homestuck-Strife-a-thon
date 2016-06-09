#include "..\Header\Rose.h"



Rose::Rose(ALLEGRO_BITMAP *sprite) : Character(sprite)
{
	walkSpeed = 4;
	runSpeed = 6;
	jumpSquat = 4;
	grndAcc = 1.5;
	runAcc = 2.0;
	friction = 0.25;
	jump = -15.0;
	shortJump = -11.25;

	gravity = 0.75;
	airDrag = 0.05;
	airAcc = 0.75;
	mass = 10;
	airSpeed = 5;
	fallSpeed = 6;
	fastfallSpeed = 8;
}

Rose::Rose()
{
}

Rose::~Rose()
{
}