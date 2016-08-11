#pragma once
#include <vector>
#include <sstream>

class Control
{
public:
	std::string name;

	std::vector<int> buttonHandles;
	std::vector<std::vector<std::vector<float>>> stickHandles;
	bool buttons[10];
	bool buttonsPrev[10];

	std::vector<std::string> configList[10];

	Control(std::string name);
	Control();
	~Control();

	void PopulateConfigList();
};

