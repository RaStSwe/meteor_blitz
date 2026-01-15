//Player header with class that include movement and rendering to be used in game.cpp

#pragma once

#include "../common.hpp"

namespace meteor_blitz
{
	struct EnemySystem; //to make collsion check possible

    struct Player {
        //--- screen size ---
        Rectangle screen_size = {};

        //--- sounds ---
        const char* player_explosion_sound_path = "assets/sound/player_explosion.wav";
		const char* player_hyperspace_sound_path = "assets/sound/hyperspace_sound.wav";
		const char* player_shipthrust_sound_path = "assets/sound/thrust_sound.wav";
        Sound player_explosion_sound = {};
		Sound hyperspace_sound = {};
		Sound shipthrust_sound = {};


        //--- player stats ---
        int lives = 3;


        //--- Ship stats ---
        static constexpr float player_speed = 200.0f;
        static constexpr float player_acceleration = 250.0f;
        static constexpr float rotation_per_sec = 300.0f;
        static constexpr float player_friction = 1.20f;
        Vector2 player_velocity = { 0,0 };

        //--- Player texture paths ---
        const char* player_sprite_path_hp = "assets/sprites/ship_hp_sprite.png";
        const char* player_sprite_path_nothrust = "assets/sprites/ship_sprite_nothrust.png";
        const char* player_sprite_path_thrust = "assets/sprites/ship_sprite_thrust.png";

        //--- player hp ---
        Texture2D player_texture_hp = {};
        Rectangle player_sprite_hp_source = {};
        Vector2 player_sprite_hp_origin = {};
        std::vector<int> player_positionsX_hp = { 26, 62, 98 };
        static constexpr float player_sprite_size_hp = 32.0f;

        //--- player ship ---
        Texture2D player_texture_nothrust = {};
        Texture2D player_texture_thrust = {};
        Rectangle player_sprite_source = {};
        Vector2 player_sprite_origin = {};
        Rectangle player_sprite_destination = {};
        static constexpr float player_spite_size = 64.0f;
        float player_rotation = 270.0f;
        Vector2 player_position = {};

        //--- Player initialize ---
        void initialize();

        //--- Player functions ---
        void hyperspace();
        void reset();
        //void reswpawn();
        void checkCollisions(EnemySystem& enemies);

        //--- Update & render ---
        void update(float dt);
        void player_warping();
        void render() const;
        void render_lives() const;
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
		//--- screen size ---
		Rectangle screen_size = {};

		//--- Initialize projectile ---
		void initialize();

		//--- sounds ---
		const char* shoot_sound_path = "assets/sound/shoot_sound.wav";
		Sound shoot_sound = {};

		//--- Projectile texture stuff ---
		const char* projectile_sprite_path = "assets/sprites/bullet.png";
		static constexpr float projectile_sprite_sizeX = 4.0f;
		static constexpr float projectile_sprite_sizeY = 16.0f;

		Texture2D projectile_texture = {};
		Rectangle projectile_sprite_source = {};
		Rectangle projectile_sprite_destination = {};
		Vector2 projectile_sprite_origin = {};

		//--- Projectile stats ---
		float projectile_speed = 800.0f; //pixels pre second
		float fire_rate = 20.0f; //bullets per second
		float fire_cooldown = 0.2f; //how long time between shots
		float projectile_lifetime = 1.1f; //how long a projectile stays in game
		std::size_t max_projectiles = 20; //how many projectiles allowed on screen at once

		std::vector<Projectile> projectile_pool = {};
		std::vector<Projectile> projectiles = {};
		
		//--- Projectile functions ---
		void spawn_projectile(const Player& player);
		void projectile_warping(Projectile& proj) const;

		//--- Update & render ---
		void update(float deltaTime, const Player& player);
		void render() const;

	};

}// end of namespace meteor_blitz
