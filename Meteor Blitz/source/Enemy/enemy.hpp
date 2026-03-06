// Enemy header file 

#pragma once

#include "../common.hpp"
#include "../Player (ship + bullet)/player.hpp"

namespace meteor_blitz
{	
	enum class EnemyType {
		Small = 0,
		Medium = 1,
		Large = 2,
		Seeker = 3
	};

	struct Enemy {
		bool active = false;
		EnemyType type = EnemyType::Small;
		Vector2 position = { 0,0 };
		Vector2 velocity = { 0,0 };
		Rectangle sprite_destination = { 0,0,0,0 };
		float rotation = 0.0f;
	};

	struct EnemySystem {
		//--- Screen size ---
		Rectangle screen_size = {};

		//--- sounds --- 
		const char* enemy_explosion_sound_path = "assets/sound/enemy_explosion.wav";

		Sound enemy_explosion_sound = {};

		//--- Initialize enemy ---
		void initialize();

		//--- Enemy texture stuff ---
			//--- small enemy ---
		const char* enemy_small_sprite_path = "assets/sprites/enemy_sprite_small.png";
		static constexpr float enemy_sprite_small_sizeX = 64.0f;
		static constexpr float enemy_sprite_small_sizeY = 64.0f;
		Texture2D enemy_small_texture = {};
		Rectangle enemy_small_sprite_source = {};
		Vector2 enemy_small_sprite_origin = {};
		Rectangle enemy_small_sprite_destination = {};
			//--- medium enemy ---
		const char* enemy_medium_sprite_path = "assets/sprites/enemy_sprite_medium.png";
		static constexpr float enemy_sprite_medium_sizeX = 96.0f;
		static constexpr float enemy_sprite_medium_sizeY = 96.0f;
		Texture2D enemy_medium_texture = {};
		Rectangle enemy_medium_sprite_source = {};
		Vector2 enemy_medium_sprite_origin = {};
		Rectangle enemy_medium_sprite_destination = {};
			//--- large enemy ---
		const char* enemy_large_sprite_path = "assets/sprites/enemy_sprite_large.png";
		static constexpr float enemy_sprite_large_sizeX = 128.0f;
		static constexpr float enemy_sprite_large_sizeY = 128.0f;
		Texture2D enemy_large_texture = {};
		Rectangle enemy_large_sprite_source = {};
		Vector2 enemy_large_sprite_origin = {};
		Rectangle enemy_large_sprite_destination = {};
		
		//--- Enemty stats ---
		float enemy_speed = 100.0f; //enemy speed
		static constexpr int seeker_wave_start = 5; //new seeker enemy type starts spawning 
		static constexpr int seeker_spawn_chance = 30; //30% for each basic enemy to be a seeker

		//--- ememy functions ---
		void reset();
		void spawn_enemyWave();
		void enemy_warping(Enemy& enemy) const;
		void checkCollisions(ProjectileSystem& projectile);
		

		//--- Wave stats & stuff ---
		std::vector<Enemy> enemy_pool;
		int enemies_per_wave = 1; 
		const char* waveInfo = "Wave: ";
		int waveCount = 1;

		//--- Wave funtions ---
		void newWave();
		void restart_wave();

		//--- Update & render ---
		void update(float deltaTime, const Vector2& playerPos);
		void render() const;


	};


} // end of namespace meteor_blitz