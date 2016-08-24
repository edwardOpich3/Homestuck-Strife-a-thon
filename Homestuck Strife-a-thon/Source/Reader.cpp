#include "..\Header\Reader.h"



Reader::Reader()
{
}


Reader::~Reader()
{
}

void Reader::LoadTiles(std::vector<ALLEGRO_BITMAP*> *tile16List, std::vector<std::vector<Tile>> *tile32List, std::vector<std::vector<Tile>> *tile64List, std::vector<std::vector<Tile>> *tile128List)
{
	std::ifstream myStream;

	// Load in the size 16 tiles
	std::string path = "Levels/tiles16/tiles16_0.png";
	for (int i = 1; al_filename_exists(path.c_str()); i++)
	{
		tile16List->push_back(al_load_bitmap(path.c_str()));
		path = "Levels/tiles16/tiles16_"; path += std::to_string(i); path += ".png";
	}
	
	// Load in the size 32 tiles' data
	path = "Levels/tiles32/tiles32_0.tile";
	for (int i = 1; al_filename_exists(path.c_str()); i++)
	{
		myStream.open(path.c_str(), myStream.binary | myStream.in);
		tile32List->push_back(std::vector<Tile>());
		unsigned char temp[4];
		while (myStream.peek() != -1)
		{
			myStream.read((char*)temp, 4);
			int type = ReadInt(temp);

			myStream.read((char*)temp, 4);
			int x = ReadInt(temp);

			myStream.read((char*)temp, 4);
			int y = ReadInt(temp);

			myStream.read((char*)temp, 4);
			int size = ReadInt(temp);

			tile32List[0][i - 1].push_back(Tile(type, x, y, size));
		}
		myStream.close();
		path = "Levels/tiles32/tiles32_"; path += std::to_string(i); path += ".tile";
	}

	// Load in the size 64 tiles' data
	path = "Levels/tiles64/tiles64_0.tile";
	for (int i = 1; al_filename_exists(path.c_str()); i++)
	{
		myStream.open(path.c_str(), myStream.binary | myStream.in);
		tile64List->push_back(std::vector<Tile>());
		unsigned char temp[4];
		while (myStream.peek() != -1)
		{
			myStream.read((char*)temp, 4);
			int type = ReadInt(temp);

			myStream.read((char*)temp, 4);
			int x = ReadInt(temp);

			myStream.read((char*)temp, 4);
			int y = ReadInt(temp);

			myStream.read((char*)temp, 4);
			int size = ReadInt(temp);

			tile64List[0][i - 1].push_back(Tile(type, x, y, size));
		}
		myStream.close();
		path = "Levels/tiles64/tiles64_"; path += std::to_string(i); path += ".tile";
	}
	
	// Load in the size 128 tiles' data
	path = "Levels/tiles128/tiles128_0.tile";
	for (int i = 1; al_filename_exists(path.c_str()); i++)
	{
		myStream.open(path.c_str(), myStream.binary | myStream.in);
		unsigned char temp[4];
		tile128List->push_back(std::vector<Tile>());
		while (myStream.peek() != -1)
		{
			myStream.read((char*)temp, 4);
			int type = ReadInt(temp);

			myStream.read((char*)temp, 4);
			int x = ReadInt(temp);

			myStream.read((char*)temp, 4);
			int y = ReadInt(temp);

			myStream.read((char*)temp, 4);
			int size = ReadInt(temp);

			tile128List[0][i - 1].push_back(Tile(type, x, y, size));
		}
		myStream.close();
		path = "Levels/tiles128/tiles128_"; path += std::to_string(i); path += ".tile";
	}
}

void Reader::LoadLevel(std::vector<Tile> *level, std::vector<ALLEGRO_BITMAP*> *levelTileList, int *width, int *height, const char* levelName)
{
	// Load in the size 16 tiles
	std::string path = "Graphics/Levels/0 - LOWAS/tiles/0.png";
	for (int i = 1; al_filename_exists(path.c_str()); i++)
	{
		levelTileList->push_back(al_load_bitmap(path.c_str()));
		path = "Graphics/Levels/0 - LOWAS/tiles/"; path += std::to_string(i); path += ".png";
	}

	std::ifstream myStream;
	unsigned char temp[4];
	path = "Levels/"; path += levelName; path += ".lvl";
	myStream.open(path, myStream.binary | myStream.in);

	myStream.read((char*)temp, 4);
	*width = ReadInt(temp);

	myStream.read((char*)temp, 4);
	*height = ReadInt(temp);

	while (myStream.peek() != -1)
	{
		myStream.read((char*)temp, 4);
		int type = ReadInt(temp);

		myStream.read((char*)temp, 4);
		int x = ReadInt(temp);

		myStream.read((char*)temp, 4);
		int y = ReadInt(temp);

		myStream.read((char*)temp, 4);
		int size = ReadInt(temp);

		level->push_back(Tile(type, x, y, size));
	}
	myStream.close();
}

void Reader::LoadLevelNames(std::vector<std::string> *levelNames)
{
	ALLEGRO_FS_ENTRY* directory = al_create_fs_entry("Levels");
	al_open_directory(directory);

	ALLEGRO_FS_ENTRY* file;
	while (file = al_read_directory(directory))
	{
		if (al_get_fs_entry_mode(file) & ALLEGRO_FILEMODE_ISFILE)
		{
			std::string currentName = al_get_fs_entry_name(file);
			std::string path = al_get_fs_entry_name(directory);
			currentName = currentName.substr(path.size() + 1, currentName.npos);
			currentName = currentName.substr(0, currentName.size() - 4);
			levelNames->push_back(currentName);
			al_destroy_fs_entry(file);
		}
	}
	al_destroy_fs_entry(directory);
}

