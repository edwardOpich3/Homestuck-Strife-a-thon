#include "Header\Game.h"



Game::Game()
{
	display = NULL;
	timer = NULL;
	event = NULL;

	mainFnt = NULL;
	mainFnt2X = NULL;
	mainFnt3X = NULL;
	mainFnt4X = NULL;

	titleSpr = NULL;

	cursorSpr = NULL;

	johnSpr = NULL;
	roseSpr = NULL;

	soundtrack = NULL;

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
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();
	al_reserve_samples(3);

	// Any file loading we have to do goes here!

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
	reader->LoadTiles(&tile16List, &tile32List, &tile64List, &tile128List);
	reader->LoadLevel(&levelTiles, &levelWidth, &levelHeight);
	levelTiles = reader->SeparateTiles(levelTiles, tile32List, tile64List, tile128List);

	reader->SectionLevel(levelTiles, &levelCollision, width, height);

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

	return true;
}

void Game::GetInput(ALLEGRO_EVENT e)
{
	// Time to check for events!
	// If a key is pressed down...

	if (e.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (e.keyboard.keycode)
		{
		case ALLEGRO_KEY_RIGHT:
		{
			buttons[RIGHT] = true;
			break;
		}

		case ALLEGRO_KEY_UP:
		{
			buttons[UP] = true;
			break;
		}

		case ALLEGRO_KEY_LEFT:
		{
			buttons[LEFT] = true;
			break;
		}

		case ALLEGRO_KEY_DOWN:
		{
			buttons[DOWN] = true;
			break;
		}

		case ALLEGRO_KEY_ENTER:
		{
			buttons[ENTER] = true;
			break;
		}

		case ALLEGRO_KEY_Z:
		{
			buttons[Z] = true;
			break;
		}
		}
	}

	// If a key is released...

	else if (e.type == ALLEGRO_EVENT_KEY_UP)
	{
		// When escape is released, ask if user wants to quit!

		switch (e.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
		{
			quit = true;
			break;
		}

		case ALLEGRO_KEY_RIGHT:
		{
			buttons[RIGHT] = false;
			break;
		}

		case ALLEGRO_KEY_UP:
		{
			buttons[UP] = false;
			break;
		}

		case ALLEGRO_KEY_LEFT:
		{
			buttons[LEFT] = false;
			break;
		}

		case ALLEGRO_KEY_DOWN:
		{
			buttons[DOWN] = false;
			break;
		}

		case ALLEGRO_KEY_ENTER:
		{
			buttons[ENTER] = false;
			break;
		}

		case ALLEGRO_KEY_Z:
		{
			buttons[Z] = false;
			break;
		}

		// Insert other released keys here.

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
				al_play_sample_instance(BGM);
			}
			if (buttons[ENTER])
			{
				buttons[ENTER] = false;
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
				al_play_sample_instance(BGM);

				currentState = MENU;
			}
			break;
		}
		case MENU:
		{
			if (buttons[ENTER])
			{
				buttons[ENTER] = false;
				al_stop_sample_instance(BGM);

				// Unload all content being used for the menu
				al_destroy_sample_instance(BGM);
				al_destroy_sample(soundtrack);
				al_destroy_bitmap(cursorSpr);
				
				switch (cursor->selection)
				{
					case 0:
					{
						johnSpr = al_load_bitmap("Graphics/Sprites/john.png");
						roseSpr = al_load_bitmap("Graphics/Sprites/rose.png");
						player1 = new John(johnSpr);
						player2 = new John(johnSpr);
						soundtrack = al_load_sample("Audio/Music/john.ogg");
						BGM = al_create_sample_instance(soundtrack);
						break;
					}
					case 1:
					{
						johnSpr = al_load_bitmap("Graphics/Sprites/john.png");
						roseSpr = al_load_bitmap("Graphics/Sprites/rose.png");
						player1 = new John(johnSpr);
						player2 = new John(johnSpr);
						soundtrack = al_load_sample("Audio/Music/rose.ogg");
						BGM = al_create_sample_instance(soundtrack);
						break;
					}
				}
				player2->x = 896 - player2->width;
				player2->direction = -1;
				al_set_sample_instance_playmode(BGM, ALLEGRO_PLAYMODE_LOOP);
				al_attach_sample_instance_to_mixer(BGM, al_get_default_mixer());
				al_play_sample_instance(BGM);
				currentState = GAME;
			}
			if (buttons[UP])
			{
				buttons[UP] = false;
				cursor->selection--;
				if (cursor->selection < 0)
				{
					cursor->selection = 1;
				}
			}
			if (buttons[DOWN])
			{
				buttons[DOWN] = false;
				cursor->selection++;
				if (cursor->selection > 1)
				{
					cursor->selection = 0;
				}
			}
			break;
		}
		case GAME:
		{
			// Insert input here.

			if (buttons[ENTER])
			{
				buttons[ENTER] = false;
			}
			if (buttons[LEFT])
			{
				player1->Run(buttons[LEFT], buttonsPrev[LEFT]);
				player1->Move(-1);
			}
			if (buttons[RIGHT])
			{
				player1->Run(buttons[RIGHT], buttonsPrev[RIGHT]);
				player1->Move(1);
			}
			if (buttons[DOWN])
			{
				player1->FastFall();
			}
			if (buttons[Z])
			{
				player1->Jump();
			}

			// Here goes checking if any buttons are UP->

			if (!buttons[LEFT])
			{
				player1->Run(buttons[LEFT], buttonsPrev[LEFT]);
			}
			if (!buttons[RIGHT])
			{
				player1->Run(buttons[RIGHT], buttonsPrev[RIGHT]);
			}
			if (!buttons[DOWN])
			{
				if (player1->isCrouching)
				{
					player1->isCrouching = false;
				}
			}

			// Insert interaction calculations here->

			// Insert position updates here->
			player1->Update(buttons, Z);
			player2->Update(buttons, Z);

			break;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		buttonsPrev[i] = buttons[i];
	}
	return;
}

