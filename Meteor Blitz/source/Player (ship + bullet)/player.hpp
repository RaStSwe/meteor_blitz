//Player header with class that include movement and rendering to be used in game.cpp

#pragma once

#include "../common.hpp"

namespace meteor_blitz
{
	struct Player {
		//screen size
		Rectangle screen_size = {};

		//------ Ship stats --------
		static constexpr float player_speed = 200.0f; //player forward speed
		static constexpr float player_acceleration = 250.0f; //Player acceleration
		static constexpr float rotation_per_sec = 300.0f; // player rotation speed
		static constexpr float player_friction = 1.20f; //player friction to not always move
		Vector2 player_velocity = { 0,0 };


		//----- Player texture stuff ----------
		const char* player_sprite_path_nothrust = "assets/ship_sprite_nothrust.png";
		const char* player_sprite_path_thrust = "assets/ship_sprite_thrust.png";
		static constexpr float player_spite_size = 64.0f;
		float player_rotation = 270.0f;

		Texture2D player_texture_nothrust = {};
		Texture2D player_texture_thrust = {};
		Rectangle player_sprite_source = {};
		Vector2 player_sprite_origin = {};
		Rectangle player_sprite_destination = {};

		//Player functions
		void hyperspace();

		//---Player initialize----
		void initialize();

		//---Update & render ------
		void update(float dt);
		void player_warping();
		void render() const;



	};

	struct Projectile {
		bool active = false;
		Vector2 position = { 0,0 };
		Vector2 velocity = { 0,0 };
		Rectangle sprite_destination = { 0,0,0,0 };
		float rotation = 0.0f;
		float lifetime = 0.0f;
	};

	struct ProjectileSystem {
		//screen size
		Rectangle screen_size = {};

		//---Initialize projectile----
		void initialize();

		//----- Projectile texture stuff ----------
		const char* projectile_sprite_path = "assets/bullet.png";
		static constexpr float projectile_sprite_sizeX = 4.0f;
		static constexpr float projectile_sprite_sizeY = 16.0f;

		Texture2D projectile_texture = {};
		Rectangle projectile_sprite_source = {};
		Rectangle projectile_sprite_destination = {};
		Vector2 projectile_sprite_origin = {};

		//---Projectile stats----
		float projectile_speed = 800.0f; //pixels pre second
		float fire_rate = 20.0f; //bullets per second
		float fire_cooldown = 0.2f; //how long time between shots
		float projectile_lifetime = 1.1f; //how long a projectile stays in game
		std::size_t max_projectiles = 20; //how many projectiles allowed on screen at once

		std::vector<Projectile> projectile_pool = {};
		std::vector<Projectile> projectiles = {};

		void spawn_projectile(const Player& player);
		void projectile_warping(Projectile& proj) const;



		//---Update & render ------
		void update(float dt);
		void render() const;

	};

	// Extern globals defined in game.cpp
	extern Player player;
	extern ProjectileSystem projectilesystem;

}
