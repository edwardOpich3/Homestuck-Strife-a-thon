#pragma once
#include "allegro5\allegro.h"
#include <cmath>

class Camera
{
public:

	int x, y;
	float scale;
	float xSpeed, ySpeed, zSpeed;
	float maxXSpeed, maxYSpeed, maxZSpeed;
	float xAcc, yAcc, zAcc;
	int centerX, centerY;
	float distance;
	float minDist;

	// Constructor & Destructor
	Camera(int x, int y);
	Camera();
	~Camera();

	// Functions
	void CalculateCenter(int p1CenterX, int p1CenterY, int p2CenterX, int p2CenterY);
	void CalculateDistance(int p1CenterX, int p1CenterY, int p2CenterX, int p2CenterY);
	void Update(int levelWidth, int levelHeight);
};

