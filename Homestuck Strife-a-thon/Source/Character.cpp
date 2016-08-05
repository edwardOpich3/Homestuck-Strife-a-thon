#include "Header\Character.h"



Character::Character(int spawnX, int spawnY, Control **control)
{
	// Set the necessary base values to their starting values
	Character::spawnX = spawnX;
	Character::spawnY = spawnY;
	Character::x = spawnX;
	Character::y = spawnY;
	Character::width = 256;
	Character::height = 256;
	direction = 1;
	xSpeed = 0;
	ySpeed = 0;
	jumpSquatTimer = 0;
	runTimer = 0;
	crouchTimer = 0;
	isGrounded = false;
	isAerial = true;
	isHanging = false;
	canDoubleJump = true;
	isFastFalling = false;
	isCrouching = false;
	isRunning = false;
	frame = 0;
	animationState = FALL;

	Character::control = control;
}

Character::Character()
{

}

Character::~Character()
{
}

void Character::GetInput()
{

}

// The player has hit either left or right, adjust their horizontal velocity accordingly
void Character::Move(int vector)
{
	// If the player is on the ground and isn't about to jump
	if (isGrounded && jumpSquatTimer == 0 && animationState != HARD_LAND && animationState != WALL_BONK)
	{
		// If they are running
		if (isRunning)
		{
			// Increase their speed by the running acceleration (in the direction of movement)
			xSpeed += vector * runAcc;
			if (vector != direction)
			{
				if (animationState != RUN_TURN)
				{
					frame = 0;
					direction = vector;
				}
				animationState = RUN_TURN;
				return;
			}
		}
		// Otherwise
		else
		{
			// Increase their speed by the walking acceleration
			xSpeed += vector * grndAcc;
			if (vector != direction)
			{
				if (animationState != WALK_TURN)
				{
					frame = 0;
				}
				animationState = WALK_TURN;
				return;
			}
		}
	}
	// Otherwise if they are in the air
	else if (isAerial)
	{
		// Increase their speed by the aerial acceleration
		xSpeed += vector * airAcc;
	}

	else if (isHanging && animationState == LEDGE_HOLD)
	{
		if (vector == direction && animationState != NORMAL_CLIMB)
		{
			y -= collisionBox.height / 2;
			animationState = NORMAL_CLIMB;
			frame = 0;
		}
		else if(animationState != NORMAL_CLIMB)
		{
			isAerial = true;
			isHanging = false;
			animationState = BACK_FALL;
			frame = 0;
		}
	}
}

// The player has hit the jump button, either put them into jumpsquat, make them double jump, or do nothing depending on the circumstances
void Character::Jump(std::vector<bool> *buttons, int Z, int LEFT, int RIGHT)
{
	// If they are on the ground
	if (isGrounded && animationState != HARD_LAND)
	{
		// If they aren't currently attempting to jump
		if (jumpSquatTimer == 0)
		{
			// Start the jumpsquat
			jumpSquatTimer = jumpSquat;
			animationState = JUMPSQUAT;
			frame = 0;
		}
	}
	// Otherwise if they can double jump (implied to be in the air)
	else if (canDoubleJump && animationState != HARD_LAND && isAerial)
	{
		// Set their ySpeed to that of the jump speed
		ySpeed = jump;

		// Set the double jump and fastfall flags to false, as they've used their double jump, and double jumping cancels out fastfalling
		canDoubleJump = false;
		isFastFalling = false;
		buttons[0][Z] = false;
		frame = 0;
		if (!(buttons[0][LEFT] && direction > 0) && !(buttons[0][RIGHT] && direction < 0))
		{
			animationState = DOUBLE_JUMP;
		}
		else
		{
			animationState = BACK_DOUBLE_JUMP;
		}
	}

	// Otherwise if they're hanging from a ledge
	else if (isHanging && animationState == LEDGE_HOLD)
	{
		y -= collisionBox.height / 2;
		buttons[0][Z] = false;
		animationState = JUMP_CLIMB;
		frame = 0;
	}
}

