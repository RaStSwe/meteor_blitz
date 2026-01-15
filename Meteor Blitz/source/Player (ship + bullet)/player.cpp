//player.cpp

#include "player.hpp"
#include "../Enemy/enemy.hpp"
#include "../game.hpp"

namespace meteor_blitz
{
	void Player::initialize() {
		//---Get screen size----
		screen_size = { 0,0, (float)GetScreenWidth(),(float)GetScreenHeight() };

		//--- initialize player lives ---
		lives = 3;

		//--- loading sounds ---
		player_explosion_sound = LoadSound(player_explosion_sound_path);
		hyperspace_sound = LoadSound(player_hyperspace_sound_path);
		shipthrust_sound = LoadSound(player_shipthrust_sound_path);

		//--- Initializing textures and positions for ship ---
		player_texture_nothrust = LoadTexture(player_sprite_path_nothrust);
		player_texture_thrust = LoadTexture(player_sprite_path_thrust);
		player_sprite_source = { 0,0,player_spite_size,player_spite_size };
		player_sprite_destination = { screen_size.width / 2,screen_size.height / 2,player_sprite_source.width,player_sprite_source.height };
		player_sprite_origin = { player_sprite_source.width / 2,player_sprite_source.height / 2 };

		//--- Initializing textures for hp sprites ---
		player_texture_hp = LoadTexture(player_sprite_path_hp);
		player_sprite_hp_source = { 0,0,player_sprite_size_hp,player_sprite_size_hp };
		player_sprite_hp_origin = { player_sprite_size_hp / 2, player_sprite_size_hp / 2 };
	}

	void Player::hyperspace() {
		PlaySound(hyperspace_sound);
		player_sprite_destination.x = static_cast<float>(GetRandomValue(0, (int)screen_size.width));
		player_sprite_destination.y = static_cast<float>(GetRandomValue(0, (int)screen_size.height));
	}

	void Player::player_warping() {

		//--- right to left ---
		if (player_sprite_destination.x >= screen_size.width + player_sprite_destination.width / 2)
			player_sprite_destination.x = player_sprite_destination.width / 2;
		//--- left to right ---
		if (player_sprite_destination.x <= -player_sprite_destination.width / 2)
			player_sprite_destination.x = screen_size.width;

		//--- top to bottom ---
		if (player_sprite_destination.y >= screen_size.height + player_sprite_destination.height / 2)
			player_sprite_destination.y = player_sprite_destination.height / 2;
		//--- bottom to top ---
		if (player_sprite_destination.y <= -player_sprite_destination.height / 2)
			player_sprite_destination.y = screen_size.height - player_sprite_destination.height / 2;
	}

	void Player::reset() {
		// Reset position/velocity/rotation; do not change lives here (Game::reset_game restores lives)
		player_sprite_destination = { screen_size.width / 2,screen_size.height / 2,player_sprite_source.width,player_sprite_source.height };
		player_velocity = { 0,0 };
		player_rotation = 270.0f;
	}

	// Collision handling: decrement life on hit, reset player if lives remain,
	// otherwise set game over. Keeps original behaviour and comments.
	void Player::checkCollisions(EnemySystem& enemies) {
		// Convert player dest (centered) to top-left rect for collision checks
		Rectangle playerRect = player_sprite_destination;
		playerRect.x -= player_sprite_origin.x;
		playerRect.y -= player_sprite_origin.y;

		for (auto& enemyInstance : enemies.enemy_pool) {
			if (!enemyInstance.active) continue;

			Rectangle enemyRect = enemyInstance.sprite_destination;
			enemyRect.x -= enemies.enemy_sprite_origin.x;
			enemyRect.y -= enemies.enemy_sprite_origin.y;

			if (CheckCollisionRecs(playerRect, enemyRect)) {
				// play hit sound if available
				PlaySound(player_explosion_sound);

				if (lives > 0) {
					//--- Remove one life ---
					--lives;
					enemies.restart_wave();
					
					if (lives > 0) {
						//--- Reset ship ---
						reset();						
					}
					else {
						//--- no lives left ---
						Game::set_game_state(GameState::GAME_OVER);
					}
				}
				else {
					// defensive: ensure game over
					Game::set_game_state(GameState::GAME_OVER);
				}

				break;
			}
		}
	}

	void Player::update(float deltaTime) {
		//--- limits angels to only 0-360 degrees ---
		if (player_rotation >= 360.0f) player_rotation -= 360.0f;
		if (player_rotation <= 0.0f)   player_rotation += 360.0f;
	 
		//--- Player Rotation ---
		if (IsKeyDown(KEY_D)) {
			player_rotation -= deltaTime * rotation_per_sec;
		}
		if (IsKeyDown(KEY_F)) {
			player_rotation += deltaTime * rotation_per_sec;
		}

		//--- Player forward movement + sound ---
		if(IsKeyReleased(KEY_K) || IsKeyPressed(KEY_K)) PlaySound(shipthrust_sound);
		if (IsKeyDown(KEY_K)) {
			float rad = DEG2RAD * player_rotation;
			player_velocity.x += std::cos(rad) * player_acceleration * deltaTime;
			player_velocity.y += std::sin(rad) * player_acceleration * deltaTime;
		}
		else {
			player_velocity.x -= player_velocity.x * player_friction * deltaTime;
			player_velocity.y -= player_velocity.y * player_friction * deltaTime;
		}
		//--- moving the player with the velocity it got while accelarating ---
		player_sprite_destination.x += player_velocity.x * deltaTime;
		player_sprite_destination.y += player_velocity.y * deltaTime;

		if(IsKeyPressed(KEY_SPACE)){
			hyperspace();
		}

		//--- warping the ship to the other side of the frame if it leaves the frame ---
		player_warping();
	}

