#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <fstream>

#include "Header\Game.h"

using namespace std;

int main(int argc, char *argv[])
{
	Game game = Game();

	if (!game.Initialize())
	{
		return -1;
	}

	game.SetupInput();

	al_start_timer(game.timer);

	// And start the game loop!
	while(!game.quit)
	{
		//  Declare the event variable.

		ALLEGRO_EVENT e;
		al_wait_for_event(game.event, &e);

		game.GetInput(e);

		// If it's time to update the frame...

		if(game.redraw == true && al_is_event_queue_empty(game.event))
		{
			game.Update();

			game.Draw();
		}
	}

	game.End();

	return 0;

}