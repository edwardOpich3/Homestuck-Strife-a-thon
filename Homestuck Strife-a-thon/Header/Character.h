#pragma once
#include "Header\BoundingBox.h"
#include "allegro5\allegro.h"
#include <vector>
#include "Header\Tile.h"

class Character
{
public:
	BoundingBox collisionBox;
	ALLEGRO_BITMAP *sprite;
	enum ANIMATION_STATES { IDLE, WALK_TURN, WALK, CROUCH, RUN, RUN_TURN, SLIDE, JUMPSQUAT, JUMP, BACK_JUMP, DOUBLE_JUMP, BACK_DOUBLE_JUMP, FALL, FORWARD_FALL, BACK_FALL, SOFT_LAND, HARD_LAND };

	// Grounded Attributes
	int walkSpeed;
	int runSpeed;
	int jumpSquat;
	float grndAcc;
	float runAcc;
	float friction;
	float jump;
	float shortJump;

	// Aerial Attributes
	float gravity;
	float airDrag;
	float airAcc;
	int mass;
	int airSpeed;
	int fallSpeed;
	int fastfallSpeed;

	// Change based on input
	int direction;
	float xSpeed, ySpeed;

	// Timers
	int jumpSquatTimer;
	int runTimer;

	// Flags
	bool isGrounded;
	bool isAerial;
	bool canDoubleJump;
	bool isFastFalling;
	bool isCrouching;
	bool isRunning;

	// Drawing purposes
	int x, y, width, height;

	// Animation Variables
	int mirrorOffset;
	int animationState;
	int frame;

	Character(ALLEGRO_BITMAP *sprite);
	Character();
	virtual ~Character();

	virtual void Move(int vector);
	virtual void Jump(bool buttons[6], int Z);
	virtual void FastFall();
	virtual void Run(bool current, bool previous);
	virtual void Collision(ALLEGRO_BITMAP** collisionBitmap, int levelWidth, int levelHeight);
	virtual void Update(bool buttons[6], int Z);
	virtual void Animate(bool buttons[6], int LEFT, int RIGHT);
};

