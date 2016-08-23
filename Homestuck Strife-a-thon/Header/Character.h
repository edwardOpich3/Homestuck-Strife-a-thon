#pragma once
#include "Header\BoundingBox.h"
#include "allegro5\allegro.h"
#include <vector>
#include "Header\Tile.h"
#include "Header\Control.h"

class Character
{
public:
	BoundingBox collisionBox;	// The player's bounding box, relative to the sprite.
	ALLEGRO_BITMAP *sprite;		// The player's sprite
	enum ANIMATION_STATES { IDLE, WALK_TURN, WALK, CROUCH, RUN, RUN_TURN, SLIDE, JUMPSQUAT, JUMP, BACK_JUMP, DOUBLE_JUMP, BACK_DOUBLE_JUMP, FALL, FORWARD_FALL, BACK_FALL, SOFT_LAND, HARD_LAND,
								LEDGE_GRAB, LEDGE_HOLD, NORMAL_CLIMB, JUMP_CLIMB, THROUGH_PLATFORM, WALL_BONK, RUN_OFF, BALANCING };	// The animation states the player can be in; determines the sprite to be displayed

	// Grounded Attributes
	int walkSpeed;	// The player's speed cap when walking
	int runSpeed;	// The player's speed cap when running
	int jumpSquat;	// The length in frames of the player's jumpsquat
	float grndAcc;	// The constant of the player's acceleration when walking
	float runAcc;	// The constant of the player's acceleration when running
	float friction;	// The constant of the player's friction
	float jump;		// The constant of the player's jump acceleration
	float shortJump;	// The constant of the player's short jump acceleration

	// Aerial Attributes
	float gravity;	// The constant of the acceleration due to gravity
	float airDrag;	// The constant of air drag
	float airAcc;	// The constant of the player's acceleration when moving whilst airborne
	int mass;	// The constant of the player's mass; determines how far they fly when hit
	int airSpeed;	// The player's horizontal speed cap whilst airborne
	int fallSpeed;	// The player's vertical speed cap whilst falling
	int fastfallSpeed;	// The player's vertical speed cap whilst fast-falling

	// Change based on input
	int direction;	// The player's current direction; negative for left, positive for right
	float xSpeed, ySpeed;	// The player's current x and y speed

	// Timers
	int jumpSquatTimer;	// The amount of time the player has to release the jump button to perform a short jump, in frames.
	int runTimer;	// The amount of time the player has to double-tap the run button to trigger running mode, in frames. Will later be abolished.
	int crouchTimer;	// The amount of time the player has to double-tap the crouch button to fall through a platform, in frames. Will later be abolished.

	// Flags
	bool isGrounded;	// Is the player grounded?
	bool isAerial;		// Is the player airborne?
	bool isHanging;		// Is the player hanging from a ledge?
	bool canDoubleJump; // Can the player double jump?
	bool isFastFalling;	// Is the player currently fastfalling?
	bool isCrouching;	// Is the player currently holding the down direction?
	bool isRunning;		// Is the player currently running?

	// Drawing purposes
	int x, y, width, height;	// The x and y the sprite needs to be drawn to, and the width and height it needs to be. These differ from the hitbox.

	// Animation Variables
	int mirrorOffset;			// The amount the player sprite needs to be moved left when they turn around.
	int animationState;			// The current animation state of the player
	int animationLengths[25];	// The lengths of each animation state for the player, in frames.
	int frame;					// The current frame of animation.

	// Stage related
	int spawnX;	// The player's horizontal spawn position
	int spawnY;	// The player's vertical spawn position

	// Controls
	Control **control;	// The player's controller.

	Character(int spawnX, int spawnY, Control **control);	// Use this constructor!
	Character();
	virtual ~Character();

	virtual void GetInput();	// Currently does nothing, will likely continue to do nothing until removed.

	virtual void Move(int vector);	// Calculates the player's xSpeed based on whether or not the left/right commands are input
	virtual void Jump(int Z, int LEFT, int RIGHT);	// Calculates the player's ySpeed based on if the jump button is hit
	virtual void FastFall(bool current, bool previous);	// Sets the player's ySpeed to fastfall speed if the conditions for fastfalling are met.
	virtual void Run(bool current, bool previous);	// Puts the player in run mode if the conditions are met.
	virtual void Collision(ALLEGRO_BITMAP** collisionBitmap, int levelWidth, int levelHeight, bool DOWN);	// Calculates the collision data and adjusts the player's position accordingly
	virtual void Update(int Z, int LEFT, int RIGHT);	// Updates the player's position and speed varibales based on constants.
	virtual void Animate(int LEFT, int RIGHT, int DOWN);	// Sets the player's animation state accordingly.
};

