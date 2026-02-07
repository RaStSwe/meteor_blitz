//Meteor Blitz - main
//By RaStSwe

#include "game.hpp"

using namespace meteor_blitz;

int main(int argc, char** argv) {
	//--Make a game instance--
	Game game;
	

	//-Initialize everthing(window, player, etc)-
	game.initialize();
	
		while(game.is_running() && !WindowShouldClose()) {			
			float deltaTime = GetFrameTime(); 
			game.update(deltaTime);
			game.render();
		}	
		
		//--- clean up and ends game ---
		game.end_game();

	return 0;
}