	void Player::render_lives() const
	{
		// Render player lives as ship icons at the top left of the screen
		for (int i = 0; i < lives; ++i){
			DrawTexturePro(
				player_texture_hp,
				player_sprite_hp_source,
				Rectangle{
					static_cast<float>(player_positionsX_hp[i]),
					70.0f,
					player_sprite_size_hp,
					player_sprite_size_hp
				},
				player_sprite_hp_origin,
				0.0f,
				WHITE
			);
			
		}
	}


	void Player::render() const {
		//--- Draw ship ---
		if (IsKeyDown(KEY_K)) {
			DrawTexturePro(player_texture_thrust, player_sprite_source, player_sprite_destination, player_sprite_origin, player_rotation, WHITE);
		}
		else {
			DrawTexturePro(player_texture_nothrust, player_sprite_source, player_sprite_destination, player_sprite_origin, player_rotation, WHITE);
		}
	}

	void ProjectileSystem::initialize() {
		//Screen size
		screen_size = { 0,0, (float)GetScreenWidth(),(float)GetScreenHeight() };

		//--- loading sounds ---
		shoot_sound = LoadSound(shoot_sound_path);
		if(!IsSoundValid(shoot_sound)){
			TraceLog(LOG_WARNING, "Failed to load shoot sound from path: %s", shoot_sound_path);
		}

		//----Initializing projectile textures----
		projectile_texture = LoadTexture(projectile_sprite_path);
		projectile_sprite_source = { 0,0,projectile_sprite_sizeX, projectile_sprite_sizeY };
		projectile_sprite_origin = { projectile_sprite_sizeX / 2, projectile_sprite_sizeY / 2 };
		projectile_sprite_destination = { 0,0,projectile_sprite_sizeX, projectile_sprite_sizeY };

		//---Projectile pool setup---
		projectile_pool.clear();
		projectile_pool.resize(max_projectiles);
		std::for_each(projectile_pool.begin(), projectile_pool.end(), [&](Projectile& proj) {
			proj.active = false;
			proj.sprite_destination.width = projectile_sprite_sizeX;
			proj.sprite_destination.height = projectile_sprite_sizeY;
			});
	}

	void ProjectileSystem::spawn_projectile(const Player& playerInstance) {

		for (auto& proj : projectile_pool) {
			if (!proj.active) {
				proj.active = true;
				float rad = DEG2RAD * playerInstance.player_rotation;
				float offset = playerInstance.player_sprite_destination.height / 2;
				proj.position.x = playerInstance.player_sprite_destination.x + std::cos(rad) * offset;
				proj.position.y = playerInstance.player_sprite_destination.y + std::sin(rad) * offset;

				proj.sprite_destination.x = proj.position.x;
				proj.sprite_destination.y = proj.position.y;
				proj.velocity.x = std::cos(rad) * projectile_speed;
				proj.velocity.y = std::sin(rad) * projectile_speed;
				proj.rotation = playerInstance.player_rotation + 270;
				proj.lifetime = projectile_lifetime;

				//--- spawn 1 projectile--- 
				break;
			}
		}
	}

	void ProjectileSystem::projectile_warping(Projectile& proj) const{
		//--- right to left ---
		if (proj.position.x >= screen_size.width + proj.sprite_destination.width / 2)
			proj.position.x = proj.sprite_destination.width / 2;
		//--- left to right ---
		if (proj.position.x <= -proj.sprite_destination.width / 2)
			proj.position.x = screen_size.width;

		//--- top to bottom ---
		if (proj.position.y >= screen_size.height + proj.sprite_destination.height / 2)
			proj.position.y = proj.sprite_destination.height / 2;
		//--- bottom to top ---
		if (proj.position.y <= -proj.sprite_destination.height / 2)
			proj.position.y = screen_size.height - proj.sprite_destination.height / 2;
	}

	void ProjectileSystem::update(float deltaTime, const Player& player) {

		//--- shooting projectiles ---
		if (IsKeyPressed(KEY_J)){
			PlaySound(shoot_sound);
			spawn_projectile(player);
		}
		for (auto& proj : projectile_pool) {
			if (!proj.active) continue;

			//--- updating positions for projectiles ---
			proj.position.x += proj.velocity.x * deltaTime;
			proj.position.y += proj.velocity.y * deltaTime;
			proj.sprite_destination.x = proj.position.x;
			proj.sprite_destination.y = proj.position.y;

			//--- lifetime calcs ---
			proj.lifetime -= deltaTime;
			if (proj.lifetime <= 0.0f) {
				proj.active = false;
				continue;
			}

			projectile_warping(proj);
		}
	}



	void ProjectileSystem::render() const {
		//--- Render all active projectiles ---
		for (const auto& proj : projectile_pool) {
			if (!proj.active) continue;

			DrawTexturePro(projectile_texture, projectile_sprite_source, proj.sprite_destination, projectile_sprite_origin, proj.rotation, WHITE);

		}
	}
}//end of namespace meteor_blitz
