#include "..\Header\Control.h"



Control::Control(std::string name)
{
	Control::name = name;
}


Control::Control()
{
}


Control::~Control()
{
}


void Control::operator=(Control newControl)
{
	name = newControl.name;
	buttonHandles = newControl.buttonHandles;
	stickHandles = newControl.stickHandles;

	for (int i = 0; i < 10; i++)
	{
		buttons[i] = newControl.buttons[i];
		buttonsPrev[i] = newControl.buttonsPrev[i];
		configList[i] = newControl.configList[i];
	}
}

void Control::PopulateConfigList()
{
	for (int i = 0; i < 10; i++)
	{
		for (unsigned int j = 0; j < buttonHandles.size(); j++)
		{
			if (buttonHandles[j] == i)
			{
				std::string button;
				if (name != "Keyboard")
				{
					button = "Button ";
				}
				std::ostringstream convert;
				convert << j;
				button += convert.str();
				configList[i].push_back(button);
			}
		}
		if (name != "Keyboard")
		{
			int axisNumber = 0;
			for (unsigned int j = 0; j < stickHandles.size(); j++)
			{
				for (unsigned int k = 0; k < stickHandles[j].size(); k++)
				{
					std::string axis;
					if (stickHandles[j][k][0] == i)
					{
						axis = "- Axis ";
						std::ostringstream convert;
						convert << axisNumber;
						axis += convert.str();
						configList[i].push_back(axis);
						convert.clear();
					}
					if (stickHandles[j][k][1] == i)
					{
						axis = "+ Axis ";
						std::ostringstream convert;
						convert << axisNumber;
						axis += convert.str();
						configList[i].push_back(axis);
					}
					axisNumber++;
				}
			}
		}
	}
}