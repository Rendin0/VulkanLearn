#pragma once
#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_model.hpp"
#include "lve_renderer.hpp"
#include "lve_game_object.hpp"

#include <memory>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLM/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH =  800;
		static constexpr int HEIGHT = 800;

		void run();
		LveWindow* lveWindow();

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;
	private:
		static void keyProcess(GLFWwindow* window, int key, int scancode, int action, int mods);

		void loadGameObjects();

		LveWindow lve_window{ WIDTH, HEIGHT, "World!" };
		LveDevice lve_device{ lve_window };
		LveRenderer lve_renderer{ lve_window, lve_device };

		void togglePause();

		void addTriangle();
		void addSquare();
		void addCircle();

		void addObject(std::shared_ptr<LveModel> model);
		void addObject(std::shared_ptr<LveModel> model, float scale);

		bool paused = false;

		std::vector<LveGameObject> game_objects;
	};
}