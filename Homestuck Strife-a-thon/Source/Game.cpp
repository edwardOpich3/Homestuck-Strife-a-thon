#include "Header\Game.h"



Game::Game()
{
	display = NULL;
	buffer = NULL;
	timer = NULL;
	event = NULL;

	mainFnt = NULL;
	mainFnt2X = NULL;
	mainFnt3X = NULL;
	mainFnt4X = NULL;

	titleSpr = NULL;

	cursorSpr = NULL;

	soundtrack = NULL;

	isCustomizing = false;
	isCleared = false;
	isOverlapping = false;

	// DEBUG
	tempBitmap1 = NULL;
	tempBitmap2 = NULL;

	currentState = TITLE;
	currentMenu = MAIN;
	width = 1024;
	height = 768;
	FPS = 60.0;
	quit = false;
	redraw = false;
}


Game::~Game()
{
}

bool Game::Initialize()
{
	// Begin the initialization!

	al_init();

	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "Error!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	display = al_create_display(width, height);

	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Failed to create display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	timer = al_create_timer(1.0 / FPS);

	if (!timer)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Failed to create timer!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return false;
	}

	event = al_create_event_queue();

	if (!event)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Failed to create event queue!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return false;
	}

	// Here, we initialize all the addons we use.

	al_install_audio();

	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();
	al_reserve_samples(3);

	// Any file loading we have to do goes here!

	buffer = al_create_bitmap(width, height);

	mainFnt = al_load_bitmap_font("Graphics/Fonts/mainFnt.png");
	mainFnt2X = al_load_bitmap_font("Graphics/Fonts/mainFnt_2X.png");
	mainFnt3X = al_load_bitmap_font("Graphics/Fonts/mainFnt_3X.png");
	mainFnt4X = al_load_bitmap_font("Graphics/Fonts/mainFnt_4X.png");

	titleSpr = al_load_bitmap("Graphics/Menu/title.png");

	//cursorSpr = al_load_bitmap("Graphics/Menu/cursor.png");

	//johnSpr = al_load_bitmap("Graphics/Sprites/john.png");
	//roseSpr = al_load_bitmap("Graphics/Sprites/rose.png");

	//soundtrack[0] = al_load_sample("Audio/Music/john.ogg");
	//soundtrack[1] = al_load_sample("Audio/Music/rose.ogg");
	//soundtrack[2] = al_load_sample("Audio/Music/dave.ogg");
	//soundtrack[3] = al_load_sample("Audio/Music/jade.ogg");
	//soundtrack[4] = al_load_sample("Audio/Music/character_select.ogg");
	soundtrack = al_load_sample("Audio/Music/title.ogg");

	BGM = al_create_sample_instance(soundtrack);
	al_set_sample_instance_playmode(BGM, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(BGM, al_get_default_mixer());

	// NOTE: Later on, move these so that they only happen upon loading any/a level for the first time, respectively
	//	Also, LoadLevels needs to be changed to load only a single level, based on a number passed in.
	reader->LoadLevelNames(&levelNames);

	/*reader->LoadTiles(&tile16List, &tile32List, &tile64List, &tile128List);
	reader->LoadLevel(&levelTiles, &levelTileList, &levelWidth, &levelHeight);
	levelTiles = reader->SeparateTiles(levelTiles, tile32List, tile64List, tile128List);

	//reader->SectionLevel(&levelTiles, width, height);	Take this out?

	reader->DrawLevel(levelTiles, &collisionBitmap, levelWidth, levelHeight, tile16List);
	reader->DrawLevel(levelTiles, &levelBitmap, levelWidth, levelHeight, levelTileList);
	for (unsigned int i = 0; i < levelTileList.size(); i++)
	{
		al_destroy_bitmap(levelTileList[i]);
	}
	levelTileList.clear();*/

	// Set up your window!

	al_set_window_title(display, "Homestuck Strife-a-thon");

	// Icon setting goes here.

	// Install your input!

	al_install_keyboard();
	al_install_joystick();

	// Register your event sources!

	al_register_event_source(event, al_get_keyboard_event_source());
	al_register_event_source(event, al_get_display_event_source(display));
	al_register_event_source(event, al_get_timer_event_source(timer));
	al_register_event_source(event, al_get_joystick_event_source());

	// DEBUG
	tempBitmap1 = al_create_bitmap(256, 256);
	tempBitmap2 = al_create_bitmap(256, 256);

	return true;
}

void Game::SetupInput()
{
	for (int i = al_get_num_joysticks(); i >= 0; i--)
	{
		controllers.push_back(new Control());
		if (i == 0)
		{
			controllers[controllers.size() - 1]->name = "Keyboard";
		}
	}
	for (unsigned int i = 0; i < controllers.size(); i++)
	{
		if (controllers[i]->name == "Keyboard")
		{
			for (int j = 0; j < ALLEGRO_KEY_MAX; j++)
			{
				controllers[i]->buttonHandles.push_back(-1);
			}
		}
		else
		{
			controllers[i]->name = al_get_joystick_name(al_get_joystick(i));
			for (int j = 0; j < al_get_joystick_num_buttons(al_get_joystick(i)); j++)
			{
				controllers[i]->buttonHandles.push_back(-1);
			}
			for (int j = 0; j < al_get_joystick_num_sticks(al_get_joystick(i)); j++)
			{
				controllers[i]->stickHandles.push_back(std::vector<std::vector<float>>(0));
				for (int k = 0; k < al_get_joystick_num_axes(al_get_joystick(i), j); k++)
				{
					controllers[i]->stickHandles[j].push_back(std::vector<float>(4));
					controllers[i]->stickHandles[j][k][0] = -1;
					controllers[i]->stickHandles[j][k][1] = -1;
					controllers[i]->stickHandles[j][k][2] = 0.0f;
					controllers[i]->stickHandles[j][k][3] = 0.0f;
				}
			}
		}

		reader->LoadControls(&controllers[i]);
		controllers[i]->PopulateConfigList();
	}
}

