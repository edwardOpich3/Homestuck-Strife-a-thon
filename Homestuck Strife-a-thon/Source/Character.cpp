#include "Header\Character.h"



Character::Character(ALLEGRO_BITMAP *sprite)
{
	// Set the necessary base values to their starting values
	Character::x = 128;
	Character::y = 128;
	Character::width = 256;
	Character::height = 256;
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
	frame = 0;
	animationState = FALL;
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
void Character::Jump(bool buttons[6], int Z)
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
		buttons[Z] = false;
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

void Character::Collision(ALLEGRO_BITMAP** collisionBitmap, int levelWidth, int levelHeight)
{
	al_lock_bitmap(*collisionBitmap, al_get_bitmap_format(*collisionBitmap), ALLEGRO_LOCK_READONLY);
	ALLEGRO_COLOR levelPixel;
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;

	if (isGrounded)
	{
		// First we check for walls
		// Left
		bool collision = false;
		for (int leftWall = x + collisionBox.x + (collisionBox.width / 2); leftWall > x + collisionBox.x - 1 && !collision; leftWall--)
		{
			if (leftWall > 0 && leftWall < levelWidth && y + collisionBox.y + collisionBox.height - 20 > 0 && y + collisionBox.y + collisionBox.height - 20 < levelHeight)
			{
				levelPixel = al_get_pixel(*collisionBitmap, leftWall, y + collisionBox.y + collisionBox.height - 20);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					x = leftWall - collisionBox.x + 1;
					if (xSpeed < 0)
					{
						xSpeed = 0;
					}
					collision = true;
				}
			}
		}

		// Right
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (int rightWall = x + collisionBox.x + (collisionBox.width / 2); rightWall < x + collisionBox.x + collisionBox.width + 1 && !collision; rightWall++)
		{
			if (rightWall > 0 && rightWall < levelWidth && y + collisionBox.y + collisionBox.height - 20 > 0 && y + collisionBox.y + collisionBox.height - 20 < levelHeight)
			{
				levelPixel = al_get_pixel(*collisionBitmap, rightWall, y + collisionBox.y + collisionBox.height - 20);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					x = rightWall - collisionBox.x - collisionBox.width - 1;
					if (xSpeed > 0)
					{
						xSpeed = 0;
					}
					collision = true;
				}
			}
		}

		// Then we check the ground below us
		// Left Sensor
		int leftGround, rightGround;
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (leftGround = y + collisionBox.y + ((3 * collisionBox.height) / 4); leftGround < y + collisionBox.y + collisionBox.height + 16; leftGround++)
		{
			if (leftGround <= y + collisionBox.y + collisionBox.height)
			{
				for (int i = collisionBox.width / 2; i >= 0; i--)
				{
					if (leftGround > 0 && leftGround < levelHeight && x + collisionBox.x + i > 0 && x + collisionBox.x + i < levelWidth)
					{
						levelPixel = al_get_pixel(*collisionBitmap, x + collisionBox.x + i, leftGround);
						al_unmap_rgba(levelPixel, &r, &g, &b, &a);

						if (a > 0)
						{
							collision = true;
							break;
						}
					}
				}
				if (collision)
				{
					break;
				}
			}
			else if (leftGround > 0 && leftGround < levelHeight && x + collisionBox.x > 0 && x + collisionBox.x < levelWidth)
			{
				levelPixel = al_get_pixel(*collisionBitmap, x + collisionBox.x, leftGround);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					collision = true;
					break;
				}
			}
		}

		// Right Sensor
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (rightGround = y + collisionBox.y + ((3 * collisionBox.height) / 4); rightGround < y + collisionBox.y + collisionBox.height + 16; rightGround++)
		{
			if (rightGround <= y + collisionBox.y + collisionBox.height)
			{
				for (int i = collisionBox.width / 2; i >= 0; i--)
				{
					if (rightGround > 0 && rightGround < levelHeight && x + collisionBox.x + collisionBox.width - i > 0 && x + collisionBox.x + collisionBox.width - i < levelWidth)
					{
						levelPixel = al_get_pixel(*collisionBitmap, x + collisionBox.x + collisionBox.width - i, rightGround);
						al_unmap_rgba(levelPixel, &r, &g, &b, &a);

						if (a > 0)
						{
							collision = true;
							break;
						}
					}
				}
				if (a > 0)
				{
					break;
				}
			}
			else if (rightGround > 0 && rightGround < levelHeight && x + collisionBox.x + collisionBox.width > 0 && x + collisionBox.x + collisionBox.width < levelWidth)
			{
				levelPixel = al_get_pixel(*collisionBitmap, x + collisionBox.x + collisionBox.width, rightGround);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					collision = true;
					break;
				}
			}
		}

		if (!collision)
		{
			isGrounded = false;
			isAerial = true;
		}
		else if(collision)
		{
			if (leftGround <= rightGround)
			{
				y = leftGround - (collisionBox.height + collisionBox.y);
			}
			else if(rightGround < leftGround)
			{
				y = rightGround - (collisionBox.height + collisionBox.y);
			}
		}
	}

	else if (isAerial)
	{
		// First we check the walls
		// Left
		bool collision = false;
		for (int leftWall = x + collisionBox.x + (collisionBox.width / 4); leftWall > x + collisionBox.x - 1 && !collision; leftWall--)
		{
			if (leftWall > 0 && leftWall < levelWidth && y + collisionBox.y + collisionBox.height - 20 > 0 && y + collisionBox.y + collisionBox.height - 20 < levelHeight)
			{
				levelPixel = al_get_pixel(*collisionBitmap, leftWall, y + collisionBox.y + collisionBox.height - 20);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					x = leftWall - collisionBox.x + 1;
					if (xSpeed < 0)
					{
						xSpeed = 0;
					}
					collision = true;
				}
			}
		}

		// Right
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (int rightWall = x + collisionBox.x + ((3 * collisionBox.width) / 4); rightWall < x + collisionBox.x + collisionBox.width + 1 && !collision; rightWall++)
		{
			if (rightWall > 0 && rightWall < levelWidth &&  y + collisionBox.y + collisionBox.height - 20 > 0 && y + collisionBox.y + collisionBox.height - 20 < levelHeight)
			{
				levelPixel = al_get_pixel(*collisionBitmap, rightWall, y + collisionBox.y + collisionBox.height - 20);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					x = rightWall - collisionBox.x - collisionBox.width - 1;
					if (xSpeed > 0)
					{
						xSpeed = 0;
					}
					collision = true;
				}
			}
		}

		// Then we check the air above us
		// Left Sensor
		int leftAir, rightAir;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (leftAir = y + collisionBox.y + (collisionBox.height / 4); leftAir > y + collisionBox.y - 16; leftAir--)
		{
			if (leftAir > 0 && leftAir < levelHeight && x + collisionBox.x > 0 && x + collisionBox.x < levelWidth)
			{
				levelPixel = al_get_pixel(*collisionBitmap, x + collisionBox.x, leftAir);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0 && leftAir > y + collisionBox.y)
				{
					y = leftAir - collisionBox.y;
					if (ySpeed < 0)
					{
						ySpeed = 0;
					}
					break;
				}
			}
		}

		// Right Sensor
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (rightAir = y + collisionBox.y + (collisionBox.height / 4); rightAir > y + collisionBox.y - 16; rightAir--)
		{
			if (rightAir > 0 && rightAir < levelHeight && x + collisionBox.x + collisionBox.width > 0 && x + collisionBox.x + collisionBox.width < levelWidth)
			{
				levelPixel = al_get_pixel(*collisionBitmap, x + collisionBox.x + collisionBox.width, rightAir);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0 && rightAir > y + collisionBox.y)
				{
					if (rightAir > leftAir)
					{
						y = rightAir - collisionBox.y;
						if (ySpeed < 0)
						{
							ySpeed = 0;
						}
					}
					break;
				}
			}
		}

		// Finally, we check the ground below us
		// Left Sensor
		int leftGround, rightGround;
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (leftGround = y + collisionBox.y + ((3 * collisionBox.height) / 4); leftGround < y + collisionBox.y + collisionBox.height; leftGround++)
		{
			for (int i = collisionBox.width / 2; i >= 0; i--)
			{
				if (leftGround > 0 && leftGround < levelHeight && x + collisionBox.x + i > 0 && x + collisionBox.x + i < levelWidth)
				{
					levelPixel = al_get_pixel(*collisionBitmap, x + collisionBox.x + i, leftGround);
					al_unmap_rgba(levelPixel, &r, &g, &b, &a);

					if (a > 0 && ySpeed > 0)
					{
						collision = true;
						break;
					}
				}
			}
			if (collision)
			{
				break;
			}
		}

		// Right Sensor
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (rightGround = y + collisionBox.y + ((3 * collisionBox.height) / 4); rightGround < y + collisionBox.y + collisionBox.height; rightGround++)
		{
			for (int i = collisionBox.width / 2; i >= 0; i--)
			{
				if (rightGround > 0 && rightGround < levelHeight && x + collisionBox.x + collisionBox.width - i > 0 && x + collisionBox.x + collisionBox.width - i < levelWidth)
				{
					levelPixel = al_get_pixel(*collisionBitmap, x + collisionBox.x + collisionBox.width - i, rightGround);
					al_unmap_rgba(levelPixel, &r, &g, &b, &a);

					if (a > 0 && ySpeed > 0)
					{
						collision = true;
						break;
					}
				}
			}
			if (a > 0 && ySpeed > 0)
			{
				break;
			}
		}

		if (collision)
		{
			isGrounded = true;
			isAerial = false;
			canDoubleJump = true;
			isFastFalling = false;
			ySpeed = 0;
			if (leftGround <= rightGround)
			{
				y = leftGround - (collisionBox.height + collisionBox.y);
			}
			else if(rightGround < leftGround)
			{
				y = rightGround - (collisionBox.height + collisionBox.y);
			}
		}
		
	}
	al_unlock_bitmap(*collisionBitmap);
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
}

