#pragma once
#include "Character.h"

// This is Rose, a character.

class Rose : public Character
{
public:
	Rose(int spawnX, int spawnY, Control** control);	// Use this constructor.
	Rose();
	~Rose();
};

