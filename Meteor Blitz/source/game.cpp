//game.cpp

#include "game.hpp"
#include "Player (ship + bullet)/player.hpp"
#include "Enemy/enemy.hpp"

namespace meteor_blitz
{

	Player player;
	ProjectileSystem projectilesystem;
	Enemy enemy;
	EnemySystem enemysystem;

	// define the static game_state
	GameState Game::game_state = GameState::Start;

	void Game::set_game_state(GameState state) {
		game_state = state;
	}

	GameState Game::get_game_state() {
		return game_state;
	}

	void Game::initialize() {
		//--- Initialize window & audio ---
		InitWindow(1280, 1280, "Meteor Blitz");
		InitAudioDevice();
		SetMasterVolume(1.0f);
		HideCursor();
		SetExitKey(KEY_NULL);
		player.initialize();
		projectilesystem.initialize();
		enemysystem.initialize();
		game_state = GameState::Start;

		//--- load sounds ---
		put_in_coin_sound = LoadSound(put_in_coin_sound_path);

	}

	bool Game::is_running() const {
		return running;
	}

	void Game::reset_game() {
		//--- Reset player position/state and restore lives & HUD ---
		player.reset();
		player.lives = 3;
		//--- enemy system reset puts wave back to 1 and spawns initial enemies ---
		enemysystem.reset();
	}

	void Game::update(float deltaTime) {
		//--- Start screen update ---
		if (game_state == GameState::Start) {
			if (IsKeyPressed(KEY_ENTER)) {
				PlaySound(put_in_coin_sound);
				coin += 1;
				WaitTime(0.2f);
			}
			if(IsKeyPressed(KEY_SPACE) && coin > 0) {
				coin -= 1;
				WaitTime(0.2f); 
				game_state = GameState::Play;
			}
			else if (IsKeyPressed(KEY_Q)){
				EndDrawing();
				UnloadSound(player.player_explosion_sound);
				UnloadSound(projectilesystem.shoot_sound);
				UnloadSound(enemysystem.enemy_explosion_sound);
				CloseAudioDevice();
				WindowShouldClose();
			}
		}
		//--- Gameplay update ---
		else if(game_state == GameState::Play){
			//--- player update & collision check ---
			player.update(deltaTime);
			player.checkCollisions(enemysystem);
			
			//--- Projectile update ---
			projectilesystem.update(deltaTime, player);

			//--- enemy update & collision check ---
			Vector2 playerPos = { 
				player.player_sprite_destination.x + player.player_sprite_origin.x, 
				player.player_sprite_destination.y + player.player_sprite_origin.y }; 
			enemysystem.update(deltaTime, playerPos);
			enemysystem.checkCollisions(projectilesystem);
		}		
		//--- Game over update ---
		else if (game_state == GameState::GAME_OVER) {
			if (IsKeyPressed(KEY_ENTER)) {
				PlaySound(put_in_coin_sound);
				coin += 1;
				WaitTime(0.2f);
			}
			if(IsKeyPressed(KEY_SPACE) && coin > 0 ) {
				coin -= 1;
				WaitTime(0.2f); // give some time to reset
				reset_game();
				game_state = GameState::Play;
			}
			else if (IsKeyPressed(KEY_Q)){
				EndDrawing();
				UnloadSound(player.player_explosion_sound);
				UnloadSound(projectilesystem.shoot_sound);
				UnloadSound(enemysystem.enemy_explosion_sound);
				CloseAudioDevice();
				WindowShouldClose();
				

			}
		}
	}

	int Game::center_textX(const char* text, int fontSize) {
        int tempWidth = MeasureText(text, fontSize);
        int textCenteredX = ((GetScreenWidth() / 2) - (tempWidth / 2));
        return textCenteredX;
	}

	void Game::render(){
		BeginDrawing();
			ClearBackground(BLACK);
			
			if(game_state == GameState::Start){
				DrawText("-Meteor Blitz-", (center_textX("-Meteor Blitz-", 100)), 300, 100, RAYWHITE);
				DrawText("-By RaStSwe-", (center_textX("-By RaStSwe-", 50)), 410, 50, RAYWHITE);
				DrawText("-Press Space to Start-", (center_textX("-Press Space to Start-", 30)), 800, 30, RAYWHITE);
				DrawText("-or-", (center_textX("-or-", 30)), 850, 30, RAYWHITE);
				DrawText("-Q to quit-", (center_textX("-Q to quit-", 30)), 900, 30, RAYWHITE);
				DrawText("< D - Rotate Left | F - Rotate Right | J - Shoot | K - Thrust | Space - HyperSpace | enter - coin >",
					(center_textX("< D - Rotate Left | F - Rotate Right | J - Shoot | K - Thrust | Space - HyperSpace | enter - coin >", 20)), 1100, 20, RAYWHITE);
				DrawText("1 coin 1 Play", (center_textX("1 coin 1 Play", 20)), 1200, 20, RAYWHITE);
				DrawText(TextFormat("Coins - %d", coin), (center_textX(TextFormat("Coins - %d", coin), 20)), 1230, 20, RAYWHITE);
			}
			else if(game_state == GameState::Play){				
				DrawText(TextFormat("Wave: %d", enemysystem.waveCount), 10, 10, 40, RAYWHITE);
				player.render();
				player.render_lives();
 				projectilesystem.render();
 				enemysystem.render();
			}
			else if(game_state == GameState::GAME_OVER){
				DrawText("-GAME OVER-", center_textX("-GAME OVER-", 100), 300, 100, RAYWHITE);
				DrawText("-Press space to start-", center_textX("-Press space to start-", 30), 700, 30, RAYWHITE);
				DrawText("-or-", center_textX("-or-", 30), 750, 30, RAYWHITE);
				DrawText("-Q to quit-", center_textX("-Q to quit-", 30), 800, 30, RAYWHITE);
				DrawText(TextFormat("You cleared Wave: %d", enemysystem.waveCount - 1),
					(center_textX(TextFormat("You cleared Wave: %d", enemysystem.waveCount - 1), 30)), 400, 30, RAYWHITE);
				DrawText("1 coin 1 Play", (center_textX("1 coin 1 Play", 20)), 1200, 20, RAYWHITE);
				DrawText(TextFormat("Coins - %d", coin), (center_textX(TextFormat("Coins - %d", coin), 20)), 1230, 20, RAYWHITE);
			}

		EndDrawing();
	}



} // end of namespace meteor_blitz