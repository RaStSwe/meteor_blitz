//game.cpp

#include "game.hpp"
#include "Player (ship + bullet)/player.hpp"
#include "Enemy/enemy.hpp"

namespace meteor_blitz
{
	//--Extern instances to be used in game.cpp--
	meteor_blitz::Player player;
	meteor_blitz::ProjectileSystem projectilesystem;
	meteor_blitz::Enemy enemy;
	meteor_blitz::EnemySystem enemysystem;

	void Game::initialize() {
		InitWindow(1280, 1280, "Meteor Blitz");
		player.initialize();
		projectilesystem.initialize();
		enemysystem.initialize();
	}

	bool Game::is_running() const {
		return running;
	}

	void Game::update(float dt) {
		// use current frame time for updates
		dt = (float)GetFrameTime();
		player.update(dt);
		projectilesystem.update(dt);
		enemysystem.update(dt);

	}

	void Game::render(){
		BeginDrawing();
			ClearBackground(BLACK);
			DrawText("-Meteor Blitz By RaStSwe-", 10, 1230, 20, RAYWHITE);
			DrawText("D - Rotate Left | F - Rotate Right | J - Shoot | K - Thrust", 10, 1255, 20, RAYWHITE);
			player.render();
			projectilesystem.render();
			enemysystem.render();
			
		EndDrawing();
	}



} // end of namespace meteor_blitz