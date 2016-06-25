#pragma once
#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "Header\John.h"
#include "Header\Rose.h"
#include "Header\Cursor.h"
#include "Header\Tile.h"
#include "Header\Reader.h"
class Game
{
public:
	// Technical Stuff
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *event;

	// Fonts
	ALLEGRO_FONT *mainFnt;
	ALLEGRO_FONT *mainFnt2X;
	ALLEGRO_FONT *mainFnt3X;
	ALLEGRO_FONT *mainFnt4X;

	// Graphics
	ALLEGRO_BITMAP *titleSpr;

	ALLEGRO_BITMAP *cursorSpr;

	// Music
	ALLEGRO_SAMPLE *soundtrack;

	ALLEGRO_SAMPLE_INSTANCE *BGM;

	// Characters
	ALLEGRO_BITMAP *johnSpr;
	ALLEGRO_BITMAP *roseSpr;

	// Players
	Character *player1;
	Character *player2;
	Character *player3;
	Character *player4;

	// Tiles
	std::vector<ALLEGRO_BITMAP*> tile16List;	// Collision Tiles
	std::vector<ALLEGRO_BITMAP*> levelTileList;	// Pretty Tiles
	std::vector<std::vector<Tile>> tile32List;
	std::vector<std::vector<Tile>> tile64List;
	std::vector<std::vector<Tile>> tile128List;

	// Levels
	std::vector<Tile> levelTiles;
	ALLEGRO_BITMAP *levelBitmap;
	ALLEGRO_BITMAP *collisionBitmap;
	int levelWidth, levelHeight;

	// Cursor
	Cursor *cursor;

	// Reader
	Reader *reader;

	// Technical Variables
	enum BUTTONS { RIGHT, UP, LEFT, DOWN, ENTER, Z };
	enum GAME_STATES { SPLASH, TITLE, MENU, GAME };
	enum MENUS { MAIN, OPTIONS, CHARACTER };
	bool buttons[6] = { false, false, false, false, false, false };
	bool buttonsPrev[6] = { false, false, false, false, false, false };
	int currentState;
	int currentMenu;
	int width;
	int height;
	double FPS;
	bool quit;
	bool redraw;

	Game();
	~Game();

	// Game Functions
	bool Initialize();
	void GetInput(ALLEGRO_EVENT e);
	void Update();
	void Draw();
	void End();
};