void Character::Animate(bool buttons[6], int LEFT, int RIGHT)
{
	if (xSpeed == 0)
	{
		if (isGrounded)
		{
			if (jumpSquatTimer > 0)
			{
				animationState = JUMPSQUAT;
			}
			else if (isCrouching)
			{
				animationState = CROUCH;
			}
			else
			{
				animationState = IDLE;
			}
		}
		else
		{
			if (ySpeed >= 0)
			{
				animationState = FALL;
			}
			else
			{
				if (canDoubleJump)
				{
					animationState = JUMP;
				}
				else
				{
					animationState = DOUBLE_JUMP;
				}
			}
		}
	}
	else
	{
		if (isGrounded)
		{
			if (jumpSquatTimer > 0)
			{
				animationState = JUMPSQUAT;
			}
			else if (xSpeed * direction > 0)
			{
				if ((buttons[RIGHT] && xSpeed > 0) || (buttons[LEFT] && xSpeed < 0))
				{
					if (isRunning)
					{
						animationState = RUN;
					}
					else
					{
						animationState = WALK;
					}
				}
				else
				{
					if (isRunning)
					{
						animationState = SLIDE;
					}
				}
			}
			else
			{
				if (isRunning)
				{
					animationState = RUN_TURN;
				}
				else
				{
					animationState = WALK_TURN;
				}
			}
		}
		else
		{
			if (ySpeed < 0)
			{
				if (canDoubleJump)
				{
					if (xSpeed * direction > 0)
					{
						animationState = JUMP;
					}
					else if (xSpeed * direction < 0)
					{
						animationState = BACK_JUMP;
					}
				}
				else
				{
					if (xSpeed * direction > 0)
					{
						animationState = DOUBLE_JUMP;
					}
					else if (xSpeed * direction < 0)
					{
						animationState = BACK_DOUBLE_JUMP;
					}
				}
			}
			else
			{
				animationState = FALL;
			}
		}
	}
	
	frame++;
	if (frame > 59)
	{
		frame = 0;
	}
}
