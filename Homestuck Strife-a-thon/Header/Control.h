#pragma once
#include <vector>

class Control
{
public:
	std::string name;

	std::vector<int> buttonHandles;
	std::vector<std::vector<std::vector<float>>> stickHandles;
	bool buttons[10];
	bool buttonsPrev[10];

	std::vector<char> configList[10];

	Control(std::string name);
	Control();
	~Control();

	void LoadControls();
};

