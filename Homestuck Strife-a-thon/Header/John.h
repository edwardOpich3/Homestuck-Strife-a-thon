#pragma once
#include "Character.h"

// This is John, a character.

class John : public Character
{
public:
	John(int spawnX, int spawnY, Control **control);	// Use this constructor.
	John();
	~John();
};

