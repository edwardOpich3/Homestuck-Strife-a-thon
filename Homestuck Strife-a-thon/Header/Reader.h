#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <allegro5/allegro.h>
#include "Tile.h"
#include "Header/Control.h"

// A class to hold functions for loading in data.

class Reader
{
public:
	Reader();
	~Reader();

	void LoadTiles(std::vector<ALLEGRO_BITMAP*> *tile16List, std::vector<std::vector<Tile>> *tile32List, std::vector<std::vector<Tile>> *tile64List, std::vector<std::vector<Tile>> *tile128List);	// Loads in all of the tiles.
	void LoadLevel(std::vector<Tile> *level, std::vector<ALLEGRO_BITMAP*> *levelTileList, int *width, int *height, const char* levelName);	// Loads in a level.
	void LoadLevelNames(std::vector<std::string> *levelNames);	// Loads the level names.

	int ReadInt(unsigned char temp[4]);		// Converts 32 bits to an integer.
	void WriteInt(int myInt, unsigned char *temp);	// Converts an integer to 32 bits.

	std::vector<Tile> SeparateTiles(std::vector<Tile> myLevel, std::vector<std::vector<Tile>> tile32List, std::vector<std::vector<Tile>> tile64List, std::vector<std::vector<Tile>> tile128List);	// Converts 32, 64, and 128 size tiles into 16 size tiles.
	void DrawLevel(std::vector<Tile> level, ALLEGRO_BITMAP **levelBitmap, int width, int height, std::vector<ALLEGRO_BITMAP*> tile16List);	// Draws the level to a bitmap, when converted to 16 size tiles. This works for both the visible map and the collision map, just pass in a different set of tiles.

	void LoadControls(std::vector<Control*> *controllers);	// Loads the control configs.
	void LoadPortConfig(std::vector<Control*> *controllers);	// Loads the port configuration.
	void LoadVideo(ALLEGRO_DISPLAY **display, int *width, int *height);		// Loads the video settings; Resolution and Fullscreen.

	void WriteInput(Control *myControl);	// Someone configured their input, now we get to write it to the file. Carefully.
	void WritePortConfig(std::vector<Control*> *controllers);	// The port configuration has been adjusted, either because someone unplugged or cause they used the menu. Re-order accordingly.
	void WriteVideo(ALLEGRO_DISPLAY *display);		// Writes the video settings.
};

