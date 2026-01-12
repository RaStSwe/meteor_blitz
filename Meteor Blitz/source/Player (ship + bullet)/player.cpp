//player.cpp

#include "player.hpp"

namespace meteor_blitz
{
	void Player::initialize() {
		//---Get screen size----
		screen_size = { 0,0, (float)GetScreenWidth(),(float)GetScreenHeight() };

		//--Initializing textures and positions--
		player_texture_nothrust = LoadTexture(player_sprite_path_nothrust);
		player_texture_thrust = LoadTexture(player_sprite_path_thrust);
		player_sprite_source = { 0,0,player_spite_size,player_spite_size };
		player_sprite_destination = { screen_size.width / 2,screen_size.height / 2,player_sprite_source.width,player_sprite_source.height };
		player_sprite_origin = { player_sprite_source.width / 2,player_sprite_source.height / 2 };
	}

	void Player::hyperspace() {
		player_sprite_destination.x = static_cast<float>(GetRandomValue(0, (int)screen_size.width));
		player_sprite_destination.y = static_cast<float>(GetRandomValue(0, (int)screen_size.height));
	}

	void Player::player_warping() {

		//right to left
		if (player_sprite_destination.x >= screen_size.width + player_sprite_destination.width / 2)
			player_sprite_destination.x = player_sprite_destination.width / 2;
		//left to right
		if (player_sprite_destination.x <= -player_sprite_destination.width / 2)
			player_sprite_destination.x = screen_size.width;

		//top to bottom
		if (player_sprite_destination.y >= screen_size.height + player_sprite_destination.height / 2)
			player_sprite_destination.y = player_sprite_destination.height / 2;
		//bottom to top
		if (player_sprite_destination.y <= -player_sprite_destination.height / 2)
			player_sprite_destination.y = screen_size.height - player_sprite_destination.height / 2;
	}

	void Player::update(float dt) {
		//-------limits angels to only 0-360 degrees------
		if (player_rotation >= 360.0f) player_rotation -= 360.0f;
		if (player_rotation <= 0.0f)   player_rotation += 360.0f;
	
		//---Player Rotation---
		if (IsKeyDown(KEY_D)) {
			player_rotation -= dt * rotation_per_sec;
		}
		if (IsKeyDown(KEY_F)) {
			player_rotation += dt * rotation_per_sec;
		}

		//--Player forward movement--
		if (IsKeyDown(KEY_K)) {
			float rad = DEG2RAD * player_rotation;
			player_velocity.x += std::cos(rad) * player_acceleration * dt;
			player_velocity.y += std::sin(rad) * player_acceleration * dt;
		}
		else {
			player_velocity.x -= player_velocity.x * player_friction * dt;
			player_velocity.y -= player_velocity.y * player_friction * dt;
		}
		//moving the player with the velocity it got while accelarating
		player_sprite_destination.x += player_velocity.x * dt;
		player_sprite_destination.y += player_velocity.y * dt;

		if(IsKeyPressed(KEY_SPACE)){
			hyperspace();
		}

		//warping the ship to the other side of the frame if it leaves the frame
		player_warping();
	}

	void Player::render() const {
		if (IsKeyDown(KEY_K)){ 
			DrawTexturePro(player_texture_thrust, player_sprite_source, player_sprite_destination, player_sprite_origin, player_rotation, WHITE); 
		}
		else{
			DrawTexturePro(player_texture_nothrust, player_sprite_source, player_sprite_destination, player_sprite_origin, player_rotation, WHITE);
		}		
	}

	void ProjectileSystem::initialize() {
		//Screen size
		screen_size = { 0,0, (float)GetScreenWidth(),(float)GetScreenHeight() };

		//----Initializing textures----
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

	void ProjectileSystem::spawn_projectile(const Player& player) {

		for (auto& proj : projectile_pool) {
			if (!proj.active) {
				proj.active = true;
				float rad = DEG2RAD * player.player_rotation;
				float offset = player.player_sprite_destination.height / 2;
				proj.position.x = player.player_sprite_destination.x + std::cos(rad) * offset; // - (projectile_sprite_sizeX / 2
				proj.position.y = player.player_sprite_destination.y + std::sin(rad) * offset; //- (projectile_sprite_sizeY / 2

				proj.sprite_destination.x = proj.position.x;
				proj.sprite_destination.y = proj.position.y;
				proj.velocity.x = std::cos(rad) * projectile_speed;
				proj.velocity.y = std::sin(rad) * projectile_speed;
				proj.rotation = player.player_rotation + 270;
				proj.lifetime = projectile_lifetime;

				// Spawn only one projectile per press
				break;
			}
		}
	}

	void ProjectileSystem::projectile_warping(Projectile& proj) const{
		//right to left
		if (proj.position.x >= screen_size.width + proj.sprite_destination.width / 2)
			proj.position.x = proj.sprite_destination.width / 2;
		//left to right
		if (proj.position.x <= -proj.sprite_destination.width / 2)
			proj.position.x = screen_size.width;

		//top to bottom
		if (proj.position.y >= screen_size.height + proj.sprite_destination.height / 2)
			proj.position.y = proj.sprite_destination.height / 2;
		//bottom to top
		if (proj.position.y <= -proj.sprite_destination.height / 2)
			proj.position.y = screen_size.height - proj.sprite_destination.height / 2;
	}

	void ProjectileSystem::update(float dt) {

		// fire handling (spawn on key press)
		if (IsKeyPressed(KEY_J)){
			spawn_projectile(player);
		}

		// Always update all projectiles (move, lifetime, warping)
		for (auto& proj : projectile_pool) {
			if (!proj.active) continue;

			// updating positions for projectiles
			proj.position.x += proj.velocity.x * dt;
			proj.position.y += proj.velocity.y * dt;
			proj.sprite_destination.x = proj.position.x;
			proj.sprite_destination.y = proj.position.y;

			// lifetime
			proj.lifetime -= dt;
			if (proj.lifetime <= 0.0f) {
				proj.active = false;
				continue;
			}

			projectile_warping(proj);
		}
	}



	void ProjectileSystem::render() const {
		// Draw active projectiles and a wireframe to inspect actual rectangles
		int activeCount = 0;
		for (const auto& proj : projectile_pool) {
			if (!proj.active) continue;

			// Draw projectile texture normally
			DrawTexturePro(projectile_texture, projectile_sprite_source, proj.sprite_destination, projectile_sprite_origin, proj.rotation, WHITE);
		}
	}
}//end of namespace meteor_blitz
