#pragma once
#include "Header\BoundingBox.h"
#include "allegro5\allegro.h"
class Character
{
public:
	BoundingBox hurtBox;
	ALLEGRO_BITMAP *sprite;

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

	Character(ALLEGRO_BITMAP *sprite);
	Character();
	~Character();

	virtual void Move(int vector);
	virtual void Jump();
	virtual void FastFall();
	virtual void Run(bool current, bool previous);
	virtual void Update(bool buttons[6], int Z);
};

