#pragma once
#include "lve_window.hpp"
#include "lve_game_object.hpp"

namespace lve
{
	class MouseContorller
	{
	public:
		void rotateInPlane(GLFWwindow* window, float dt, LveGameObject& game_object);


	private:
		float look_speed{ .2f };

	};
}