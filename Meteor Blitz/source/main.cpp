//Meteor Blitz - main
//By RaStSwe

#include "game.hpp"

int main(int argc, char** argv) {
	//--Make a game instance--
	meteor_blitz::Game game;

	//-Initialize everthing(window, player, etc)-
	game.initialize();

		while(game.is_running() && !WindowShouldClose()) {			
			float deltaTime = GetFrameTime(); 

			game.update(deltaTime);
			game.render();
		}	
	CloseWindow();
	return 0;
}