void Game::Draw()
{
	switch (currentState)
	{
		case TITLE:
		{
			al_clear_to_color(al_map_rgb(255, 255, 255));

			// Text Testing
			// al_draw_bitmap(test, 0, 0, NULL);

			al_draw_bitmap(titleSpr, (width / 2) - (al_get_bitmap_width(titleSpr) / 2), 64, NULL);
			al_draw_text(mainFnt, al_map_rgb(0, 0, 0), width / 2, 240, ALLEGRO_ALIGN_CENTER, "Press Enter");
			al_flip_display();
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
					al_draw_text(mainFnt3X, al_map_rgb(0, 0, 0), (width / 2), 256, ALLEGRO_ALIGN_CENTER, "CHOOSE YOUR CHARACTER");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 320, ALLEGRO_ALIGN_CENTER, "John Egbert");
					al_draw_text(mainFnt, al_map_rgb(0, 0, 0), (width / 2), 352, ALLEGRO_ALIGN_CENTER, "Rose Lalonde");
					al_draw_bitmap(cursor->sprite, cursor->x - cursor->width, cursor->y + (cursor->selection * 32) - (cursor->height / 4), NULL);
					al_flip_display();
					break;
				}
			}
			break;
		}
		case GAME:
		{
			al_clear_to_color(al_map_rgb(128, 128, 128));

			for (unsigned int i = 0; i < levelTiles.size(); i++)
			{
				al_draw_bitmap(tile16List[levelTiles[i].type], levelTiles[i].x, levelTiles[i].y, NULL);
			}

			switch (player1->direction)
			{
				case 1:
				{
					al_draw_bitmap(player1->sprite, player1->x, player1->y, NULL);
					break;
				}
				case -1:
				{
					al_draw_bitmap(player1->sprite, player1->x, player1->y, ALLEGRO_FLIP_HORIZONTAL);
					break;
				}
			}
			switch (player2->direction)
			{
				case 1:
				{
					al_draw_bitmap(player2->sprite, player2->x, player2->y, NULL);
					break;
				}
				case -1:
				{
					al_draw_bitmap(player2->sprite, player2->x, player2->y, ALLEGRO_FLIP_HORIZONTAL);
					break;
				}
			}

			// DEBUG!
			//al_draw_textf(mainFnt3X, al_map_rgb(255, 255, 255), 0, 0, NULL, "%i %i %i", player1->runTimer, buttonsPrev[RIGHT], player1->isRunning);

			al_flip_display();
			break;
		}
	}
	return;
}

void Game::End()
{
	// The user has quit; time to clean up and end the program->

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event);

	al_destroy_font(mainFnt);
	al_destroy_font(mainFnt2X);
	al_destroy_font(mainFnt3X);
	al_destroy_font(mainFnt4X);

	al_destroy_bitmap(titleSpr);

	al_destroy_bitmap(johnSpr);
	al_destroy_bitmap(roseSpr);

	for (unsigned int i = 0; i < tile16List.size(); i++)
	{
		al_destroy_bitmap(tile16List[i]);
	}

	al_destroy_sample_instance(BGM);
	al_destroy_sample(soundtrack);
}