// The player has hit the down button, for now this will be exclusively for fastfalling but later this should be appended for ducking as well
void Character::FastFall(bool current, bool previous)
{
	// If the player is falling and isn't already holding the down button
	if (ySpeed > 0 && (current && !previous) && isAerial)
	{
		// They are now fastfalling; adjust their yspeed accordingly
		isFastFalling = true;
		ySpeed = fastfallSpeed;
	}
	else if (isHanging && current && animationState != LEDGE_GRAB)
	{
		isFastFalling = true;
		ySpeed = fastfallSpeed;
		isHanging = false;
		isAerial = true;
		x += 4 * -direction;
		animationState = FALL;
		frame = 0;
	}
	// They are now considered to be holding the down button
	if (current)
	{
		if (!previous && crouchTimer > 0)
		{
			isCrouching = true;
		}
		crouchTimer = 10;
	}
	else if (previous)
	{
		crouchTimer = 10;
	}
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

void Character::Collision(ALLEGRO_BITMAP** collisionBitmap, int levelWidth, int levelHeight, bool DOWN)
{
	al_lock_bitmap(*collisionBitmap, al_get_bitmap_format(*collisionBitmap), ALLEGRO_LOCK_READONLY);
	ALLEGRO_BITMAP* temp = NULL;
	ALLEGRO_COLOR levelPixel;
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;
	int tempX = x;
	int tempY = y;

	temp = al_create_bitmap(256, 256);
	al_set_target_bitmap(temp);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
	al_draw_bitmap(*collisionBitmap, 0, 0, NULL);

	if (isGrounded)
	{
		// First we check for walls
		// Left
		bool collision = false;
		for (int leftWall = collisionBox.x + (collisionBox.width / 2); leftWall > collisionBox.x - 1 && !collision; leftWall--)
		{
			if (tempX + leftWall > 0 && tempX + leftWall < levelWidth && tempY + collisionBox.y + collisionBox.height - 20 > 0 && tempY + collisionBox.y + collisionBox.height - 20 < levelHeight)
			{
				levelPixel = al_get_pixel(*collisionBitmap, leftWall, collisionBox.y + collisionBox.height - 20);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					tempX = tempX + leftWall - collisionBox.x + 1;
					if (xSpeed < 0)
					{
						xSpeed = 0;
						if (isRunning && animationState != WALL_BONK)
						{
							isRunning = false;
							animationState = WALL_BONK;
							frame = 0;
						}
					}
					collision = true;
				}
			}
		}

		al_unlock_bitmap(*collisionBitmap);

		al_set_target_bitmap(*collisionBitmap);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_bitmap(temp, x - tempX, y - tempY, NULL);

		al_lock_bitmap(*collisionBitmap, al_get_bitmap_format(*collisionBitmap), ALLEGRO_LOCK_READONLY);

		// Right
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (int rightWall = collisionBox.x + (collisionBox.width / 2); rightWall < collisionBox.x + collisionBox.width + 1 && !collision; rightWall++)
		{
			if (tempX + rightWall > 0 && tempX + rightWall < levelWidth && tempY + collisionBox.y + collisionBox.height - 20 > 0 && tempY + collisionBox.y + collisionBox.height - 20 < levelHeight)
			{
				levelPixel = al_get_pixel(*collisionBitmap, rightWall, collisionBox.y + collisionBox.height - 20);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					tempX = tempX + rightWall - collisionBox.x - collisionBox.width - 1;
					if (xSpeed > 0)
					{
						xSpeed = 0;
						if (isRunning && animationState != WALL_BONK)
						{
							isRunning = false;
							animationState = WALL_BONK;
							frame = 0;
						}
					}
					collision = true;
				}
			}
		}

		al_unlock_bitmap(*collisionBitmap);

		al_set_target_bitmap(*collisionBitmap);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_bitmap(temp, x - tempX, y - tempY, NULL);

		al_lock_bitmap(*collisionBitmap, al_get_bitmap_format(*collisionBitmap), ALLEGRO_LOCK_READONLY);

		// Then we check the ground below us
		// Left Sensor
		int leftGround, rightGround;
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (leftGround = collisionBox.y + ((3 * collisionBox.height) / 4); leftGround < collisionBox.y + collisionBox.height + 16; leftGround++)
		{
			for (int i = collisionBox.width / 2; i >= 0; i--)
			{
				if (tempY + leftGround > 0 && tempY + leftGround < levelHeight && tempX + collisionBox.x + i > 0 && tempX + collisionBox.x + i < levelWidth)
				{
					levelPixel = al_get_pixel(*collisionBitmap, collisionBox.x + i, leftGround);
					al_unmap_rgba(levelPixel, &r, &g, &b, &a);

					if (a > 0)
					{
						if (g == 0 || g == 1)
						{
							collision = true;
							break;
						}
						else if (g == 2)
						{
							if (!isCrouching)
							{
								collision = true;
								break;
							}
							else if(animationState != THROUGH_PLATFORM)
							{
								animationState = THROUGH_PLATFORM;
								frame = 0;
								break;
							}
						}
					}
					else if (leftGround >= collisionBox.y + collisionBox.height + 15 && xSpeed == 0 && direction < 0 && animationState != CROUCH)
					{
						if (animationState != BALANCING)
						{
							frame = 0;
						}
						animationState = BALANCING;
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
		for (rightGround = collisionBox.y + ((3 * collisionBox.height) / 4); rightGround < collisionBox.y + collisionBox.height + 16; rightGround++)
		{
			for (int i = collisionBox.width / 2; i >= 0; i--)
			{
				if (tempY + rightGround > 0 && tempY + rightGround < levelHeight && tempX + collisionBox.x + collisionBox.width - i > 0 && tempX + collisionBox.x + collisionBox.width - i < levelWidth)
				{
					levelPixel = al_get_pixel(*collisionBitmap, collisionBox.x + collisionBox.width - i, rightGround);
					al_unmap_rgba(levelPixel, &r, &g, &b, &a);

					if (a > 0)
					{
						if (g == 0 || g == 1)
						{
							collision = true;
							break;
						}
						else if (g == 2)
						{
							if (!isCrouching)
							{
								collision = true;
								break;
							}
							else if (animationState != THROUGH_PLATFORM)
							{
								animationState = THROUGH_PLATFORM;
								frame = 0;
								break;
							}
						}
					}
					else if (rightGround >= collisionBox.y + collisionBox.height + 15 && xSpeed == 0 && direction > 0 && animationState != CROUCH)
					{
						if (animationState != BALANCING)
						{
							frame = 0;
						}
						animationState = BALANCING;
					}
				}
			}
			if (a > 0)
			{
				break;
			}
		}
		
		if(!collision)
		{
			isGrounded = false;
			isAerial = true;
			if (isRunning && animationState != RUN_OFF)
			{
				animationState = RUN_OFF;
				frame = 0;
			}
		}	
		else if (collision)
		{
			if (leftGround <= rightGround)
			{
				tempY = tempY + leftGround - (collisionBox.height + collisionBox.y);
			}
			else if(rightGround < leftGround)
			{
				tempY = tempY + rightGround - (collisionBox.height + collisionBox.y);
			}
		}
	}

	else if (isAerial)
	{
		// First we check the walls
		// Left
		bool collision = false;
		for (int leftWall = collisionBox.x + (collisionBox.width / 4); leftWall > collisionBox.x - 1 && !collision; leftWall--)
		{
			if (tempX + leftWall > 0 && tempX + leftWall < levelWidth && tempY + collisionBox.y + collisionBox.height - 20 > 0 && tempY + collisionBox.y + collisionBox.height - 20 < levelHeight)
			{
				levelPixel = al_get_pixel(*collisionBitmap, leftWall, collisionBox.y + collisionBox.height - 20);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					if (g == 0)
					{
						tempX = tempX + leftWall - collisionBox.x + 1;
						if (xSpeed < 0)
						{
							xSpeed = 0;
						}
						collision = true;
					}
					else if (g == 1 && !isHanging)
					{
						tempX = tempX + leftWall - collisionBox.x + 1;
						xSpeed = 0;
						tempY = tempY + collisionBox.height - 20;
						ySpeed = 0;
						collision = true;
						direction = -1;
						canDoubleJump = true;
						isAerial = false;
						isFastFalling = false;
						isHanging = true;
						animationState = LEDGE_GRAB;
						frame = 0;
					}
				}
			}
		}

		al_unlock_bitmap(*collisionBitmap);

		al_set_target_bitmap(*collisionBitmap);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_bitmap(temp, x - tempX, y - tempY, NULL);

		al_lock_bitmap(*collisionBitmap, al_get_bitmap_format(*collisionBitmap), ALLEGRO_LOCK_READONLY);

		// Right
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (int rightWall = collisionBox.x + ((3 * collisionBox.width) / 4); rightWall < collisionBox.x + collisionBox.width + 1 && !collision; rightWall++)
		{
			if (tempX + rightWall > 0 && tempX + rightWall < levelWidth && tempY + collisionBox.y + collisionBox.height - 20 > 0 && tempY + collisionBox.y + collisionBox.height - 20 < levelHeight)
			{
				levelPixel = al_get_pixel(*collisionBitmap, rightWall, collisionBox.y + collisionBox.height - 20);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);

				if (a > 0)
				{
					if (g == 0)
					{
						tempX = tempX + rightWall - collisionBox.x - collisionBox.width - 1;
						if (xSpeed > 0)
						{
							xSpeed = 0;
						}
						collision = true;
					}
					else if (g == 1 && !isHanging)
					{
						tempX = tempX + rightWall - collisionBox.x - collisionBox.width - 1;
						xSpeed = 0;
						tempY = tempY + collisionBox.height - 20;
						ySpeed = 0;
						collision = true;
						direction = 1;
						canDoubleJump = true;
						isAerial = false;
						isFastFalling = false;
						isHanging = true;
						animationState = LEDGE_GRAB;
						frame = 0;
					}
				}
			}
		}

		al_unlock_bitmap(*collisionBitmap);

		al_set_target_bitmap(*collisionBitmap);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_bitmap(temp, x - tempX, y - tempY, NULL);

		al_lock_bitmap(*collisionBitmap, al_get_bitmap_format(*collisionBitmap), ALLEGRO_LOCK_READONLY);

		// Sloped ceiling detection goes here
		// Left
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (int i = collisionBox.width / 2; i >= 0; i--)
		{
			if (tempY + collisionBox.y > 0 && tempY + collisionBox.y < levelHeight && tempX + collisionBox.x + i > 0 && tempX + collisionBox.x + i < levelWidth)
			{
				levelPixel = al_get_pixel(*collisionBitmap, collisionBox.x + i, collisionBox.y);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);
				if (a > 0 && g != 2)
				{
					if (r < 182 || r > 202)
					{
						tempX = tempX + i;
						if (ySpeed < 0)
						{
							if (xSpeed < 0)
							{
								xSpeed = cos((ALLEGRO_PI * float(r)) / 128) * -ySpeed;
							}
							else
							{
								xSpeed += cos((ALLEGRO_PI * float(r)) / 128) * -ySpeed;
							}
						}
						collision = true;
						break;
					}
				}
			}
		}

		al_unlock_bitmap(*collisionBitmap);

		al_set_target_bitmap(*collisionBitmap);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_bitmap(temp, x - tempX, y - tempY, NULL);

		al_lock_bitmap(*collisionBitmap, al_get_bitmap_format(*collisionBitmap), ALLEGRO_LOCK_READONLY);

		// Right
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (int i = collisionBox.width / 2; i >= 0; i--)
		{
			if (tempY + collisionBox.y > 0 && tempY + collisionBox.y < levelHeight && tempX + collisionBox.x + collisionBox.width - i > 0 && tempX + collisionBox.x + collisionBox.width - i < levelWidth)
			{
				levelPixel = al_get_pixel(*collisionBitmap, collisionBox.x + collisionBox.width - i, collisionBox.y);
				al_unmap_rgba(levelPixel, &r, &g, &b, &a);
				if (a > 0 && g != 2)
				{
					if (r < 182 || r > 202)
					{
						tempX = tempX - i;
						if (ySpeed < 0)
						{
							if (xSpeed > 0)
							{
								xSpeed = cos((ALLEGRO_PI * float(r)) / 128) * -ySpeed;
							}
							else
							{
								xSpeed += cos((ALLEGRO_PI * float(r)) / 128) * -ySpeed;
							}
						}
						collision = true;
						break;
					}
				}
			}
		}

		al_unlock_bitmap(*collisionBitmap);

		al_set_target_bitmap(*collisionBitmap);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_bitmap(temp, x - tempX, y - tempY, NULL);

		al_lock_bitmap(*collisionBitmap, al_get_bitmap_format(*collisionBitmap), ALLEGRO_LOCK_READONLY);

		// Then we check the air above us
		// Left Sensor
		int leftAir, rightAir;
		int finalAir = tempY;
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (leftAir = collisionBox.y + (collisionBox.height / 4); leftAir > collisionBox.y - 16; leftAir--)
		{
			for (int i = collisionBox.width / 2; i >= 0; i--)
			{
				if (tempY + leftAir > 0 && tempY + leftAir < levelHeight && tempX + collisionBox.x + i > 0 && tempX + collisionBox.x + i < levelWidth)
				{
					levelPixel = al_get_pixel(*collisionBitmap, collisionBox.x + i, leftAir);
					al_unmap_rgba(levelPixel, &r, &g, &b, &a);

					if (a > 0 && tempY + leftAir > tempY + collisionBox.y)
					{
						if (g == 0 && !(r < 182 || r > 202) && !collision)
						{
							finalAir = tempY + leftAir - collisionBox.y;
							if (ySpeed < 0)
							{
								ySpeed = 0;
							}
							collision = true;
						}
						else if (g == 1 && !isHanging)
						{
							tempY = tempY + leftAir - collisionBox.y;
							ySpeed = 0;
							tempX = tempX + i + 2;
							xSpeed = 0;
							collision = true;
							direction = -1;
							canDoubleJump = true;
							isAerial = false;
							isFastFalling = false;
							isHanging = true;
							animationState = LEDGE_GRAB;
							frame = 0;
							break;
						}
					}
				}
			}
			if (g == 1)
			{
				break;
			}
		}
		if (finalAir != tempY && g == 0)
		{
			leftAir = finalAir;
		}

		// Right Sensor
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (rightAir = collisionBox.y + (collisionBox.height / 4); rightAir > collisionBox.y - 16; rightAir--)
		{
			for (int i = collisionBox.width / 2; i >= 0; i--)
			{
				if (tempY + rightAir > 0 && tempY + rightAir < levelHeight && tempX + collisionBox.x + collisionBox.width - i > 0 && tempX + collisionBox.x + collisionBox.width - i < levelWidth)
				{
					levelPixel = al_get_pixel(*collisionBitmap, collisionBox.x + collisionBox.width - i, rightAir);
					al_unmap_rgba(levelPixel, &r, &g, &b, &a);

					if (a > 0 && tempY + rightAir > tempY + collisionBox.y && rightAir > leftAir)
					{
						if (g == 0 && !(r < 182 || r > 202) && !collision)
						{
							finalAir = tempY + rightAir - collisionBox.y;
							if (ySpeed < 0)
							{
								ySpeed = 0;
							}
							collision = true;
						}
						else if (g == 1 && !isHanging)
						{
							tempY = tempY + rightAir - collisionBox.y;
							ySpeed = 0;
							tempX = tempX - i - 1;
							xSpeed = 0;
							collision = true;
							direction = 1;
							canDoubleJump = true;
							isAerial = false;
							isFastFalling = false;
							isHanging = true;
							animationState = LEDGE_GRAB;
							frame = 0;
							break;
						}
					}
				}
			}
			if (g == 1)
			{
				break;
			}
		}
		if (finalAir != tempY && g == 0 && !isHanging)
		{
			if (finalAir > leftAir)
			{
				tempY = finalAir;
			}
			else
			{
				tempY = leftAir;
			}
		}

		al_unlock_bitmap(*collisionBitmap);

		al_set_target_bitmap(*collisionBitmap);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_bitmap(temp, x - tempX, y - tempY, NULL);

		al_lock_bitmap(*collisionBitmap, al_get_bitmap_format(*collisionBitmap), ALLEGRO_LOCK_READONLY);

		// Finally, we check the ground below us
		// Left Sensor
		int leftGround, rightGround;
		int angleBoost = 0;
		collision = false;
		r = 0;
		g = 0;
		b = 0;
		a = 0;
		for (leftGround = collisionBox.y + ((3 * collisionBox.height) / 4); leftGround < collisionBox.y + collisionBox.height; leftGround++)
		{
			for (int i = collisionBox.width / 2; i >= 0; i--)
			{
				if (tempY + leftGround > 0 && tempY + leftGround < levelHeight && tempX + collisionBox.x + i > 0 && tempX + collisionBox.x + i < levelWidth)
				{
					levelPixel = al_get_pixel(*collisionBitmap, collisionBox.x + i, leftGround);
					al_unmap_rgba(levelPixel, &r, &g, &b, &a);

					if (a > 0 && ySpeed > 0)
					{
						if (g == 0 || g == 1)
						{
							collision = true;
							if (r <= 128)
							{
								angleBoost = cos((ALLEGRO_PI * float(r)) / 128) * ySpeed;
							}
							break;
						}
						else if (g == 2 && !DOWN)
						{
							collision = true;
							if (r <= 128)
							{
								angleBoost = cos((ALLEGRO_PI * float(r)) / 128) * ySpeed;
							}
							break;
						}
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
		for (rightGround = collisionBox.y + ((3 * collisionBox.height) / 4); rightGround < collisionBox.y + collisionBox.height; rightGround++)
		{
			for (int i = collisionBox.width / 2; i >= 0; i--)
			{
				if (tempY + rightGround > 0 && tempY + rightGround < levelHeight && tempX + collisionBox.x + collisionBox.width - i > 0 && tempX + collisionBox.x + collisionBox.width - i < levelWidth)
				{
					levelPixel = al_get_pixel(*collisionBitmap, collisionBox.x + collisionBox.width - i, rightGround);
					al_unmap_rgba(levelPixel, &r, &g, &b, &a);

					if (a > 0 && ySpeed > 0)
					{
						if (g == 0 || g == 1)
						{
							collision = true;
							break;
						}
						else if (g == 2 && !DOWN)
						{
							collision = true;
							break;
						}
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
			if (ySpeed >= (float)fallSpeed)
			{
				animationState = HARD_LAND;
			}
			else
			{
				animationState = SOFT_LAND;
			}
			frame = 0;
			if (leftGround <= rightGround)
			{
				tempY = tempY + leftGround - (collisionBox.height + collisionBox.y);
				xSpeed += angleBoost;
			}
			else if(rightGround < leftGround)
			{
				tempY = tempY + rightGround - (collisionBox.height + collisionBox.y);
				if (r <= 128)
				{
					angleBoost = cos((ALLEGRO_PI * float(r)) / 128) * ySpeed;
				}
				xSpeed += angleBoost;
			}
			ySpeed = 0;
		}
		
	}
	al_unlock_bitmap(*collisionBitmap);

	if (tempX + collisionBox.x + collisionBox.width < 0 || tempX + collisionBox.x > levelWidth || tempY + collisionBox.y > levelHeight || tempY + collisionBox.y + collisionBox.height < 0)
	{
		tempX = spawnX;
		tempY = spawnY;
		direction = 1;
		xSpeed = 0;
		ySpeed = 0;
		jumpSquatTimer = 0;
		runTimer = 0;
		isGrounded = false;
		isAerial = true;
		isHanging = false;
		canDoubleJump = true;
		isFastFalling = false;
		isCrouching = false;
		isRunning = false;
		frame = 0;
		animationState = FALL;
	}

	x = tempX;
	y = tempY;

	al_destroy_bitmap(temp);
}

void Character::Update(std::vector<bool> *buttons, int Z, int LEFT, int RIGHT)
{
	if (jumpSquatTimer > 0)
	{
		jumpSquatTimer--;
		if (jumpSquatTimer == 0)
		{
			if (buttons[0][Z])
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
			buttons[0][Z] = false;
			frame = 0;
			if (!(buttons[0][LEFT] && direction > 0) && !(buttons[0][RIGHT] && direction < 0))
			{
				animationState = JUMP;
			}
			else
			{
				animationState = BACK_JUMP;
			}
		}
	}

	if (runTimer > 0)
	{
		runTimer--;
	}

	if (crouchTimer > 0)
	{
		crouchTimer--;
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
			if (xSpeed == 0 && animationState != RUN_TURN)
			{
				isRunning = false;
			}
			if (animationState == RUN_TURN && xSpeed * direction > 0)
			{
				xSpeed = 0;
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

void Character::Animate(std::vector<bool> *buttons, int LEFT, int RIGHT, int DOWN)
{
	frame++;

	if (isGrounded)
	{
		if (buttons[0][RIGHT] || buttons[0][LEFT])
		{
			if (jumpSquatTimer == 0)
			{
				if (isRunning)
				{
					if (animationState != RUN_TURN && animationState != HARD_LAND && animationState != WALL_BONK)
					{
						if (animationState != RUN)
						{
							frame = 0;
						}
						animationState = RUN;
					}
				}
				else if (animationState != WALK_TURN && animationState != HARD_LAND && animationState != WALL_BONK)
				{
					if (animationState != WALK)
					{
						frame = 0;
					}
					animationState = WALK;
				}
			}
		}
		else
		{
			if (jumpSquatTimer == 0)
			{
				if (isRunning)
				{
					if (animationState != SLIDE)
					{
						frame = 0;
					}
					animationState = SLIDE;
				}
				else if (buttons[0][DOWN])
				{
					if (animationState != CROUCH)
					{
						frame = 0;
					}
					animationState = CROUCH;
				}
				else
				{
					if (animationState != SOFT_LAND && animationState != HARD_LAND && animationState != BALANCING)
					{
						if (animationState != IDLE)
						{
							frame = 0;
						}
						animationState = IDLE;
					}
				}
			}
		}
	}
	else if(isAerial)
	{
		if (ySpeed >= 0)
		{
			if (animationState != THROUGH_PLATFORM && animationState != RUN_OFF)
			{
				if ((buttons[0][RIGHT] && direction > 0) || (buttons[0][LEFT] && direction < 0))
				{
					if (animationState != FORWARD_FALL)
					{
						frame = 0;
					}
					animationState = FORWARD_FALL;
				}
				else if ((buttons[0][RIGHT] && direction < 0) || (buttons[0][LEFT] && direction > 0))
				{
					if (animationState != BACK_FALL)
					{
						frame = 0;
					}
					animationState = BACK_FALL;
				}
				else if (!buttons[0][RIGHT] && !buttons[0][LEFT])
				{
					if (animationState != FALL)
					{
						frame = 0;
					}
					animationState = FALL;
				}
			}
		}
	}

	if (frame == animationLengths[animationState] / 2)
	{
		switch (animationState)
		{
			case WALK_TURN:
			{
				direction = -direction;
				break;
			}
			case NORMAL_CLIMB:
			{
				y -= (collisionBox.height / 2) + 1;
				x += direction * collisionBox.width;
				break;
			}
			case JUMP_CLIMB:
			{
				y -= (collisionBox.height / 2) + 1;
				x += direction * collisionBox.width;
				ySpeed = jump;
				isAerial = true;
				isHanging = false;
				break;
			}
		}
	}

	else if (frame >= animationLengths[animationState])
	{
		switch (animationState)
		{
			case WALK_TURN:
			{
				frame = 0;
				if (buttons[0][LEFT] || buttons[0][RIGHT])
				{
					animationState = WALK;
				}
				else
				{
					animationState = IDLE;
				}
				break;
			}
			case CROUCH:
			{
				frame = 59;
				break;
			}
			case RUN_TURN:
			{
				frame = 0;
				if (buttons[0][LEFT] || buttons[0][RIGHT])
				{
					animationState = RUN;
				}
				else
				{
					animationState = IDLE;
				}
				break;
			}
			case SLIDE:
			{
				frame = 0;
				animationState = IDLE;
				break;
			}
			case FALL:
			{
				frame = 59;
				break;
			}
			case FORWARD_FALL:
			{
				frame = 59;
				break;
			}
			case BACK_FALL:
			{
				frame = 59;
				break;
			}
			case SOFT_LAND:
			{
				frame = 0;
				animationState = IDLE;
				break;
			}
			case HARD_LAND:
			{
				frame = 0;
				animationState = IDLE;
				break;
			}
			case LEDGE_GRAB:
			{
				frame = 0;
				animationState = LEDGE_HOLD;
				break;
			}
			case NORMAL_CLIMB:
			{
				isGrounded = true;
				isHanging = false;
				frame = 0;
				animationState = IDLE;
				break;
			}
			case THROUGH_PLATFORM:
			{
				animationState = FALL;
				frame = 0;
				break;
			}
			case WALL_BONK:
			{
				animationState = IDLE;
				frame = 0;
				break;
			}
			case RUN_OFF:
			{
				animationState = FALL;
				frame = 0;
				break;
			}
			default:
			{
				frame = 0;
				break;
			}
		}
	}
}
