#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <allegro5/allegro.h>
#include "Tile.h"

class Reader
{
public:
	Reader();
	~Reader();

	void LoadTiles(std::vector<ALLEGRO_BITMAP*> *tile16List, std::vector<std::vector<Tile>> *tile32List, std::vector<std::vector<Tile>> *tile64List, std::vector<std::vector<Tile>> *tile128List);
	void LoadLevel(std::vector<Tile> *level, int *width, int *height);

	int ReadInt(unsigned char temp[4]);
	std::vector<Tile> SeparateTiles(std::vector<Tile> myLevel, std::vector<std::vector<Tile>> tile32List, std::vector<std::vector<Tile>> tile64List, std::vector<std::vector<Tile>> tile128List);
	void SectionLevel(std::vector<Tile> level, std::vector<std::vector<Tile>> *levelCollision, int width, int height);
};

