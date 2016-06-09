#include "Header\Character.h"



Character::Character(ALLEGRO_BITMAP *sprite)
{
	// Set the necessary base values to their starting values
	Character::x = 128;
	Character::y = 128;
	Character::width = al_get_bitmap_width(sprite);
	Character::height = al_get_bitmap_height(sprite);
	Character::sprite = sprite;
	direction = 1;
	xSpeed = 0;
	ySpeed = 0;
	jumpSquatTimer = 0;
	runTimer = 0;
	isGrounded = false;
	isAerial = true;
	canDoubleJump = false;
	isFastFalling = false;
	isCrouching = false;
	isRunning = false;
}

Character::Character()
{

}

Character::~Character()
{
}

// The player has hit either left or right, adjust their horizontal velocity accordingly
void Character::Move(int vector)
{
	// If the player is on the ground and isn't about to jump
	if (isGrounded && jumpSquatTimer == 0)
	{
		// If they are running
		if (isRunning)
		{
			// Increase their speed by the running acceleration (in the direction of movement)
			xSpeed += vector * runAcc;
		}
		// Otherwise
		else
		{
			// Increase their speed by the walking acceleration
			xSpeed += vector * grndAcc;
		}
		// If they are going to the left
		if (xSpeed < 0)
		{
			// Turn them to face the left
			direction = -1;
		}
		// Otherwise if they are going to the right
		else if (xSpeed > 0)
		{
			// Turn them to face the right
			direction = 1;
		}
	}
	// Otherwise if they are in the air
	else if (isAerial)
	{
		// Increase their speed by the aerial acceleration
		xSpeed += vector * airAcc;
	}
}

// The player has hit the jump button, either put them into jumpsquat, make them double jump, or do nothing depending on the circumstances
void Character::Jump()
{
	// If they are on the ground
	if (isGrounded)
	{
		// If they aren't currently attempting to jump
		if (jumpSquatTimer == 0)
		{
			// Start the jumpsquat
			jumpSquatTimer = jumpSquat;
		}
	}
	// Otherwise if they can double jump (implied to be in the air)
	else if (canDoubleJump)
	{
		// Set their ySpeed to that of the jump speed
		ySpeed = jump;

		// Set the double jump and fastfall flags to false, as they've used their double jump, and double jumping cancels out fastfalling
		canDoubleJump = false;
		isFastFalling = false;
	}
}

// The player has hit the down button, for now this will be exclusively for fastfalling but later this should be appended for ducking as well
void Character::FastFall()
{
	// If the player is falling and isn't already holding the down button
	if (ySpeed > 0 && !isCrouching)
	{
		// They are now fastfalling; adjust their yspeed accordingly
		isFastFalling = true;
		ySpeed = fastfallSpeed;
	}
	// They are now considered to be holding the down button
	isCrouching = true;
}

void Character::Run(bool current, bool previous)
{
	if (isGrounded)
	{
		if (current)
		{
			if (!previous && runTimer > 0)
			{
				isRunning = true;
			}
			runTimer = 10;
		}
		else if(previous)
		{
			runTimer = 10;
		}
	}
}

void Character::Update(bool buttons[6], int Z)
{
	if (jumpSquatTimer > 0)
	{
		jumpSquatTimer--;
		if (jumpSquatTimer == 0)
		{
			if (buttons[Z])
			{
				ySpeed = jump;
			}
			else
			{
				ySpeed = shortJump;
			}
			isGrounded = false;
			isAerial = true;
			isRunning = false;
			buttons[Z] = false;
		}
	}

	if (runTimer > 0)
	{
		runTimer--;
	}

	if (isGrounded)
	{
		if (xSpeed > 0)
		{
			xSpeed -= friction;
			if (xSpeed < 0)
			{
				xSpeed = 0;
			}
		}
		else if (xSpeed < 0)
		{
			xSpeed += friction;
			if (xSpeed > 0)
			{
				xSpeed = 0;
			}
		}
		if (!isRunning)
		{
			if (xSpeed > walkSpeed)
			{
				xSpeed = walkSpeed;
			}
			else if (xSpeed < -walkSpeed)
			{
				xSpeed = -walkSpeed;
			}
		}
		else
		{
			if (xSpeed > runSpeed)
			{
				xSpeed = runSpeed;
			}
			else if (xSpeed < -runSpeed)
			{
				xSpeed = -runSpeed;
			}
			if (xSpeed == 0)
			{
				isRunning = false;
			}
		}
	}
	else if (isAerial)
	{
		if (xSpeed > 0)
		{
			xSpeed -= airDrag;
			if (xSpeed < 0)
			{
				xSpeed = 0;
			}
		}
		else if (xSpeed < 0)
		{
			xSpeed += airDrag;
			if (xSpeed > 0)
			{
				xSpeed = 0;
			}
		}
		if (xSpeed > airSpeed)
		{
			xSpeed = airSpeed;
		}
		else if (xSpeed < -airSpeed)
		{
			xSpeed = -airSpeed;
		}
	}

	if (isAerial)
	{
		ySpeed += gravity;
	}

	if (ySpeed > fallSpeed && !isFastFalling)
	{
		ySpeed = fallSpeed;
	}
	else if (ySpeed > fastfallSpeed)
	{
		ySpeed = fastfallSpeed;
	}

	x += xSpeed;
	y += ySpeed;

	if (y + height > 420)
	{
		y = 420 - height;
		isGrounded = true;
		isAerial = false;
		canDoubleJump = true;
		isFastFalling = false;
		ySpeed = 0;
	}
}