int Reader::ReadInt(unsigned char temp[4])
{
	int myInt = 0;
	myInt += temp[0];
	myInt += (256 * temp[1]);
	myInt += (256 * 256 * temp[2]);
	myInt += (256 * 256 * 256 * temp[3]);
	return myInt;
}

std::vector<Tile> Reader::SeparateTiles(std::vector<Tile> myLevel, std::vector<std::vector<Tile>> tile32List, std::vector<std::vector<Tile>> tile64List, std::vector<std::vector<Tile>> tile128List)
{
	std::vector<Tile> temp;
	for (unsigned int i = 0; i < myLevel.size(); i++)
	{
		switch (myLevel[i].dimension)
		{
			case 16:
			{
				temp.push_back(myLevel[i]);
				break;
			}
			case 32:
			{
				std::vector<Tile> tempTemp = SeparateTiles(tile32List[myLevel[i].type], tile32List, tile64List, tile128List);
				for (unsigned int t = 0; t < tempTemp.size(); t++)
				{
					temp.push_back(Tile(tempTemp[t].type, myLevel[i].x + tempTemp[t].x, myLevel[i].y + tempTemp[t].y, tempTemp[t].dimension));
				}
				break;
			}
			case 64:
			{
				std::vector<Tile> tempTemp = SeparateTiles(tile64List[myLevel[i].type], tile32List, tile64List, tile128List);
				for (unsigned int t = 0; t < tempTemp.size(); t++)
				{
					temp.push_back(Tile(tempTemp[t].type, myLevel[i].x + tempTemp[t].x, myLevel[i].y + tempTemp[t].y, tempTemp[t].dimension));
				}
				break;
			}
			case 128:
			{
				std::vector<Tile> tempTemp = SeparateTiles(tile128List[myLevel[i].type], tile32List, tile64List, tile128List);
				for (unsigned int t = 0; t < tempTemp.size(); t++)
				{
					temp.push_back(Tile(tempTemp[t].type, myLevel[i].x + tempTemp[t].x, myLevel[i].y + tempTemp[t].y, tempTemp[t].dimension));
				}
				break;
			}
		}
	}
	return temp;
}

// Outdated code; Delete when it's confirmed we won't need it again
/*void Reader::SectionLevel(std::vector<std::vector<Tile>> *level, int width, int height)
{
	std::vector<std::vector<Tile>> temp;
	for (int t = (width / 128) * (height / 128); t > 0; t--)
	{
		temp.push_back(std::vector<Tile>());
	}
	for (unsigned int i = 0; i < level[0][0].size(); i++)
	{
		int index = (level[0][0][i].y / 128) * (level[0][0][i].x / 128);
		temp[index].push_back(level[0][0][i]);
	}
	level->clear();
	for (unsigned int i = 0; i < temp.size(); i++)
	{
		level->push_back(temp[i]);
	}
}*/

void Reader::DrawLevel(std::vector<Tile> level, ALLEGRO_BITMAP **levelBitmap, int width, int height, std::vector<ALLEGRO_BITMAP*> tile16List)
{
	*levelBitmap = al_create_bitmap(width, height);
	al_set_target_bitmap(*levelBitmap);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));

	for (unsigned int i = 0; i < level.size(); i++)
	{
		al_draw_bitmap(tile16List[level[i].type], level[i].x, level[i].y, NULL);
	}
}

void Reader::LoadControls(std::vector<Control*> *controllers){

	if (!al_filename_exists("Config/controls.dat"))
	{
		return;
	}
	else
	{
		std::vector<std::string> controllerNames;
		std::ifstream myStream;
		bool match = false;
		unsigned char tempString = 0;
		myStream.open("Config/controls.dat", myStream.binary | myStream.in);

		// First, search for the name we need
		for (unsigned int i = 0; i < controllers->size(); i++)
		{
			if (myStream.peek() == -1)
			{
				break;
			}
			match = false;
			controllerNames.push_back(std::string());
			while (myStream.peek() != (int)'\0' && myStream.peek() != -1)	// We're writing null-terminated strings up in this bitch
			{
				myStream.read((char *)tempString, 1);
				controllerNames[i] += tempString;
			}
			myStream.read((char *)tempString, 1);

			for (unsigned int j = 0; j < controllers->size(); j++)
			{
				if (controllers[0][j]->name == controllerNames[i])
				{
					match = true;
					break;
				}
			}

			if (match)	// Found the name, have a match, now load in the data into the appropriate places.
			{
				for (unsigned int j = 0; j < controllers[0][i]->buttonHandles.size(); j++)
				{
					myStream.read((char *)tempString, 1);
					controllers[0][i]->buttonHandles[j] = (tempString % 12) - 1;	// Note; this means that you have to write the file with all commands added by one! REMEMBER THIS!
				}
				for (unsigned int j = 0; j < controllers[0][i]->stickHandles.size(); j++)
				{
					for (unsigned int k = 0; k < controllers[0][i]->stickHandles[j].size(); k++)
					{
						myStream.read((char *)tempString, 1);
						controllers[0][i]->stickHandles[j][k][0] = (tempString % 12) - 1;	// Guess what this means?

						myStream.read((char *)tempString, 1);
						controllers[0][i]->stickHandles[j][k][1] = (tempString % 12) - 1;	// Alternate your axes, negative first
					}
				}
				myStream.read((char *)tempString, 1);	// Read that 12 char that divides controllers!
			}
			else
			{
				while (myStream.peek() != 12 && myStream.peek() != -1)	// You see, I have to do 12 here instead of a null char because null chars are an actual value here.
				{
					myStream.read((char *)tempString, 1);
				}
				myStream.read((char *)tempString, 1);	// Read the 12 char that divides controllers!
			}
		}
	}
}