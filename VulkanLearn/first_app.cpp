#include "first_app.hpp"
#include "dvd_render_system.hpp"
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
				app->addTriangle();
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
		DvdRenerSystem simple_render_system(lve_device, lve_renderer.getSwapChainRenderPass());

		lve_window.setWindowUserPointer(this);
		lve_window.setKeyCallback(keyProcess);

		while (!lve_window.shouldClose())
		{
			glfwPollEvents();

			if (auto command_buffer = lve_renderer.beginFrame())
			{
				simple_render_system.update(game_objects);

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

	void FirstApp::addTriangle()
	{
		vertices =
		{
			{{0.0f, -0.5f}, {0.1f, 0.0f, 0.34f}},
			{{0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}},
			{{-0.5f, 0.5f}, {0.0f, 0.5f, 0.5f}}
		};
		auto lve_model = std::make_shared<LveModel>(lve_device, vertices);

		auto triangle = LveGameObject::createGameObject();
		triangle.transform_2d.translation = { (rand() % 100) / 100.f, (rand() % 100) / 100.f };
		triangle.model = lve_model;
		triangle.color = { (rand() % 100) / 100.f, (rand() % 100) / 100.f, (rand() % 100) / 100.f };
		triangle.transform_2d.scale = { 0.1f, 0.1f };

		game_objects.push_back(std::move(triangle));
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

		auto triangle = LveGameObject::createGameObject();
		triangle.model = lve_model;
		triangle.color = { .1f, .8f, .1f };
		triangle.transform_2d.scale = { 0.1f, 0.1f };

		game_objects.push_back(std::move(triangle));
	}

	LveWindow* FirstApp::lveWindow()
	{
		return &lve_window;
	}
}