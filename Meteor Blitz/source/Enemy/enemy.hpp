// Enemy header file 

#pragma once

#include "../common.hpp"

namespace meteor_blitz
{

	struct Enemy {
		bool active = false;
		Vector2 position = { 0,0 };
		Vector2 velocity = { 0,0 };
		Rectangle sprite_destination = { 0,0,0,0 };
		float rotation = 0.0f;
	};

	struct EnemySystem {
		//---Screen size---
		Rectangle screen_size = {};

		//---Initialize enemy---
		void initialize();

		//---Enemy texture stuff---
		const char* enemy_sprite_path = "assets/enemy_sprite_1.png";
		static constexpr float enemy_sprite_sizeX = 32.0f;
		static constexpr float enemy_sprite_sizeY = 32.0f;

		Texture2D enemy_texture = {};
		Rectangle enemy_sprite_source = {};
		Vector2 enemy_sprite_origin = {};
		Rectangle enemy_sprite_destination = {};

		//---Enemty stats---
		float enemy_speed = 50.0f; //enemy speed

		void spawn_enemyWave();
		void enemy_warping(Enemy& enemy) const;

		//---Update & render ---
		void update(float dt);
		void render() const;


	};
	//--Extern instances to be used in game.cpp--
	extern Enemy enemy;
	extern EnemySystem enemysystem;

} // end of namespace meteor_blitz