#pragma once
#include "Header\BoundingBox.h"
#include "allegro5\allegro.h"
#include <vector>
#include "Header\Tile.h"
#include "Header\Control.h"

class Character
{
public:
	BoundingBox collisionBox;
	ALLEGRO_BITMAP *sprite;
	enum ANIMATION_STATES { IDLE, WALK_TURN, WALK, CROUCH, RUN, RUN_TURN, SLIDE, JUMPSQUAT, JUMP, BACK_JUMP, DOUBLE_JUMP, BACK_DOUBLE_JUMP, FALL, FORWARD_FALL, BACK_FALL, SOFT_LAND, HARD_LAND,
								LEDGE_GRAB, LEDGE_HOLD, NORMAL_CLIMB, JUMP_CLIMB, THROUGH_PLATFORM, WALL_BONK, RUN_OFF, BALANCING };

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
	int crouchTimer;

	// Flags
	bool isGrounded;
	bool isAerial;
	bool isHanging;
	bool canDoubleJump;
	bool isFastFalling;
	bool isCrouching;
	bool isRunning;

	// Drawing purposes
	int x, y, width, height;

	// Animation Variables
	int mirrorOffset;
	int animationState;
	int animationLengths[25];
	int frame;

	// Stage related
	int spawnX;
	int spawnY;

	// Controls
	Control **control;

	Character(int spawnX, int spawnY, Control **control);
	Character();
	virtual ~Character();

	virtual void GetInput();

	virtual void Move(int vector);
	virtual void Jump(std::vector<bool> *buttons, int Z, int LEFT, int RIGHT);
	virtual void FastFall(bool current, bool previous);
	virtual void Run(bool current, bool previous);
	virtual void Collision(ALLEGRO_BITMAP** collisionBitmap, int levelWidth, int levelHeight, bool DOWN);
	virtual void Update(std::vector<bool> *buttons, int Z, int LEFT, int RIGHT);
	virtual void Animate(std::vector<bool> *buttons, int LEFT, int RIGHT, int DOWN);
};

