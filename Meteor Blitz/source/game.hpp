//game header

#pragma once

#include "common.hpp"

namespace meteor_blitz
{
	class Game {
	public:
		void initialize();
		bool is_running() const;
		void update(float dt);
		void render();
	private:

		bool running = true;
	};
} // end of namespace meteor_blitz