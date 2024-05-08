#include "simple_render_system.hpp"

#include "first_app.hpp"
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
			case GLFW_KEY_SPACE:
				app->togglePause();
				break;
			default:
				break;
			}
		}
	}
	void FirstApp::mouseProcess(GLFWwindow* window, int button, int action, int mods)
	{
		FirstApp* app = static_cast<FirstApp*>(glfwGetWindowUserPointer(window));
	}
	void FirstApp::run()
	{
		SimpleRenderSystem tmp_system(lve_device, lve_renderer.getSwapChainRenderPass());

		lve_window.setWindowUserPointer(this);
		lve_window.setKeyCallback(keyProcess);
		lve_window.setMouseButtonCallback(mouseProcess);


		time_t timer = clock();

		uint32_t fps = 60;

		while (!lve_window.shouldClose())
		{
			glfwPollEvents();
			if (clock() - timer >= (1000 / fps))
			{
				if (auto command_buffer = lve_renderer.beginFrame())
				{
					if (!paused)
					{

					}
					lve_renderer.beginSwapChainRenderPass(command_buffer);
					tmp_system.renderGameObjects(command_buffer, game_objects);
					lve_renderer.endSwapChainRenderPass(command_buffer);
					lve_renderer.endFrame();
				}
				timer = clock();
			}
		}
	}

	std::unique_ptr<LveModel> createCubeModel(LveDevice& device, glm::vec3 offset) {
		std::vector<LveModel::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.1f, -.5f, -.5f}, {.8f, .8f, .7f}},
			{{.1f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.1f, -.5f, .5f}, {.8f, .1f, .4f}},
			{{.1f, -.5f, -.5f}, {.8f, .1f, .7f}},
			{{.1f, .5f, -.5f}, {.4f, .1f, .1f}},
			{{.1f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto& v : vertices) {
			v.position += offset;
		}
		return std::make_unique<LveModel>(device, vertices);
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
		std::shared_ptr<LveModel> cube_model = createCubeModel(lve_device, glm::vec3{0.f});

		auto cube = LveGameObject::createGameObject();

		cube.transform.translation = { 0.f, 0.f, 0.5f };
		cube.transform.scale = { .5f, .5f, .5f };
		cube.model = cube_model;

		game_objects.push_back(std::move(cube));
	}

	void FirstApp::togglePause()
	{
		paused = !paused;
	}

	LveWindow* FirstApp::lveWindow()
	{
		return &lve_window;
	}
}