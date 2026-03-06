//Enemy cpp file 

#include "enemy.hpp"

namespace meteor_blitz
{
	void EnemySystem::initialize(){
		//--- Get screen size ---
		screen_size = { 0,0, (float)GetScreenWidth(),(float)GetScreenHeight() };

		//--- loading sounds ---
		enemy_explosion_sound = LoadSound(enemy_explosion_sound_path);

		//--- Initializing textures and the positions ---
		enemy_small_texture = LoadTexture(enemy_small_sprite_path);
		enemy_small_sprite_source = { 0,0,enemy_sprite_small_sizeX,enemy_sprite_small_sizeY };
		enemy_small_sprite_origin = { enemy_small_sprite_source.width / 2,enemy_small_sprite_source.height / 2 };
		enemy_small_sprite_destination = { screen_size.width / 2,screen_size.height / 2,enemy_small_sprite_source.width,enemy_small_sprite_source.height };

		//--- Enemy pool setup ---
		enemy_pool.clear();
		enemy_pool.resize(enemies_per_wave);
		std::for_each(enemy_pool.begin(), enemy_pool.end(), [&](Enemy& enemyInstance) {
			enemyInstance.active = false;
			enemyInstance.sprite_destination.width = enemy_small_sprite_source.width;
			enemyInstance.sprite_destination.height = enemy_small_sprite_source.height;
			enemyInstance.position = { 0, 0 };
			enemyInstance.velocity = { 0, 0 };
			});

		//--- Spawn the first wave ---
		spawn_enemyWave();
	}

	void EnemySystem::spawn_enemyWave() {

		for (auto& enemyInstance : enemy_pool) {
			if (!enemyInstance.active) {
				enemyInstance.active = true;

				//--- Determine enemy type ---
				if(waveCount >= seeker_wave_start && GetRandomValue(1, 100) <= seeker_spawn_chance){
					enemyInstance.type = EnemyType::Seeker;
				}
				else {
					enemyInstance.type = EnemyType::Small;
				}

				//--- Spawn position (top, bottom, left, or right) ---
				int side = GetRandomValue(0, 3);

				// top
				if (side == 0) {
					int spawnX = GetRandomValue(0, (int)screen_size.width);
					enemyInstance.position.x = static_cast<float>(spawnX);
					enemyInstance.position.y = -enemyInstance.sprite_destination.height - 10.0f;
				}
				// bottom
				else if (side == 1) {
					int spawnX = GetRandomValue(0, (int)screen_size.width);
					enemyInstance.position.x = static_cast<float>(spawnX);
					enemyInstance.position.y = screen_size.height + enemyInstance.sprite_destination.height + 10.0f;
				}
				// left
				else if (side == 2) {
					int spawnY = GetRandomValue(0, (int)screen_size.height);
					enemyInstance.position.x = -enemyInstance.sprite_destination.width - 10.0f;
					enemyInstance.position.y = static_cast<float>(spawnY);
				}
				// right
				else { // aka side == 3
					int spawnY = GetRandomValue(0, (int)screen_size.height);
					enemyInstance.position.x = screen_size.width + enemyInstance.sprite_destination.width + 10.0f;
					enemyInstance.position.y = static_cast<float>(spawnY);
				}

				if (enemyInstance.type == EnemyType::Seeker) {
					// initial velocity toward screen center (will home to player in update)
					Vector2 center{ screen_size.width * 0.5f, screen_size.height * 0.5f };
					Vector2 dir{ center.x - enemyInstance.position.x, center.y - enemyInstance.position.y };
					float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
					if (len > 0.0001f) {
						dir.x /= len;
						dir.y /= len;
						enemyInstance.velocity.x = dir.x * (enemy_speed);
						enemyInstance.velocity.y = dir.y * (enemy_speed);
					}
					else {
						float angle = DEG2RAD * static_cast<float>(GetRandomValue(0, 359));
						enemyInstance.velocity.x = std::cos(angle) * enemy_speed;
						enemyInstance.velocity.y = std::sin(angle) * enemy_speed;
					}
				}else{ //--- randomize direction with thine volocity ---
				float angleInDeg = static_cast<float>(GetRandomValue(0, 359));
				float angleInRad = DEG2RAD * angleInDeg;
				enemyInstance.velocity.x = std::cos(angleInRad) * enemy_speed;
				enemyInstance.velocity.y = std::sin(angleInRad) * enemy_speed;
				}

				//--- sync position to destination ---
				enemyInstance.sprite_destination.x = enemyInstance.position.x;
				enemyInstance.sprite_destination.y = enemyInstance.position.y;

				enemyInstance.rotation = static_cast<float>(GetRandomValue(0, 359));
			}
		}
	}

	void EnemySystem::enemy_warping(Enemy& enemy) const{
		//--- right to left ---
		if (enemy.position.x >= screen_size.width + enemy.sprite_destination.width / 2)
			enemy.position.x = enemy.sprite_destination.width / 2;
		//--- left to right ---
		if (enemy.position.x <= -enemy.sprite_destination.width / 2)
			enemy.position.x = screen_size.width;
		//---top to bottom ---
		if (enemy.position.y >= screen_size.height + enemy.sprite_destination.height / 2)
			enemy.position.y = enemy.sprite_destination.height / 2;
		//--- bottom to top --- 
		if (enemy.position.y <= -enemy.sprite_destination.height / 2)
			enemy.position.y = screen_size.height - enemy.sprite_destination.height / 2;
	}

