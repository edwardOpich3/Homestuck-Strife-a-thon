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
	jump = -15.0;
	shortJump = -11.0;

	gravity = 0.5;
	airDrag = 0.05;
	airAcc = 0.5;
	mass = 20;
	airSpeed = 4;
	fallSpeed = 12;
	fastfallSpeed = 14;

	mirrorOffset = 37;

	// Set the frame data
	animationLengths[IDLE] = 60;
	animationLengths[WALK_TURN] = 5;
	animationLengths[WALK] = 60;
	animationLengths[CROUCH] = 5;
	animationLengths[RUN] = 60;
	animationLengths[RUN_TURN] = 15;
	animationLengths[SLIDE] = 30;
	animationLengths[JUMPSQUAT] = jumpSquat;
	animationLengths[JUMP] = 60;
	animationLengths[BACK_JUMP] = 60;
	animationLengths[DOUBLE_JUMP] = 60;
	animationLengths[BACK_DOUBLE_JUMP] = 60;
	animationLengths[FALL] = 60;
	animationLengths[FORWARD_FALL] = 60;
	animationLengths[BACK_FALL] = 60;
	animationLengths[SOFT_LAND] = 5;
	animationLengths[HARD_LAND] = 5;
}

John::John()
{
}

John::~John()
{
}