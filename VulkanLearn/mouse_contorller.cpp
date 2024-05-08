#include "mouse_contorller.hpp"
#include <limits>
#include <iostream>

namespace lve
{
	void MouseContorller::rotateInPlane(GLFWwindow* window, float dt, LveGameObject& game_object)
	{
		glm::vec3 rotate{ 0.f };

		double x, y;
		glfwGetCursorPos(window, &y, &x);

		rotate.x = -(static_cast<float>(x));
		rotate.y = (static_cast<float>(y));

		glfwSetCursorPos(window, 0, 0);
		
		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
		{
			game_object.transform.rotation += look_speed * dt * rotate;
		}

		game_object.transform.rotation.x = glm::clamp(game_object.transform.rotation.x, -1.5f, 1.5f);
		game_object.transform.rotation.y = glm::mod(game_object.transform.rotation.y, glm::two_pi<float>());
	}
}
