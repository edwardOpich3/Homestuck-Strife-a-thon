#include "..\Header\Camera.h"



Camera::Camera(int x, int y, int width, int height)
{
	Camera::x = x;
	Camera::y = y;
	Camera::width = width;
	Camera::height = height;

	xSpeed = 0;
	ySpeed = 0;
	zxSpeed = 0;
	zySpeed = 0;
	centerX = 0;
	centerY = 0;
	distanceX = 0;
	distanceY = 0;

	maxXSpeed = 10.0;
	maxYSpeed = 10.0;
	maxZXSpeed = 10.0;
	maxZYSpeed = 7.5;
	xAcc = 0.5;
	yAcc = 0.5;
	zxAcc = 0.5;
	zyAcc = 0.375;
	minDistX = 128;
	minDistY = 96;
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
	if (abs(distanceX) < minDistX)
	{
		if (distanceX > 0)
		{
			distanceX = minDistX;
		}
		else
		{
			distanceX = -minDistX;
		}
	}
	if (abs(distanceY) < minDistY)
	{
		if (distanceY > 0)
		{
			distanceY = minDistY;
		}
		else
		{
			distanceY = -minDistY;
		}
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
	height -= zySpeed;*/

	if (abs(distanceX) > abs(distanceY))
	{
		width = abs(distanceX) * 4;
		height = (3 * width) / 4;
	}
	else if (abs(distanceX) <= abs(distanceY))
	{
		height = abs(distanceY) * 3;
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
	if (x < 0)
	{
		x = 0;
	}
	else if (x + width > levelWidth)
	{
		x = levelWidth - width;
	}

	y = centerY - (height / 2);
	if (y < 0)
	{
		y = 0;
	}
	else if (y + height > levelHeight)
	{
		y = levelHeight - height;
	}
	
}