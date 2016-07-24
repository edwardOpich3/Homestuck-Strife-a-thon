#include "..\Header\Camera.h"



Camera::Camera(int x, int y, int width, int height)
{
	Camera::x = x;
	Camera::y = y;
	Camera::width = width;
	Camera::height = height;

	xSpeed = 0;
	ySpeed = 0;
	zSpeed = 0;
	center = 0;
	distance = 0;

	maxXSpeed = 10.0;
	maxYSpeed = 7.5;
	maxZSpeed = 10.0;
	xAcc = 0.5;
	yAcc = 0.375;
	zAcc = 0.5;
}

Camera::Camera()
{
}


Camera::~Camera()
{
}


void Camera::CalculateCenter()
{

}

void Camera::CalculateDistance()
{

}

void Camera::Update()
{

}