#pragma once
#include <vector>

class Control
{
public:
	int port;
	std::vector<int> buttonHandles;
	std::vector<bool> buttons;
	std::vector<bool> buttonsPrev;

	std::vector<int> stickHandles;
	std::vector<int> axisHandles;
	std::vector<float> axisPos;

	Control();
	~Control();

	void LoadControls();
};

