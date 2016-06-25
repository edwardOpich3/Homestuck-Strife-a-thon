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
	void LoadLevel(std::vector<Tile> *level, std::vector<ALLEGRO_BITMAP*> *levelTileList, int *width, int *height);

	int ReadInt(unsigned char temp[4]);
	std::vector<Tile> SeparateTiles(std::vector<Tile> myLevel, std::vector<std::vector<Tile>> tile32List, std::vector<std::vector<Tile>> tile64List, std::vector<std::vector<Tile>> tile128List);
	//void SectionLevel(std::vector<std::vector<Tile>> *level, int width, int height);	Outdated; Delete when confirmed no longer needed
	void DrawLevel(std::vector<Tile> level, ALLEGRO_BITMAP **levelBitmap, int width, int height, std::vector<ALLEGRO_BITMAP*> tile16List);
};

