#pragma once
#include "allegro5\allegro.h"

class Camera
{
public:

	int x, y;
	int width, height;
	float xSpeed, ySpeed, zxSpeed, zySpeed;
	float maxXSpeed, maxYSpeed, maxZXSpeed, maxZYSpeed;
	float xAcc, yAcc, zxAcc, zyAcc;
	int centerX, centerY;
	int distanceX, distanceY;
	int minDistX, minDistY;

	// Constructor & Destructor
	Camera(int x, int y, int width, int height);
	Camera();
	~Camera();

	// Functions
	void CalculateCenter(int p1CenterX, int p1CenterY, int p2CenterX, int p2CenterY);
	void CalculateDistance(int p1CenterX, int p1CenterY, int p2CenterX, int p2CenterY);
	void Update(int levelWidth, int levelHeight);
};

