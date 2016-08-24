#pragma once
#include <vector>
#include <sstream>

// A class to represent a controller or keyboard.

class Control
{
public:
	std::string name;	// The title of the device in question. For the keyboard, this is "Keyboard".

	std::vector<int> buttonHandles;									// A vector for every button on the device. Contains the command it's assigned to.
	std::vector<std::vector<std::vector<float>>> stickHandles;		// A vector for every stick on the device. Contains a vector for each axis on that stick, which contains 4 floats. The first two are negative and positive axis commands, the second two are current and previous axis positions.
	bool buttons[10];												// The current list of commands. If one is being input, its slot is set to true.
	bool buttonsPrev[10];											// The list of commands from the previous frame. Always based off of the current buttons list.

	std::vector<std::string> configList[10];	// A vector of strings solely used for the controller config screen, to tell the player which button does what.

	Control(std::string name);	// Use this constructor.
	Control();
	~Control();

	void Control::operator=(Control other);	// Self explanatory, but I'm explaining it anyway. Makes the left Control object exactly the same as the one on the right.

	void PopulateConfigList();	// Populates the config list with the updated control assignments.
};

