#include "..\Header\Camera.h"



Camera::Camera(int x, int y)
{
	Camera::x = x;
	Camera::y = y;
	Camera::scale = 1;

	xSpeed = 0;
	ySpeed = 0;
	zSpeed = 0;
	centerX = 0;
	centerY = 0;
	distance = 0;
	distance = 0;

	maxXSpeed = 10.0;
	maxYSpeed = 10.0;
	maxZSpeed = 10.0;
	xAcc = 0.5;
	yAcc = 0.5;
	zAcc = 0.5;

	// NOTE: These must be in the ratio of 4:3 unless you want the game to look like shit!
	minDist = 128;
}

Camera::Camera()
{
}


Camera::~Camera()
{
}


void Camera::CalculateCenter(int p1CenterX, int p1CenterY, int p2CenterX, int p2CenterY)
{
	centerX = (p1CenterX + p2CenterX) / 2;
	centerY = (p1CenterY + p2CenterY) / 2;
}

void Camera::CalculateDistance(int p1CenterX, int p1CenterY, int p2CenterX, int p2CenterY)
{
	int distanceX, distanceY;
	if (abs(centerX - p1CenterX) > abs(centerX - p2CenterX))
	{
		distanceX = centerX - p1CenterX;
	}
	else
	{
		distanceX = centerX - p2CenterX;
	}
	if (abs(centerY - p1CenterY) > abs(centerY - p2CenterY))
	{
		distanceY = centerY - p1CenterY;
	}
	else
	{
		distanceY = centerY - p2CenterY;
	}
	distance = abs(sqrtf((abs(distanceX * distanceX)) + abs((distanceY + distanceY))));
	if (distance < minDist)
	{
		distance = minDist;
	}
}

void Camera::Update(int levelWidth, int levelHeight)
{
	// First, resize the camera appropriately
	/*if (abs(distanceX) > width / 2 || abs(distanceY) > height / 2)
	{
		zxSpeed -= zxAcc;
		zySpeed -= zyAcc;
	}
	else if (abs(distanceX) + 64 < width / 2 || abs(distanceY) + 64 < height / 2)
	{
		zxSpeed += zxAcc;
		zySpeed += zyAcc;
	}
	if (zxSpeed > maxZXSpeed)
	{
		zxSpeed = maxZXSpeed;
	}
	else if (zxSpeed < -maxZXSpeed)
	{
		zxSpeed = -maxZXSpeed;
	}
	if (zySpeed > maxZYSpeed)
	{
		zySpeed = maxZYSpeed;
	}
	else if (zySpeed < -maxZYSpeed)
	{
		zySpeed = -maxZYSpeed;
	}

	// Then, move the camera to the appropriate spot
	width -= zxSpeed;
	height -= zySpeed;

	// Special note: As a rule of thumb, anything that is multiplied by width must be 3, and anything multiplied by height must be a multiple of 4.
	// The inverse goes for divisors; width must be 4, height must be 3.
	// Otherwise the camera acts up.
	if (abs(distanceX) > abs(distanceY))
	{
		width = (abs(distanceX) * 2) + 256;
		height = (3 * width) / 4;
	}
	else if (abs(distanceX) <= abs(distanceY))
	{
		height = (abs(distanceY) * 1.5) + 192;
		width = (4 * height) / 3;
	}
	if (width > levelWidth)
	{
		width = levelWidth;
		height = (3 * width) / 4;
	}
	else if (height > levelHeight)
	{
		height = levelHeight;
		width = (4 * height) / 3;
	}

	x = centerX - (width / 2);
	y = centerY - (height / 2);
	if (x < 0)
	{
		x = 0;
	}
	else if (x + width > levelWidth)
	{
		x = levelWidth - width;
	}

	if (y < 0)
	{
		y = 0;
	}
	else if (y + height > levelHeight)
	{
		y = levelHeight - height;
	}

	if (x + (width / 2) != centerX || y + (height / 2) != centerY)
	{
		if (abs(x + (width / 2) - centerX) > abs(y + (height / 2) - centerY))
		{
			width -= abs((x + (width / 2)) - centerX);
			height = (3 * width) / 4;
		}
		else if (abs(x + (width / 2) - centerX) < abs(y + (height / 2) - centerY))
		{
			height -= abs((y + (height / 2)) - centerY);
			width = (4 * height) / 3;
		}
		x = centerX - (width / 2);
		y = centerY - (height / 2);
		if (x < 0)
		{
			x = 0;
		}
		else if (x + width > levelWidth)
		{
			x = levelWidth - width;
		}

		if (y < 0)
		{
			y = 0;
		}
		else if (y + height > levelHeight)
		{
			y = levelHeight - height;
		}
	}*/

	scale = 8 * distance / 768;
	if (scale * 1024 > levelWidth)
	{
		scale = levelWidth / 1024;
	}
	else if (scale * 768 > levelHeight)
	{
		scale = levelHeight / 768;
	}

	x = centerX - ((scale * 1024) / 2);
	y = centerY - ((scale * 768) / 2);
	if (x < 0)
	{
		x = 0;
	}
	else if (x + (scale * 1024) > levelWidth)
	{
		x = levelWidth - (scale * 1024);
	}

	if (y < 0)
	{
		y = 0;
	}
	else if (y + (scale * 768) > levelHeight)
	{
		y = levelHeight - (scale * 768);
	}

	/*if (x + (width / 2) != centerX || y + (height / 2) != centerY)
	{
		if (abs(x + (width / 2) - centerX) > abs(y + (height / 2) - centerY))
		{
			width -= abs((x + (width / 2)) - centerX);
			height = (3 * width) / 4;
		}
		else if (abs(x + (width / 2) - centerX) < abs(y + (height / 2) - centerY))
		{
			height -= abs((y + (height / 2)) - centerY);
			width = (4 * height) / 3;
		}
		x = centerX - (width / 2);
		y = centerY - (height / 2);
		if (x < 0)
		{
			x = 0;
		}
		else if (x + width > levelWidth)
		{
			x = levelWidth - width;
		}

		if (y < 0)
		{
			y = 0;
		}
		else if (y + height > levelHeight)
		{
			y = levelHeight - height;
		}
	}*/
}