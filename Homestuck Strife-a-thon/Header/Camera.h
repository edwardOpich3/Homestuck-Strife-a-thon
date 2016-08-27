#pragma once
#include "allegro5\allegro.h"
#include <cmath>

// Class to contain the camera used during the battle scenes

class Camera
{
public:

	int x, y;	// Position
	float scale;	// The scale, a ratio of resolution to camera size
	float xSpeed, ySpeed, zSpeed;	// CURRENTLY UNIMPLEMENTED: The speed the camera is currently travelling horizontally, vertically, and zoom-ways
	float maxXSpeed, maxYSpeed, maxZSpeed;	// Also unimplemented; the max speed values
	float xAcc, yAcc, zAcc;	// The constant of acceleration in any of the directions, to be added when accelerating, and subtracted when decelerating.
	int centerX, centerY;	// The position of the center of the camera; it is scaled and positioned relative to this.
	float distance;		// The distance from the center point to the furthest player; the scale is based on this.
	float distanceX, distanceY;		// Used to stop the camera from bugging out
	float minDist;	// The constant for the minimum allowed distance from the center; this determines the smallest the scale can be.

	// Constructor & Destructor
	Camera(int x, int y);	// Only use this constructor!
	Camera();
	~Camera();

	// Functions
	void CalculateCenter(int p1CenterX, int p1CenterY, int p2CenterX, int p2CenterY);	// Calculates the center position based on every player's center; needs some adjusting to handle an arbitrary number of players
	void CalculateDistance(int p1CenterX, int p1CenterY, int p2CenterX, int p2CenterY);	// Calculates the distance based on every player's center; needs some adjusting to handle an arbitrary number of players
	void Update(int levelWidth, int levelHeight, int width, int height);	// The big show; calculates the camera's x, y, and scale based on the center, distance, and the stage's boundaries. Needs some work to stop glitching near the boundaries.
};