	void EnemySystem::reset() {
		//-- - Clear and make a new enemy pool ---
		enemy_pool.clear();
		enemies_per_wave = 1;
		enemy_pool.resize(enemies_per_wave);
		for (auto& enemyInstance : enemy_pool) {
			enemyInstance.active = false;
			enemyInstance.sprite_destination.width = enemy_small_sprite_source.width;
			enemyInstance.sprite_destination.height = enemy_small_sprite_source.height;
			enemyInstance.position = { 0, 0 };
			enemyInstance.velocity = { 0, 0 };
		}
		enemy_speed = 100.0f;
		waveCount = 1;
		
		//--- spawn in the first wave ---
		spawn_enemyWave();
	}

	void EnemySystem::restart_wave() {
		// Ensure pool size matches current enemies_per_wave
		if (enemy_pool.size() != static_cast<std::size_t>(enemies_per_wave)) {
			std::size_t oldSize = enemy_pool.size();
			enemy_pool.resize(enemies_per_wave);
			for (std::size_t i = oldSize; i < enemy_pool.size(); ++i) {
				enemy_pool[i].active = false;
				enemy_pool[i].sprite_destination.width = enemy_small_sprite_source.width;
				enemy_pool[i].sprite_destination.height = enemy_small_sprite_source.height;
				enemy_pool[i].position = { 0, 0 };
				enemy_pool[i].velocity = { 0, 0 };
			}
		}

		// Deactivate and clear current enemies so spawn_enemyWave can fill them again
		for (auto& enemies : enemy_pool) {
			enemies.active = false;
			enemies.position = { 0, 0 };
			enemies.velocity = { 0, 0 };
			enemies.sprite_destination.x = 0;
			enemies.sprite_destination.y = 0;
			enemies.rotation = 0.0f;
		}

		// Spawn enemies for the current wave (uses enemies_per_wave and waveCount)
		spawn_enemyWave();
	}

	void EnemySystem::update(float deltaTime, const Vector2& playerPos) {
		//--- Update all enemies in the pool ---
		for (auto& enemyInstance : enemy_pool) {
			if (!enemyInstance.active) continue;

			//--- Seeker enemy seeking behavior ---
			if (enemyInstance.type == EnemyType::Seeker) {
				Vector2 dir{ playerPos.x - enemyInstance.position.x, playerPos.y - enemyInstance.position.y };
				float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
				if (len > 0.0001f) {
					dir.x /= len;
					dir.y /= len;
					float speed = enemy_speed;
					enemyInstance.velocity.x = dir.x * speed;
					enemyInstance.velocity.y = dir.y * speed;
				}
			}

			//--- Move by velocity ---
			enemyInstance.position.x += enemyInstance.velocity.x * deltaTime;
			enemyInstance.position.y += enemyInstance.velocity.y * deltaTime;

			//--- Update destination rectangle ---
			enemyInstance.sprite_destination.x = enemyInstance.position.x;
			enemyInstance.sprite_destination.y = enemyInstance.position.y;

			enemyInstance.rotation += 90.0f * deltaTime; // Rotate enemy for visual effect

			enemy_warping(enemyInstance);	
		}

		//--- wave clear whooooo ---
		if (std::find_if(enemy_pool.begin(), enemy_pool.end(), [](const Enemy& enemy) { return enemy.active; }) == enemy_pool.end()) {
			waveCount++;
			newWave();
		}
	}

	void EnemySystem::newWave(){
		//--- increase enemies per wave & enemy speed ---
		enemies_per_wave += 2;
		enemy_speed += 10.0f;

		//--- resize pool and initialize newly added entries ---
		std::size_t oldSize = enemy_pool.size();
		enemy_pool.resize(enemies_per_wave);
		for (std::size_t i = oldSize; i < enemy_pool.size(); ++i) {
			enemy_pool[i].active = false;
			enemy_pool[i].sprite_destination.width = enemy_small_sprite_source.width;
			enemy_pool[i].sprite_destination.height = enemy_small_sprite_source.height;
			enemy_pool[i].position = { 0, 0 };
			enemy_pool[i].velocity = { 0, 0 };
		}

		//--- spawn the new wave ---
		spawn_enemyWave();
	}

	void EnemySystem::checkCollisions(ProjectileSystem& projectiles) {
		for (auto& enemyInstance : enemy_pool) {
			if (!enemyInstance.active) continue;

			//--- checks as if the rectagle uses top-left coords fixes that projectiles sometimes dont register hits ---
			Rectangle enemyRect = enemyInstance.sprite_destination;
			enemyRect.x -= enemy_small_sprite_origin.x;
			enemyRect.y -= enemy_small_sprite_origin.y;

			for (auto& projInstance : projectiles.projectile_pool) {
				if (!projInstance.active) continue;

				//--- checks as if the rectagle uses top-left coords fixes that projectiles sometimes dont register hits ---
				Rectangle projRect = projInstance.sprite_destination;
				projRect.x -= projectiles.projectile_sprite_origin.x;
				projRect.y -= projectiles.projectile_sprite_origin.y;

				if (CheckCollisionRecs(enemyRect, projRect)) {
					enemyInstance.active = false;
					projInstance.active = false;
					PlaySound(enemy_explosion_sound);
					break;
				}
			}
		}
	}

	void EnemySystem::render() const {
		//--- Draw all active enemies ---
		for (const auto& enemyInstance : enemy_pool) {
			if (!enemyInstance.active) continue;
			Color tint = (enemyInstance.type == EnemyType::Seeker) ? RED : WHITE;
			DrawTexturePro(enemy_small_texture, enemy_small_sprite_source, enemyInstance.sprite_destination, enemy_small_sprite_origin, enemyInstance.rotation, tint);
		}
	}

} // end of namespace meteor_blitz