#pragma once
#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_primitives.h>
#include "Header\John.h"
#include "Header\Rose.h"
#include "Header\Cursor.h"
#include "Header\Tile.h"
#include "Header\Reader.h"
#include "Header\Camera.h"
class Game
{
public:
	// Technical Stuff
	ALLEGRO_DISPLAY *display;	// The handle of the display.
	ALLEGRO_BITMAP *buffer;		// The handle of the screen buffer; everything is drawn to this before the screen.
	ALLEGRO_TIMER *timer;		// The handle of the timer, used to keep the game's pace at 60FPS.
	ALLEGRO_EVENT_QUEUE *event;	// The handle of the event queue. Used to keep track of recieved events.

	// Fonts
	ALLEGRO_FONT *mainFnt;		// The normal sized font.
	ALLEGRO_FONT *mainFnt2X;	// The 2X sized font.
	ALLEGRO_FONT *mainFnt3X;	// The 3X sized font.
	ALLEGRO_FONT *mainFnt4X;	// The 4X sized font.

	// Graphics
	ALLEGRO_BITMAP *titleSpr;	// The title screen graphic.
	
	ALLEGRO_BITMAP *cursorSpr;	// The cursor's sprite; this should be abolished.

	// Music
	ALLEGRO_SAMPLE *soundtrack;	// The current song.

	ALLEGRO_SAMPLE_INSTANCE *BGM;	// The handle of the current song to be played.

	// Controls
	std::vector<Control*> controllers;	// The vector containing every controller currently plugged in, including the keyboard
	bool isCustomizing;					// Is a player currently adjusting their controls?
	bool isCleared;						// Has the player cleared a command of controls?
	bool isOverlapping;					// Are two commands overlapping a control?
	unsigned int customizedControl;		// The controller that's being customized.

	// Players
	Character *player1;	// Player 1. Will later be replaced by a vector of players.
	Character *player2;	// Player 2.
	Character *player3;	// Player 3. Likely won't be used.
	Character *player4;	// Player 4. Likely won't be used.

	// Tiles
	std::vector<ALLEGRO_BITMAP*> tile16List;	// Collision Tiles of size 16X16
	std::vector<ALLEGRO_BITMAP*> levelTileList;	// Pretty Tiles of size 16X16. This is what the player actually sees.
	std::vector<std::vector<Tile>> tile32List;	// Data for 32X32 tiles consisting of 16X16 tiles at various positions.
	std::vector<std::vector<Tile>> tile64List;	// Data for 64X64 tiles consisting of smaller tiles.
	std::vector<std::vector<Tile>> tile128List;	// Data for 128X128 tiles consisting of smaller tiles.

	// Levels
	std::vector<Tile> levelTiles;	// The list containing the level in 16X16 tiles.
	ALLEGRO_BITMAP *levelBitmap;	// The bitmap of the level that's drawn to the screen each frame.
	ALLEGRO_BITMAP *collisionBitmap;	// The bitmap of the level that's used to calculate collision data.
	int levelWidth, levelHeight;		// The width and height boundaries of the level. These are known as the "blast zones".
	std::vector<std::string> levelNames;	// The list of level names. Populated on opening the level select.

	// Cursor
	Cursor *cursor;		// A cursor.

	// Reader
	Reader *reader;		// A reader to load in data.

	// Camera
	Camera *camera;		// The camera.

	// DEBUG
	ALLEGRO_BITMAP* tempBitmap1;	// A temporary bitmap to hold collision data.
	ALLEGRO_BITMAP* tempBitmap2;	// A temporary bitmap to hold collision data for player 2.

	// Technical Variables
	enum BUTTONS { RIGHT, UP, LEFT, DOWN, PAUSE, JUMP, ATTACK, SPECIAL, BLOCK, TAUNT };		// An enum for all of the commands.
	enum GAME_STATES { SPLASH, TITLE, MENU, GAME };											// An enum for all of the game states.
	enum MENUS { MAIN, OPTIONS, CHARACTER, STAGE, SOUND, VIDEO, CONTROLS, RESOLUTION, CONTROLLER_SELECT, PORT_CONFIG };		// An enum for all of the menu states.

	int currentState;	// The current game state.
	int currentMenu;	// The current menu.
	int width;			// The width of the window.
	int height;			// The height of the window.
	double FPS;			// The constant FPS the game will try to run at. This should always be 60.
	bool quit;			// Has the player quit?
	bool redraw;		// Is it time to calculate another frame?

	Game();
	~Game();

	// Game Functions
	bool Initialize();	// Sets up the game
	void SetupInput();	// Inital input setup; only ran once, does an initial check of controllers and keyboard plugged in, and sets them to actually be able to interact.
	void WriteInput(Control *myControl);	// Someone configured their input, now we get to write it to the file. Carefully.
	void GetInput(ALLEGRO_EVENT e);	// Gets input from every registered source.
	void Update();	// Updates coordinates and states.
	void Draw();	// Draws to the screen.
	void End();	// Terminates the program.
};

