//Enemy cpp file 

#include "enemy.hpp"

namespace meteor_blitz
{
	void EnemySystem::initialize(){
		//---Get screen size---
		screen_size = { 0,0, (float)GetScreenWidth(),(float)GetScreenHeight() };

		//---Initializing textures and the positions---
		enemy_texture = LoadTexture(enemy_sprite_path);
		enemy_sprite_source = { 0,0,enemy_sprite_sizeX,enemy_sprite_sizeY };
		enemy_sprite_origin = { enemy_sprite_source.width / 2,enemy_sprite_source.height / 2 };
		enemy_sprite_destination = { screen_size.width / 2,screen_size.height / 2,enemy_sprite_source.width,enemy_sprite_source.height };
	}

	void EnemySystem::spawn_enemyWave() {
		
	}

	void EnemySystem::enemy_warping(Enemy& enemy) const{
		//right to left
		if (enemy.position.x >= screen_size.width + enemy.sprite_destination.width / 2)
			enemy.position.x = enemy.sprite_destination.width / 2;
		//left to right
		if (enemy.position.x <= -enemy.sprite_destination.width / 2)
			enemy.position.x = screen_size.width;
		//top to bottom
		if (enemy.position.y >= screen_size.height + enemy.sprite_destination.height / 2)
			enemy.position.y = enemy.sprite_destination.height / 2;
		//bottom to top
		if (enemy.position.y <= -enemy.sprite_destination.height / 2)
			enemy.position.y = screen_size.height - enemy.sprite_destination.height / 2;
	}

	void EnemySystem::update(float dt){
	
	}

	void EnemySystem::render() const{
		
	}

} // end of namespace meteor_blitz