void Game::GetInput(ALLEGRO_EVENT e)
{
	// Time to check for events!
	// If a key is pressed down...

	if (e.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		if (isCustomizing)
		{
			if (e.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
			{
				for (unsigned int i = 0; i < controllers[customizedControl]->buttonHandles.size(); i++)
				{
					if (controllers[customizedControl]->buttonHandles[i] == cursor->selection)
					{
						controllers[customizedControl]->buttonHandles[i] = -1;
					}
				}
				if (controllers[customizedControl]->name != "Keyboard")
				{
					for (unsigned int i = 0; i < controllers[customizedControl]->stickHandles.size(); i++)
					{
						for (unsigned int j = 0; j < controllers[customizedControl]->stickHandles[i].size(); j++)
						{
							if (controllers[customizedControl]->stickHandles[i][j][0] == cursor->selection)
							{
								controllers[customizedControl]->stickHandles[i][j][0] = -1;
							}
							if (controllers[customizedControl]->stickHandles[i][j][1] == cursor->selection)
							{
								controllers[customizedControl]->stickHandles[i][j][1] = -1;
							}
						}
					}
				}

				for (int i = 0; i < 10; i++)
				{
					controllers[customizedControl]->configList[i].clear();
				}
				controllers[customizedControl]->PopulateConfigList();
				isCleared = true;
				isOverlapping = false;
			}
			else if (controllers[customizedControl]->name == "Keyboard")
			{
				if (controllers[customizedControl]->buttonHandles[e.keyboard.keycode] == cursor->selection)
				{
					isOverlapping = false;
					isCleared = false;
					isCustomizing = false;
				}
				else if (controllers[customizedControl]->buttonHandles[e.keyboard.keycode] >= 0)
				{
					isOverlapping = true;
					isCleared = false;
				}
				else
				{
					controllers[customizedControl]->buttonHandles[e.keyboard.keycode] = cursor->selection;
					for (int i = 0; i < 10; i++)
					{
						controllers[customizedControl]->configList[i].clear();
					}
					controllers[customizedControl]->PopulateConfigList();

					for (int i = 0; i < 10; i++)
					{
						controllers[customizedControl]->buttons[i] = false;
					}

					isCustomizing = false;
					isCleared = false;
					isOverlapping = false;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < controllers.size(); i++)
			{
				if (controllers[i]->name == "Keyboard")
				{
					int temp = controllers[i]->buttonHandles[e.keyboard.keycode];
					if (temp >= 0)
					{
						controllers[i]->buttons[temp] = true;
					}
					i = controllers.size();
				}
			}
		}
	}

	// If a key is released...

	else if (e.type == ALLEGRO_EVENT_KEY_UP)
	{
		// When escape is released, ask if user wants to quit!

		if(e.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		{
			quit = true;
		}

		for (unsigned int i = 0; i < controllers.size(); i++)
		{
			if (controllers[i]->name == "Keyboard")
			{
				int temp = controllers[i]->buttonHandles[e.keyboard.keycode];
				if (temp >= 0)
				{
					controllers[i]->buttons[temp] = false;
				}
				i = controllers.size();
			}
		}

		// Insert other released keys here.
	}

	else if (e.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
	{
		if (isCustomizing)
		{
			if (controllers[customizedControl]->name != "Keyboard")
			{
				if (controllers[customizedControl]->buttonHandles[e.joystick.button] == cursor->selection)
				{
					isOverlapping = false;
					isCleared = false;
					isCustomizing = false;
				}
				else if (controllers[customizedControl]->buttonHandles[e.joystick.button] >= 0)
				{
					isOverlapping = true;
					isCleared = false;
				}
				else
				{
					controllers[customizedControl]->buttonHandles[e.joystick.button] = cursor->selection;
					for (int i = 0; i < 10; i++)
					{
						controllers[customizedControl]->configList[i].clear();
					}
					controllers[customizedControl]->PopulateConfigList();

					for (int i = 0; i < 10; i++)
					{
						controllers[customizedControl]->buttons[i] = false;
					}

					isCustomizing = false;
					isCleared = false;
					isOverlapping = false;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < controllers.size(); i++)
			{
				if (controllers[i]->name == al_get_joystick_name(e.joystick.id))
				{
					int temp = controllers[i]->buttonHandles[e.joystick.button];
					if (temp >= 0)
					{
						controllers[i]->buttons[temp] = true;
					}
					i = controllers.size();
				}
			}
		}
	}

	else if (e.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)
	{
		for (unsigned int i = 0; i < controllers.size(); i++)
		{
			if (controllers[i]->name == al_get_joystick_name(e.joystick.id))
			{
				int temp = controllers[i]->buttonHandles[e.joystick.button];
				if (temp >= 0)
				{
					controllers[i]->buttons[temp] = false;
				}
				i = controllers.size();
			}
		}
	}

	else if (e.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
	{
		if (isCustomizing)
		{
			if (controllers[customizedControl]->name != "Keyboard")
			{
				if ((controllers[customizedControl]->stickHandles[e.joystick.stick][e.joystick.axis][0] == cursor->selection && e.joystick.pos <= -0.5) || (controllers[customizedControl]->stickHandles[e.joystick.stick][e.joystick.axis][1] == cursor->selection && e.joystick.pos >= 0.5))
				{
					isOverlapping = false;
					isCleared = false;
					isCustomizing = false;
				}
				else if ((controllers[customizedControl]->stickHandles[e.joystick.stick][e.joystick.axis][0] >= 0 && e.joystick.pos <= -0.5) || (controllers[customizedControl]->stickHandles[e.joystick.stick][e.joystick.axis][1] >= 0 && e.joystick.pos >= 0.5))
				{
					isOverlapping = true;
					isCleared = false;
				}
				else if(abs(e.joystick.pos) > 0.5)
				{
					if (e.joystick.pos < -0.5)
					{
						controllers[customizedControl]->stickHandles[e.joystick.stick][e.joystick.axis][0] = cursor->selection;
					}
					else if (e.joystick.pos > 0.5)
					{
						controllers[customizedControl]->stickHandles[e.joystick.stick][e.joystick.axis][1] = cursor->selection;
					}
					for (int i = 0; i < 10; i++)
					{
						controllers[customizedControl]->configList[i].clear();
					}
					controllers[customizedControl]->PopulateConfigList();

					for (int i = 0; i < 10; i++)
					{
						controllers[customizedControl]->buttons[i] = false;
					}

					isCustomizing = false;
					isCleared = false;
					isOverlapping = false;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < controllers.size(); i++)
			{
				if (controllers[i]->name == al_get_joystick_name(e.joystick.id))
				{
					for (int j = 0; j < al_get_joystick_num_sticks(al_get_joystick(i)); j++)
					{
						if (j == e.joystick.stick)
						{
							for (int k = 0; k < al_get_joystick_num_axes(al_get_joystick(i), j); k++)
							{
								if (k == e.joystick.axis)
								{
									controllers[i]->stickHandles[j][k][2] = e.joystick.pos;
									int temp;
									if (e.joystick.pos < -0.5f && controllers[i]->stickHandles[j][k][3] >= -0.5f)
									{
										temp = controllers[i]->stickHandles[j][k][0];
										if (temp >= 0)
										{
											controllers[i]->buttons[temp] = true;
										}
										temp = controllers[i]->stickHandles[j][k][1];
										if (temp >= 0)
										{
											controllers[i]->buttons[temp] = false;
										}
									}
									else if (e.joystick.pos > 0.5f && controllers[i]->stickHandles[j][k][3] <= 0.5f)
									{
										temp = controllers[i]->stickHandles[j][k][1];
										if (temp >= 0)
										{
											controllers[i]->buttons[temp] = true;
										}
										temp = controllers[i]->stickHandles[j][k][0];
										if (temp >= 0)
										{
											controllers[i]->buttons[temp] = false;
										}
									}
									else if (e.joystick.pos < 0.5f && e.joystick.pos > -0.5f)
									{
										temp = controllers[i]->stickHandles[j][k][0];
										if (temp >= 0)
										{
											controllers[i]->buttons[temp] = false;
										}
										temp = controllers[i]->stickHandles[j][k][1];
										if (temp >= 0)
										{
											controllers[i]->buttons[temp] = false;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	else if (e.type == ALLEGRO_EVENT_JOYSTICK_CONFIGURATION)
	{
		int oldJoy = al_get_num_joysticks();
		al_reconfigure_joysticks();
		int newJoy = al_get_num_joysticks();
		if (newJoy - oldJoy > 0)	// A joystick was added
		{
			// Get the handle on the new joystick by name matching; if one of the joysticks in allegro's vector doesn't match one in mine, then that's the new one!
			for (int i = 0; i < newJoy; i++)
			{
				std::string joyName = al_get_joystick_name(al_get_joystick(i));
				bool match = false;
				for (int j = 0; j < oldJoy; j++)
				{
					if (joyName == controllers[j]->name)
					{
						match = true;
						break;
					}
				}
				if (!match)		// We've found the new controller; time to set it up
				{
					controllers.insert(controllers.end() - 1, new Control(joyName));

					for (int j = 0; j < al_get_joystick_num_buttons(al_get_joystick(i)); j++)
					{
						controllers[controllers.size() - 2]->buttonHandles.push_back(-1);
					}
					for (int j = 0; j < al_get_joystick_num_sticks(al_get_joystick(i)); j++)
					{
						controllers[controllers.size() - 2]->stickHandles.push_back(std::vector<std::vector<float>>(0));
						for (int k = 0; k < al_get_joystick_num_axes(al_get_joystick(i), j); k++)
						{
							controllers[controllers.size() - 2]->stickHandles[j].push_back(std::vector<float>(4));
							controllers[controllers.size() - 2]->stickHandles[j][k][0] = -1;
							controllers[controllers.size() - 2]->stickHandles[j][k][1] = -1;
							controllers[controllers.size() - 2]->stickHandles[j][k][2] = 0.0f;
							controllers[controllers.size() - 2]->stickHandles[j][k][3] = 0.0f;
						}
					}
					reader->LoadControls(&controllers[controllers.size() - 2]);
					controllers[controllers.size() - 2]->PopulateConfigList();
					break;
				}
			}
		}
		else if (newJoy - oldJoy < 0)	// A joystick was removed
		{
			// Like for finding a new joystick, we have to find the perpetrator by name matching; if the inactive joystick's name matches the one in the controller list, then we remove it.
			for (int i = 0; i < oldJoy; i++)
			{
				bool match = false;
				for (int j = 0; j <  newJoy; j++)
				{
					if (controllers[i]->name == al_get_joystick_name(al_get_joystick(j)))
					{
						match = true;
						break;
					}
				}
				if (!match)
				{
					if (currentMenu == CONTROLS && i == customizedControl)
					{
						currentMenu = CONTROLLER_SELECT;
						cursor->selection = 0;
					}
					else if (currentMenu == CONTROLLER_SELECT && cursor->selection > controllers.size() - 2)
					{
						cursor->selection = controllers.size() - 2;
					}

					controllers.erase(controllers.begin() + i);
					for (unsigned int i = 0; i < controllers.size(); i++)
					{
						for (int j = 0; j < 10; j++)
						{
							controllers[i]->buttons[j] = false;
						}
					}
					break;
				}
			}
		}
	}

	// If the 'X' button on the window is pressed...

	else if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		quit = true;

	// If the frame has ended...

	else if (e.type == ALLEGRO_EVENT_TIMER)
	{
		redraw = true;
	}
	return;
}

void Game::Update()
{
	redraw = false;

	switch (currentState)
	{
		case TITLE:
		{
			if (!al_get_sample_instance_playing(BGM))
			{
				BGM = al_create_sample_instance(soundtrack);
				al_set_sample_instance_playmode(BGM, ALLEGRO_PLAYMODE_LOOP);
				al_attach_sample_instance_to_mixer(BGM, al_get_default_mixer());
				//al_play_sample_instance(BGM);
			}
			if (controllers[0]->buttons[PAUSE])
			{
				controllers[0]->buttons[PAUSE] = false;
				al_stop_sample_instance(BGM);

				// Unload all content being used for the title screen
				al_destroy_sample_instance(BGM);
				al_destroy_sample(soundtrack);

				// Then load everything for the menu
				cursorSpr = al_load_bitmap("Graphics/Menu/cursor.png");
				cursor = new Cursor(((5 * width) / 12), 320, cursorSpr);
				soundtrack = al_load_sample("Audio/Music/character_select.ogg");
				BGM = al_create_sample_instance(soundtrack);
				al_set_sample_instance_playmode(BGM, ALLEGRO_PLAYMODE_LOOP);
				al_attach_sample_instance_to_mixer(BGM, al_get_default_mixer());
				//al_play_sample_instance(BGM);

				currentState = MENU;
			}
			break;
		}
		case MENU:
		{
			switch (currentMenu)
			{
				case MAIN:
				{
					if (controllers[0]->buttons[PAUSE] || controllers[0]->buttons[JUMP])
					{
						controllers[0]->buttons[PAUSE] = false;
						controllers[0]->buttons[JUMP] = false;

						switch (cursor->selection)
						{
							case 0:
							{
								currentMenu = CHARACTER;
								cursor->selection = 0;
								break;
							}
							case 1:
							{
								currentMenu = OPTIONS;
								cursor->selection = 0;
								break;
							}
						}
					}
					if (controllers[0]->buttons[UP])
					{
						controllers[0]->buttons[UP] = false;
						cursor->selection--;
						if (cursor->selection < 0)
						{
							cursor->selection = 1;
						}
					}
					if (controllers[0]->buttons[DOWN])
					{
						controllers[0]->buttons[DOWN] = false;
						cursor->selection++;
						if (cursor->selection > 1)
						{
							cursor->selection = 0;
						}
					}
					if (controllers[0]->buttons[ATTACK])
					{
						controllers[0]->buttons[ATTACK] = false;
						currentState = TITLE;
						cursor->selection = 0;
					}
					break;
				}
				case OPTIONS:
				{
					if (controllers[0]->buttons[PAUSE] || controllers[0]->buttons[JUMP])
					{
						controllers[0]->buttons[PAUSE] = false;
						controllers[0]->buttons[JUMP] = false;

						switch (cursor->selection)
						{
							case 0:
							{
								currentMenu = SOUND;
								cursor->selection = 0;
								break;
							}
							case 1:
							{
								currentMenu = VIDEO;
								cursor->selection = 0;
								break;
							}
							case 2:
							{
								currentMenu = CONTROLLER_SELECT;
								cursor->selection = 0;
								break;
							}
						}
					}
					if (controllers[0]->buttons[UP])
					{
						controllers[0]->buttons[UP] = false;
						cursor->selection--;
						if (cursor->selection < 0)
						{
							cursor->selection = 2;
						}
					}
					if (controllers[0]->buttons[DOWN])
					{
						controllers[0]->buttons[DOWN] = false;
						cursor->selection++;
						if (cursor->selection > 2)
						{
							cursor->selection = 0;
						}
					}
					if (controllers[0]->buttons[ATTACK])
					{
						controllers[0]->buttons[ATTACK] = false;
						currentMenu = MAIN;
						cursor->selection = 0;
					}
					break;
				}
				case CHARACTER:
				{
					if (controllers[0]->buttons[PAUSE] || controllers[0]->buttons[JUMP])
					{
						controllers[0]->buttons[PAUSE] = false;
						controllers[0]->buttons[JUMP] = false;
						al_stop_sample_instance(BGM);

						// Unload all content being used for the menu
						al_destroy_sample_instance(BGM);
						al_destroy_sample(soundtrack);

						switch (cursor->selection)
						{
							case 0:
							{
								player1 = new John(128, 128, &controllers[0]);
								player1->sprite = al_load_bitmap("Graphics/Sprites/john.png");

								player2 = new John(896 - 512, 128, &controllers[1]);
								player2->sprite = al_load_bitmap("Graphics/Sprites/john.png");
								soundtrack = al_load_sample("Audio/Music/john.ogg");
								BGM = al_create_sample_instance(soundtrack);
								break;
							}
							case 1:
							{
								player1 = new Rose(128, 128, &controllers[0]);
								player2 = new John(896 - 256, 128, &controllers[1]);
								player1->sprite = al_load_bitmap("Graphics/Sprites/john.png");
								player2->sprite = al_load_bitmap("Graphics/Sprites/john.png");
								soundtrack = al_load_sample("Audio/Music/rose.ogg");
								BGM = al_create_sample_instance(soundtrack);
								break;
							}
						}
						player2->direction = -1;
						/*al_set_sample_instance_playmode(BGM, ALLEGRO_PLAYMODE_LOOP);
						al_attach_sample_instance_to_mixer(BGM, al_get_default_mixer());
						//al_play_sample_instance(BGM);
						camera = new Camera(0, 0);*/
						cursor->selection = 0;
						currentMenu = STAGE;
					}
					if (controllers[0]->buttons[UP])
					{
						controllers[0]->buttons[UP] = false;
						cursor->selection--;
						if (cursor->selection < 0)
						{
							cursor->selection = 1;
						}
					}
					if (controllers[0]->buttons[DOWN])
					{
						controllers[0]->buttons[DOWN] = false;
						cursor->selection++;
						if (cursor->selection > 1)
						{
							cursor->selection = 0;
						}
					}
					if (controllers[0]->buttons[ATTACK])
					{
						controllers[0]->buttons[ATTACK] = false;
						currentMenu = MAIN;
						cursor->selection = 0;
					}
					break;
				}
				case STAGE:
				{
					if (controllers[0]->buttons[PAUSE] || controllers[0]->buttons[JUMP])
					{
						controllers[0]->buttons[PAUSE] = false;
						controllers[0]->buttons[JUMP] = false;

						reader->LoadTiles(&tile16List, &tile32List, &tile64List, &tile128List);
						reader->LoadLevel(&levelTiles, &levelTileList, &levelWidth, &levelHeight, levelNames[cursor->selection].c_str());
						levelTiles = reader->SeparateTiles(levelTiles, tile32List, tile64List, tile128List);

						reader->DrawLevel(levelTiles, &collisionBitmap, levelWidth, levelHeight, tile16List);
						reader->DrawLevel(levelTiles, &levelBitmap, levelWidth, levelHeight, levelTileList);
						for (unsigned int i = 0; i < levelTileList.size(); i++)
						{
							al_destroy_bitmap(levelTileList[i]);
						}

						al_set_sample_instance_playmode(BGM, ALLEGRO_PLAYMODE_LOOP);
						al_attach_sample_instance_to_mixer(BGM, al_get_default_mixer());
						//al_play_sample_instance(BGM);
						camera = new Camera(0, 0);
						currentState = GAME;
					}
					if (controllers[0]->buttons[UP])
					{
						controllers[0]->buttons[UP] = false;
						cursor->selection--;
						if (cursor->selection < 0)
						{
							cursor->selection = levelNames.size() - 1;
						}
					}
					if (controllers[0]->buttons[DOWN])
					{
						controllers[0]->buttons[DOWN] = false;
						cursor->selection++;
						if ((unsigned int)cursor->selection > levelNames.size() - 1)
						{
							cursor->selection = 0;
						}
					}
					if (controllers[0]->buttons[ATTACK])
					{
						controllers[0]->buttons[ATTACK] = false;
						currentMenu = CHARACTER;
						cursor->selection = 0;
						al_destroy_bitmap(player1->sprite);
						player1->sprite = NULL;
					}
					break;
				}
				case SOUND:
				{
					if (controllers[0]->buttons[PAUSE] || controllers[0]->buttons[JUMP])
					{
						controllers[0]->buttons[PAUSE] = false;
						controllers[0]->buttons[JUMP] = false;
						currentMenu = OPTIONS;
						cursor->selection = 0;
					}
					if (controllers[0]->buttons[ATTACK])
					{
						controllers[0]->buttons[ATTACK] = false;
						currentMenu = OPTIONS;
						cursor->selection = 0;
					}
					break;
				}
				case VIDEO:
				{
					if (controllers[0]->buttons[PAUSE] || controllers[0]->buttons[JUMP])
					{
						controllers[0]->buttons[PAUSE] = false;
						controllers[0]->buttons[JUMP] = false;

						switch (cursor->selection)
						{
							case 0:
							{
								currentMenu = RESOLUTION;
								cursor->selection = 0;
								break;
							}
							case 1:
							{
								if (al_get_display_flags(display) & ALLEGRO_FULLSCREEN)
								{
									al_set_new_display_flags(ALLEGRO_WINDOWED);
								}
								else
								{
									al_set_new_display_flags(ALLEGRO_FULLSCREEN);
								}
								al_destroy_display(display);
								al_destroy_bitmap(buffer);

								al_destroy_font(mainFnt);
								al_destroy_font(mainFnt2X);
								al_destroy_font(mainFnt3X);
								al_destroy_font(mainFnt4X);

								al_destroy_bitmap(titleSpr);

								al_destroy_bitmap(tempBitmap1);
								al_destroy_bitmap(tempBitmap2);

								for (unsigned int i = 0; i < tile16List.size(); i++)
								{
									al_destroy_bitmap(tile16List[i]);
								}

								for (unsigned int i = 0; i < levelTileList.size(); i++)
								{
									al_destroy_bitmap(levelTileList[i]);
								}

								al_destroy_bitmap(cursorSpr);

								display = al_create_display(width, height);
								buffer = al_create_bitmap(width, height);

								mainFnt = al_load_bitmap_font("Graphics/Fonts/mainFnt.png");
								mainFnt2X = al_load_bitmap_font("Graphics/Fonts/mainFnt_2X.png");
								mainFnt3X = al_load_bitmap_font("Graphics/Fonts/mainFnt_3X.png");
								mainFnt4X = al_load_bitmap_font("Graphics/Fonts/mainFnt_4X.png");

								titleSpr = al_load_bitmap("Graphics/Menu/title.png");

								tempBitmap1 = al_create_bitmap(256, 256);
								tempBitmap2 = al_create_bitmap(256, 256);

								cursorSpr = al_load_bitmap("Graphics/Menu/cursor.png");
								cursor = new Cursor(((5 * width) / 12), 320, cursorSpr);

								break;
							}
						}
					}
					if (controllers[0]->buttons[UP])
					{
						controllers[0]->buttons[UP] = false;
						cursor->selection--;
						if (cursor->selection < 0)
						{
							cursor->selection = 1;
						}
					}
					if (controllers[0]->buttons[DOWN])
					{
						controllers[0]->buttons[DOWN] = false;
						cursor->selection++;
						if (cursor->selection > 1)
						{
							cursor->selection = 0;
						}
					}
					if (controllers[0]->buttons[ATTACK])
					{
						controllers[0]->buttons[ATTACK] = false;
						currentMenu = OPTIONS;
						cursor->selection = 0;
					}
					break;
				}
				case CONTROLS:
				{
					if (customizedControl > controllers.size())
					{
						currentMenu = OPTIONS;
						cursor->selection = 0;
					}
					else if (!isCustomizing)
					{
						if (controllers[customizedControl]->buttons[PAUSE] || controllers[customizedControl]->buttons[JUMP])
						{
							for (unsigned int i = 0; i < 10; i++)
							{
								controllers[customizedControl]->buttons[i] = false;
							}
							isCustomizing = true;
						}
						if (controllers[customizedControl]->buttons[UP])
						{
							controllers[customizedControl]->buttons[UP] = false;
							cursor->selection--;
							if (cursor->selection < 0)
							{
								cursor->selection = 9;
							}
						}
						if (controllers[customizedControl]->buttons[DOWN])
						{
							controllers[customizedControl]->buttons[DOWN] = false;
							cursor->selection++;
							if (cursor->selection > 9)
							{
								cursor->selection = 0;
							}
						}
						if (controllers[customizedControl]->buttons[ATTACK])
						{
							controllers[customizedControl]->buttons[ATTACK] = false;
							currentMenu = CONTROLLER_SELECT;
							cursor->selection = 0;
						}
					}
					break;
				}
				case RESOLUTION:
				{
					if (controllers[0]->buttons[PAUSE] || controllers[0]->buttons[JUMP])
					{
						controllers[0]->buttons[PAUSE] = false;
						controllers[0]->buttons[JUMP] = false;

						if (al_get_display_flags(display) & ALLEGRO_FULLSCREEN)
						{
							al_set_new_display_flags(ALLEGRO_FULLSCREEN);
						}
						else
						{
							al_set_new_display_flags(ALLEGRO_WINDOWED);
						}
						al_destroy_display(display);
						display = NULL;

						al_destroy_bitmap(buffer);
						buffer = NULL;

						al_destroy_font(mainFnt);
						al_destroy_font(mainFnt2X);
						al_destroy_font(mainFnt3X);
						al_destroy_font(mainFnt4X);

						al_destroy_bitmap(titleSpr);

						al_destroy_bitmap(tempBitmap1);
						al_destroy_bitmap(tempBitmap2);

						al_destroy_bitmap(cursorSpr);

						switch (cursor->selection)
						{
							case 0:
							{
								width = 320;
								height = 240;
								break;
							}
							case 1:
							{
								width = 640;
								height = 480;
								break;
							}
							case 2:
							{
								width = 800;
								height = 600;
								break;
							}
							case 3:
							{
								width = 1024;
								height = 768;
								break;
							}
							case 4:
							{
								width = 1152;
								height = 864;
								break;
							}
							case 5:
							{
								width = 1280;
								height = 960;
								break;
							}
							case 6:
							{
								width = 1400;
								height = 1050;
								break;
							}
							case 7:
							{
								width = 1600;
								height = 1200;
								break;
							}
						}
						display = al_create_display(width, height);

						buffer = al_create_bitmap(width, height);

						mainFnt = al_load_bitmap_font("Graphics/Fonts/mainFnt.png");
						mainFnt2X = al_load_bitmap_font("Graphics/Fonts/mainFnt_2X.png");
						mainFnt3X = al_load_bitmap_font("Graphics/Fonts/mainFnt_3X.png");
						mainFnt4X = al_load_bitmap_font("Graphics/Fonts/mainFnt_4X.png");

						titleSpr = al_load_bitmap("Graphics/Menu/title.png");

						tempBitmap1 = al_create_bitmap(256, 256);
						tempBitmap2 = al_create_bitmap(256, 256);

						cursorSpr = al_load_bitmap("Graphics/Menu/cursor.png");
						cursor = new Cursor(((5 * width) / 12), 320, cursorSpr);
					}
					if (controllers[0]->buttons[UP])
					{
						controllers[0]->buttons[UP] = false;
						cursor->selection--;
						if (cursor->selection < 0)
						{
							cursor->selection = 7;
						}
					}
					if (controllers[0]->buttons[DOWN])
					{
						controllers[0]->buttons[DOWN] = false;
						cursor->selection++;
						if (cursor->selection > 7)
						{
							cursor->selection = 0;
						}
					}
					if (controllers[0]->buttons[ATTACK])
					{
						controllers[0]->buttons[ATTACK] = false;
						currentMenu = VIDEO;
						cursor->selection = 0;
					}
					break;
				}
				case CONTROLLER_SELECT:
				{
					if (controllers[0]->buttons[PAUSE] || controllers[0]->buttons[JUMP])
					{
						controllers[0]->buttons[PAUSE] = false;
						controllers[0]->buttons[JUMP] = false;

						customizedControl = cursor->selection;
						currentMenu = CONTROLS;
						cursor->selection = 0;
					}
					if (controllers[0]->buttons[UP])
					{
						controllers[0]->buttons[UP] = false;
						cursor->selection--;
						if (cursor->selection < 0)
						{
							cursor->selection = controllers.size() - 1;
						}
					}
					if (controllers[0]->buttons[DOWN])
					{
						controllers[0]->buttons[DOWN] = false;
						cursor->selection++;
						if (cursor->selection >= (signed int)controllers.size())
						{
							cursor->selection = 0;
						}
					}
					if (controllers[0]->buttons[ATTACK])
					{
						controllers[0]->buttons[ATTACK] = false;
						currentMenu = OPTIONS;
						cursor->selection = 0;
					}
					break;
				}
			}
			break;
		}
		case GAME:
		{
			// Insert input here.
			if (player1->control[0]->buttons[PAUSE])
			{
				player1->control[0]->buttons[PAUSE] = false;
			}
			if (player1->control[0]->buttons[LEFT])
			{
				player1->Run(controllers[0]->buttons[LEFT], controllers[0]->buttonsPrev[LEFT]);
				player1->Move(-1);
			}
			if (player1->control[0]->buttons[RIGHT])
			{
				player1->Run(controllers[0]->buttons[RIGHT], controllers[0]->buttonsPrev[RIGHT]);
				player1->Move(1);
			}
			if (player1->control[0]->buttons[DOWN])
			{
				player1->FastFall(controllers[0]->buttons[DOWN], controllers[0]->buttonsPrev[DOWN]);
			}
			if (player1->control[0]->buttons[JUMP])
			{
				player1->Jump(JUMP, LEFT, RIGHT);
			}

			// Here goes checking if any buttons are UP->

			if (!player1->control[0]->buttons[LEFT])
			{
				player1->Run(controllers[0]->buttons[LEFT], controllers[0]->buttonsPrev[LEFT]);
			}
			if (!player1->control[0]->buttons[RIGHT])
			{
				player1->Run(controllers[0]->buttons[RIGHT], controllers[0]->buttonsPrev[RIGHT]);
			}
			if (!player1->control[0]->buttons[DOWN])
			{
				if (player1->isCrouching)
				{
					player1->isCrouching = false;
				}
				player1->FastFall(controllers[0]->buttons[DOWN], controllers[0]->buttonsPrev[DOWN]);
			}

			if (player2->control[0]->buttons[PAUSE])
			{
				player2->control[0]->buttons[PAUSE] = false;
			}
			if (player2->control[0]->buttons[LEFT])
			{
				player2->Run(controllers[1]->buttons[LEFT], controllers[1]->buttonsPrev[LEFT]);
				player2->Move(-1);
			}
			if (player2->control[0]->buttons[RIGHT])
			{
				player2->Run(controllers[1]->buttons[RIGHT], controllers[1]->buttonsPrev[RIGHT]);
				player2->Move(1);
			}
			if (player2->control[0]->buttons[DOWN])
			{
				player2->FastFall(controllers[1]->buttons[DOWN], controllers[1]->buttonsPrev[DOWN]);
			}
			if (player2->control[0]->buttons[JUMP])
			{
				player2->Jump(JUMP, LEFT, RIGHT);
			}

			// Here goes checking if any buttons are UP->

			if (!player2->control[0]->buttons[LEFT])
			{
				player2->Run(controllers[1]->buttons[LEFT], controllers[1]->buttonsPrev[LEFT]);
			}
			if (!player2->control[0]->buttons[RIGHT])
			{
				player2->Run(controllers[1]->buttons[RIGHT], controllers[1]->buttonsPrev[RIGHT]);
			}
			if (!player2->control[0]->buttons[DOWN])
			{
				if (player2->isCrouching)
				{
					player2->isCrouching = false;
				}
				player2->FastFall(controllers[1]->buttons[DOWN], controllers[1]->buttonsPrev[DOWN]);
			}

			// Insert interaction calculations here->

			// Insert position updates here->
			camera->CalculateCenter(player1->x + (player1->width / 2), player1->y + (player1->height / 2), player2->x + (player2->width / 2), player2->y + (player2->height / 2));
			camera->CalculateDistance(player1->x + (player1->width / 2), player1->y + (player1->height / 2), player2->x + (player2->width / 2), player2->y + (player2->height / 2));
			camera->Update(levelWidth, levelHeight, width, height);

			player1->Update(JUMP, LEFT, RIGHT);
			// Collision time, boyo! That means that we need to draw a region of the collision bitmap to a new bitmap so we don't expend a shit-ton of time
			// Implement the following when you feel like going through hell in refactoring your collision code
			// ALLEGRO_BITMAP* tempBitmap = al_create_bitmap(256, 256);
			// tempBitmap = al_create_bitmap(256, 256);
			al_set_target_bitmap(tempBitmap1);
			al_clear_to_color(al_map_rgba(0, 0, 0, 0));

			al_draw_bitmap_region(collisionBitmap, player1->x, player1->y, 256, 256, 0, 0, NULL);
			player1->Collision(&tempBitmap1, levelWidth, levelHeight, controllers[0]->buttons[DOWN]);
			player1->Animate(LEFT, RIGHT, DOWN);

			// Efficiency testing; calling collision 8 times per frame is the ultimate stress test!
			player2->Update(JUMP, LEFT, RIGHT);

			al_set_target_bitmap(tempBitmap2);
			al_clear_to_color(al_map_rgba(0, 0, 0, 0));

			al_draw_bitmap_region(collisionBitmap, player2->x, player2->y, 256, 256, 0, 0, NULL);
			player2->Collision(&tempBitmap2, levelWidth, levelHeight, controllers[1]->buttons[DOWN]);
			// player2->Collision(&collisionBitmap, levelWidth, levelHeight, player1->buttons[DOWN]);
			player2->Animate(LEFT, RIGHT, DOWN);

			//al_destroy_bitmap(tempBitmap);
			break;
		}
	}

	for (unsigned int j = 0; j < controllers.size(); j++)
	{
		for (int i = 0; i < 10; i++)
		{
			controllers[j]->buttonsPrev[i] = controllers[j]->buttons[i];
		}
		if (controllers[j]->name != "Keyboard")
		{
			for (unsigned int i = 0; i < controllers[j]->stickHandles.size(); i++)
			{
				for (unsigned int k = 0; k < controllers[j]->stickHandles[i].size(); k++)
				{
					controllers[j]->stickHandles[i][k][3] = controllers[j]->stickHandles[i][k][2];
				}
			}
		}
	}
	return;
}

void Game::Draw()
{
	al_set_target_bitmap(buffer);
	switch (currentState)
	{
		case TITLE:
		{
			al_clear_to_color(al_map_rgb(255, 255, 255));

			// Text Testing
			// al_draw_bitmap(test, 0, 0, NULL);

			al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
			al_draw_text(mainFnt, al_map_rgb(0, 0, 0), width / 2, 240, ALLEGRO_ALIGN_CENTER, "Press Enter");
			break;
		}
		case MENU:
		{
			switch (currentMenu)
			{
				case MAIN:
				{
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
					al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "MAIN MENU");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320, ALLEGRO_ALIGN_CENTER, "STRIFE!");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 352, ALLEGRO_ALIGN_CENTER, "OPTIONS");
					al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, cursor->y + (cursor->selection * 32) - (cursor->height / 4), NULL);
					break;
				}
				case OPTIONS:
				{
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
					al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "OPTIONS");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320, ALLEGRO_ALIGN_CENTER, "SOUND");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 352, ALLEGRO_ALIGN_CENTER, "VIDEO");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 384, ALLEGRO_ALIGN_CENTER, "CONTROLS");
					al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, cursor->y + (cursor->selection * 32) - (cursor->height / 4), NULL);
					break;
				}
				case CHARACTER:
				{
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
					al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "CHOOSE YOUR CHARACTER");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320, ALLEGRO_ALIGN_CENTER, "John Egbert");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 352, ALLEGRO_ALIGN_CENTER, "Rose Lalonde");
					al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, cursor->y + (cursor->selection * 32) - (cursor->height / 4), NULL);
					break;
				}
				case STAGE:
				{
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
					al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "CHOOSE A STAGE");
					for (unsigned int i = 0; i < levelNames.size(); i++)
					{
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320 + (i * 32), ALLEGRO_ALIGN_CENTER, levelNames[i].c_str());
					}
					al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, cursor->y + (cursor->selection * 32) - (cursor->height / 4), NULL);
					break;
				}
				case SOUND:
				{
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
					al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "SOUND");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320, ALLEGRO_ALIGN_CENTER, "lol there's nothing here yet");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 352, ALLEGRO_ALIGN_CENTER, "GO BACK");
					al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, 352 - (cursor->height / 4), NULL);
					break;
				}
				case VIDEO:
				{
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
					al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "VIDEO");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320, ALLEGRO_ALIGN_CENTER, "RESOLUTION");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 352, ALLEGRO_ALIGN_CENTER, "FULLSCREEN");
					al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, cursor->y + (cursor->selection * 32) - (cursor->height / 4), NULL);
					break;
				}
				case CONTROLS:
				{
					if (!isCustomizing)
					{
						al_clear_to_color(al_map_rgb(255, 255, 255));
						al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
						al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "CHOOSE A COMMAND TO CUSTOMIZE");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320, ALLEGRO_ALIGN_CENTER, "Right");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 352, ALLEGRO_ALIGN_CENTER, "Up");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 384, ALLEGRO_ALIGN_CENTER, "Left");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 416, ALLEGRO_ALIGN_CENTER, "Down");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 448, ALLEGRO_ALIGN_CENTER, "Pause");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 480, ALLEGRO_ALIGN_CENTER, "Jump");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 512, ALLEGRO_ALIGN_CENTER, "Attack");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 544, ALLEGRO_ALIGN_CENTER, "Special");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 576, ALLEGRO_ALIGN_CENTER, "Block");
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 608, ALLEGRO_ALIGN_CENTER, "Taunt");

						std::string list;
						for (int i = 0; i < 10; i++)
						{
							list = "";
							for (unsigned int j = 0; j < controllers[customizedControl]->configList[i].size(); j++)
							{
								if (controllers[customizedControl]->name != "Keyboard")
								{
									list += controllers[customizedControl]->configList[i][j];
								}
								else
								{
									list += al_keycode_to_name(atoi(controllers[customizedControl]->configList[i][j].c_str()));
								}
								if (j + 1 < controllers[customizedControl]->configList[i].size())
								{
									list += ", ";
								}
							}
							al_draw_text(mainFnt, al_map_rgb(0, 0, 0), 3 * width / 4, 320 + (32 * i), ALLEGRO_ALIGN_CENTER, list.c_str());
						}

						al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, cursor->y + (cursor->selection * 32) - (cursor->height / 4), NULL);
					}
					else
					{
						al_draw_filled_rectangle(width / 4, height / 4, 3 * width / 4, 3 * height / 4, al_map_rgb(255, 255, 255));
						al_draw_rectangle(width / 4, height / 4, 3 * width / 4, 3 * height / 4, al_map_rgb(0, 0, 0), 2);
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 32, ALLEGRO_ALIGN_CENTER, "Please press desired button/axis. Backspace to clear.");
						if (isCleared)
						{
							al_draw_text(mainFnt, al_map_rgb(0, 0, 0), width / 2, (height / 2) + 32, ALLEGRO_ALIGN_CENTER, "Cleared.");
						}
						else if (isOverlapping)
						{
							al_draw_text(mainFnt, al_map_rgb(0, 0, 0), width / 2, (height / 2) + 32, ALLEGRO_ALIGN_CENTER, "That button/axis is being used for something else.");
						}
						if (cursor->selection == RIGHT)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Right");
						}
						else if (cursor->selection == UP)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Up");
						}
						else if (cursor->selection == LEFT)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Left");
						}
						else if (cursor->selection == DOWN)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Down");
						}
						else if (cursor->selection == PAUSE)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Pause");
						}
						else if (cursor->selection == JUMP)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Jump");
						}
						else if (cursor->selection == ATTACK)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Attack");
						}
						else if (cursor->selection == SPECIAL)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Special");
						}
						else if (cursor->selection == BLOCK)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Block");
						}
						else if (cursor->selection == TAUNT)
						{
							al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), width / 2, (height / 2) - 128, ALLEGRO_ALIGN_CENTER, "Editing: Taunt");
						}
					}
					break;
				}
				case RESOLUTION:
				{
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
					al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "CHOOSE A RESOLUTION");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320, ALLEGRO_ALIGN_CENTER, "320 X 240");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 352, ALLEGRO_ALIGN_CENTER, "640 X 480");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 384, ALLEGRO_ALIGN_CENTER, "800 X 600");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 416, ALLEGRO_ALIGN_CENTER, "1024 X 768");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 448, ALLEGRO_ALIGN_CENTER, "1152 X 864");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 480, ALLEGRO_ALIGN_CENTER, "1280 X 960");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 512, ALLEGRO_ALIGN_CENTER, "1400 X 1050");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 544, ALLEGRO_ALIGN_CENTER, "1600 X 1200");
					al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, cursor->y + (cursor->selection * 32) - (cursor->height / 4), NULL);
					break;
				}
				case CONTROLLER_SELECT:
				{
					al_clear_to_color(al_map_rgb(255, 255, 255));
					al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
					al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "CHOOSE A CONTROLLER TO CUSTOMIZE");
					for (unsigned int i = 0; i < controllers.size(); i++)
					{
						al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320 + (32 * i), ALLEGRO_ALIGN_CENTER, controllers[i]->name.c_str());
					}
					al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, cursor->y + (cursor->selection * 32) - (cursor->height / 4), NULL);
					break;
				}
			}
			break;
		}
		case GAME:
		{
			al_clear_to_color(al_map_rgb(128, 128, 128));

			al_draw_scaled_bitmap(levelBitmap, camera->x, camera->y, width * camera->scale, height * camera->scale, 0, 0, width, height, NULL);

			switch (player1->direction)
			{
				case 1:
				{
					al_draw_scaled_bitmap(player1->sprite, 256 * (player1->frame / 30), 256 * player1->animationState, player1->width, player1->height, (player1->x - camera->x) / camera->scale, (player1->y - camera->y) / camera->scale, player1->width / camera->scale, player1->height / camera->scale, NULL);
					break;
				}
				case -1:
				{
					al_draw_scaled_bitmap(player1->sprite, 256 * (player1->frame / 30), 256 * player1->animationState, player1->width, player1->height, (player1->x - player1->mirrorOffset - camera->x) / camera->scale, (player1->y - camera->y) / camera->scale, player1->width / camera->scale, player1->height / camera->scale, ALLEGRO_FLIP_HORIZONTAL);
					break;
				}
			}
			al_draw_scaled_bitmap(tempBitmap1, 0, 0, 256, 256, (player1->x - camera->x) / camera->scale, (player1->y - camera->y) / camera->scale, player1->width / camera->scale, player1->height / camera->scale, NULL);
			switch (player2->direction)
			{
				case 1:
				{
					//al_draw_bitmap_region(player2->sprite, 256 * (player2->frame / 30), 256 * player2->animationState, player2->width, player2->height, player2->x, player2->y, NULL);
					al_draw_scaled_bitmap(player2->sprite, 256 * (player2->frame / 30), 256 * player2->animationState, player2->width, player2->height, (player2->x - camera->x) / camera->scale, (player2->y - camera->y) / camera->scale, player2->width / camera->scale, player2->height / camera->scale, NULL);
					break;
				}
				case -1:
				{
					//al_draw_bitmap_region(player2->sprite, 256 * (player2->frame / 30), 256 * player2->animationState, player2->width, player2->height, player2->x - player2->mirrorOffset, player2->y, ALLEGRO_FLIP_HORIZONTAL);
					al_draw_scaled_bitmap(player2->sprite, 256 * (player2->frame / 30), 256 * player2->animationState, player2->width, player2->height, (player2->x - player2->mirrorOffset - camera->x) / camera->scale, (player2->y - camera->y) / camera->scale, player2->width / camera->scale, player2->height / camera->scale, ALLEGRO_FLIP_HORIZONTAL);
					break;
				}
			}

			// DEBUG!
			//al_draw_textf(mainFnt3X, al_map_rgb(255, 255, 255), 0, 0, NULL, "%i %i %i", player1->runTimer, buttonsPrev[RIGHT], player1->isRunning);
			al_draw_textf(mainFnt3X, al_map_rgb(255, 255, 255), 0, 0, NULL, "%0.1f", camera->zSpeed);

			break;
		}
	}
	al_set_target_backbuffer(display);
	al_draw_scaled_bitmap(buffer, 0, 0, width, height, 0, 0, width, height, NULL);
	al_flip_display();
	return;
}

void Game::End()
{
	// The user has quit; time to clean up and end the program->

	al_destroy_display(display);
	al_destroy_bitmap(buffer);
	al_destroy_timer(timer);
	al_destroy_event_queue(event);

	al_destroy_font(mainFnt);
	al_destroy_font(mainFnt2X);
	al_destroy_font(mainFnt3X);
	al_destroy_font(mainFnt4X);

	al_destroy_bitmap(titleSpr);

	//al_destroy_bitmap(player1->sprite);
	//al_destroy_bitmap(player2->sprite);

	// DEBUG
	al_destroy_bitmap(tempBitmap1);
	al_destroy_bitmap(tempBitmap2);

	for (unsigned int i = 0; i < tile16List.size(); i++)
	{
		al_destroy_bitmap(tile16List[i]);
	}

	al_destroy_sample_instance(BGM);
	al_destroy_sample(soundtrack);
}