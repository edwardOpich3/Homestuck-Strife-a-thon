#pragma once
#include "allegro5\allegro.h"

class Camera
{
public:

	int x, y;
	int width, height;
	float xSpeed, ySpeed, zSpeed;
	float maxXSpeed, maxYSpeed, maxZSpeed;
	float xAcc, yAcc, zAcc;
	int center;
	int distance;

	// Constructor & Destructor
	Camera(int x, int y, int width, int height);
	Camera();
	~Camera();

	// Functions
	void CalculateCenter();
	void CalculateDistance();
	void Update();
};

