//game header

#pragma once

#include "common.hpp"

namespace meteor_blitz
{
	enum class GameState {
		Start,
		Play,
		GAME_OVER
	};

	class Game {
	public:		
		//--- game functions ---
		void initialize();
		bool is_running() const;
		void update(float dt);
		void render();
		int center_textX(const char* text, int fontSize);
		static void set_game_state(GameState state);
		static GameState get_game_state();

		//--- sounds ---
		const char* put_in_coin_sound_path = "assets/sound/coin.wav";
		Sound put_in_coin_sound = {};

	private:
		void reset_game();
		bool running = true;

		int coin = 0;

		static GameState game_state;	
	};
} // end of namespace meteor_blitz