#include "first_app.hpp"
#include "simple_render_system.hpp"
#include <stdexcept>
#include <array>
#include <conio.h>
#include <iostream>
#include <thread>
#include <ctime>

namespace lve
{
	void FirstApp::keyProcess(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		FirstApp* app = static_cast<FirstApp*>(glfwGetWindowUserPointer(window));

		if (action == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_ENTER:
				//app->vertices = LveModel::Vertex::makeSerpinskiStep(app->vertices);
				//app->reloadModels(app->vertices);
				break;
			case GLFW_KEY_ESCAPE: // Todo: Closing window by key
				//app->lve_window.windowShouldClose();
				break;
			case GLFW_KEY_RIGHT:
				break;
			default:
				break;
			}
		}
	}
	void FirstApp::run()
	{
		SimpleRenderSystem simple_render_system(lve_device, lve_renderer.getSwapChainRenderPass());

		lve_window.setWindowUserPointer(this);
		lve_window.setKeyCallback(keyProcess);

		while (!lve_window.shouldClose())
		{
			glfwPollEvents();

			if (auto command_buffer = lve_renderer.beginFrame())
			{
				lve_renderer.beginSwapChainRenderPass(command_buffer);
				simple_render_system.renderGameObjects(command_buffer, game_objects);
				lve_renderer.endSwapChainRenderPass(command_buffer);
				lve_renderer.endFrame();
			}
		}
	}

	FirstApp::FirstApp()
	{
		loadGameObjects();
	}
	FirstApp::~FirstApp()
	{
	}

	void FirstApp::loadGameObjects()
	{
		vertices =
		{
			{{0.0f, -0.5f}, {0.1f, 0.0f, 0.34f}},
			{{0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}},
			{{-0.5f, 0.5f}, {0.0f, 0.5f, 0.5f}}
		};

		auto lve_model = std::make_shared<LveModel>(lve_device, vertices);


		std::vector<glm::vec3> colors =
		{
			{1.f, .7f, .73f},
			{1.f, .87f, .73f},
			{1.f, 1.f, .73f},
			{.73f, 1.f, .8f},
			{.73, .88f, 1.f}
		};

		std::vector<glm::vec2> translations =
		{
			{{0.f}, {-1.f}},
			{{1.f}, {1.f}},
			{{-1.f}, {1.f}}
		};

		for (int i = 0; i < 3; i++)
		{
			auto triangle = LveGameObject::createGameObject();
			triangle.model = lve_model;
			triangle.color = { colors[i % colors.size()] };
			triangle.transform_2d.translation.x = translations[i % translations.size()].x * 0.25f;
			triangle.transform_2d.translation.y = translations[i % translations.size()].y * 0.25f;
			triangle.transform_2d.scale *= glm::vec2(.5f);
			//triangle.transform_2d.rotation = i * glm::pi<float>() * .025f;

			game_objects.push_back(std::move(triangle));
		}


	}
	
	LveWindow* FirstApp::lveWindow()
	{
		return &lve_window;
	}
}