// Enemy header file 

#pragma once

#include "../common.hpp"
#include "../Player (ship + bullet)/player.hpp"

namespace meteor_blitz
{	
	enum class EnemyType {
		Basic = 0,
		Seeker = 1
	};

	struct Enemy {
		bool active = false;
		EnemyType type = EnemyType::Basic;
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
		const char* enemy_sprite_path = "assets/sprites/enemy_sprite.png";
		static constexpr float enemy_sprite_sizeX = 64.0f;
		static constexpr float enemy_sprite_sizeY = 64.0f;

		Texture2D enemy_texture = {};
		Rectangle enemy_sprite_source = {};
		Vector2 enemy_sprite_origin = {};
		Rectangle enemy_sprite_destination = {};

		//--- Enemty stats ---
		float enemy_speed = 100.0f; //enemy speed
		static constexpr int seeker_wave_start = 5; //new seeker enemy type starts spawning 
		static constexpr int seeker_spawn_chance = 30; //30% for each basic enemy to be a seeker

		//--- ememy functions ---
		void reset();
		void spawn_enemyWave();
		void enemy_warping(Enemy& enemy) const;
		void checkCollisions(ProjectileSystem& projectile);
		// Restart the current wave if player got hit
		void restart_wave();

		//--- Wave stats & stuff ---
		std::vector<Enemy> enemy_pool;
		int enemies_per_wave = 1; 
		const char* waveInfo = "Wave: ";
		int waveCount = 1;

		//--- Wave funtions ---
		void newWave();

		//--- Update & render ---
		void update(float deltaTime, const Vector2& playerPos);
		void render() const;


	};


} // end of namespace meteor_blitz