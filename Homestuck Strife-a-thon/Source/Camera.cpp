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
	distance = abs(sqrtf((abs(distanceX * distanceX)) + abs((distanceY * distanceY))));
	if (distance < minDist)
	{
		distance = minDist;
	}
}

void Camera::Update(int levelWidth, int levelHeight, int width, int height)
{
	scale = 2 * (abs(distance) + 128) / width;

	if (scale * width > levelWidth)
	{
		scale = levelWidth / (float)width;
	}
	if (scale * height > levelHeight)
	{
		scale = levelHeight / (float)height;
	}

	x = centerX - ((scale * width) / 2);
	y = centerY - ((scale * height) / 2);

	if (x < 0)
	{
		x = 0;
	}
	else if (x + (scale * width) > levelWidth)
	{
		x = levelWidth - (scale * width);
	}
	if (y < 0)
	{
		y = 0;
	}
	else if (y + (scale * height) > levelHeight)
	{
		y = levelHeight - (scale * height);
	}

	if (x + (scale * width / 2) != centerX || y + (scale * height / 2) != centerY)
	{
		//distance -= sqrtf((abs(x + (scale * width / 2) - centerX) * abs(x + (scale * width / 2) - centerX)) + (abs(y + (scale * height / 2) - centerY) * abs(y + (scale * height / 2) - centerY))) / 2;
		if (abs(distanceX) > abs(distanceY))
		{
			distance -= abs(x + (scale * width / 2) - centerX) / 2;
		}
		else
		{
			distance -= abs(y + (scale * height / 2) - centerY) / 2;
		}
		if (distance < minDist)
		{
			distance = minDist;
		}
		scale = 2 * (abs(distance) + 128) / width;

		if (scale * width > levelWidth)
		{
			scale = levelWidth / width;
		}
		if (scale * height > levelHeight)
		{
			scale = levelHeight / height;
		}

		x = centerX - ((scale * width) / 2);
		y = centerY - ((scale * height) / 2);

		if (x < 0)
		{
			x = 0;
		}
		else if (x + (scale * width) > levelWidth)
		{
			x = levelWidth - (scale * width);
		}
		if (y < 0)
		{
			y = 0;
		}
		else if (y + (scale * height) > levelHeight)
		{
			y = levelHeight - (scale * height);
		}
	}
}