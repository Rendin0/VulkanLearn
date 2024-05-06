#include "first_app.hpp"
#include "dvd_render_system.hpp"
#include "scale_render_system.hpp"
#include "color_render_system.hpp"
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
			case GLFW_KEY_T:
				app->addTriangle();
				//app->vertices = LveModel::Vertex::makeSerpinskiStep(app->vertices);
				//app->reloadModels(app->vertices);
				break;
			case GLFW_KEY_ESCAPE: // Todo: Closing window by key
				//app->lve_window.windowShouldClose();
				break;
			case GLFW_KEY_S:
				app->addSquare();
				break;
			case GLFW_KEY_C:
				app->addCircle();
				break;
			case GLFW_KEY_SPACE:
				app->togglePause();
				break;
			default:
				break;
			}
		}
	}
	void FirstApp::run()
	{
		DvdRenerSystem dvd_render_system(lve_device, lve_renderer.getSwapChainRenderPass());
		ScaleRenderSystem scale_render_system(lve_device, lve_renderer.getSwapChainRenderPass());
		ColorRenderSystem color_render_system(lve_device, lve_renderer.getSwapChainRenderPass());

		lve_window.setWindowUserPointer(this);
		lve_window.setKeyCallback(keyProcess);

		while (!lve_window.shouldClose())
		{
			glfwPollEvents();
			if (!paused)
			{

				if (auto command_buffer = lve_renderer.beginFrame())
				{
					dvd_render_system.update(game_objects.begin(), game_objects.end(), 0.02f);
					scale_render_system.update(game_objects.begin(), game_objects.end());
					color_render_system.update(game_objects.begin(), game_objects.end());

					lve_renderer.beginSwapChainRenderPass(command_buffer);
					dvd_render_system.renderGameObjects(command_buffer, game_objects);
					lve_renderer.endSwapChainRenderPass(command_buffer);
					lve_renderer.endFrame();
				}
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
		addCircle();
	}

	void FirstApp::togglePause()
	{
		paused = !paused;
	}

	void FirstApp::addTriangle()
	{
		addObject(SimpleRenderSystem::createTriangleModel(lve_device));
	}

	void FirstApp::addSquare()
	{
		addObject(SimpleRenderSystem::createSquareModel(lve_device));
	}

	void FirstApp::addCircle()
	{
		addObject(SimpleRenderSystem::createCircleModel(lve_device, 100), 0.5f);
	}

	void FirstApp::addObject(std::shared_ptr<LveModel> model)
	{
		addObject(model, 1.f);
	}

	void FirstApp::addObject(std::shared_ptr<LveModel> model, float scale)
	{
		glm::vec3 color
		{
			(rand() % 50) / 100.f + 0.5f,
			(rand() % 50) / 100.f + 0.2f,
			(rand() % 50) / 100.f + 0.5f
		};

		glm::vec2 offset
		{
			(rand() % 200) / 100.f - 1,
			(rand() % 200) / 100.f - 1
		};

		auto circle = LveGameObject::createGameObject();
		circle.transform_2d.translation = offset;
		circle.model = model;
		circle.color = color;
		circle.transform_2d.scale = { 0.1f * scale, 0.1f * scale };

		game_objects.push_back(std::move(circle));
	}

	LveWindow* FirstApp::lveWindow()
	{
		return &lve_window;
	}
}