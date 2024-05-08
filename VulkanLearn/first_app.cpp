#include "move_render_system.hpp"
#include "scale_render_system.hpp"
#include "color_render_system.hpp"
#include "gravitation_render_system.hpp"
#include "pushback_render_system.hpp"
#include "holding_render_system.hpp"
#include "bounce_render_system.hpp"

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
			case GLFW_KEY_T:
				app->addTriangle();
				//app->vertices = LveModel::Vertex::makeSerpinskiStep(app->vertices);
				//app->reloadModels(app->vertices);
				break;
			case GLFW_KEY_BACKSPACE:
				app->eraseObjects();
				break;
			case GLFW_KEY_S:
				app->addSquare();
				break;
			case GLFW_KEY_C:
				app->addCircle(glm::vec2{rand() % 100 / 100.f});
				break;
			case GLFW_KEY_F:
				app->addStaticFat(glm::vec2{ 0.f }, glm::vec2{ 0.f });
				break;
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

		double x, y;

		glm::vec2 first_pos{}, second_pos{};

		if (action == GLFW_PRESS)
		{
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
			{
				glfwGetCursorPos(window, &x, &y);
				glm::vec2 pos{ static_cast<float>(x / (WIDTH / 2) - 1), static_cast<float>(y / (HEIGHT / 2) - 1) };

				app->setObjectTranslation(0, pos);
				break;
			}
			case GLFW_MOUSE_BUTTON_RIGHT:
				glfwGetCursorPos(window, &x, &y);
				app->old_mouse_pos = { static_cast<float>(x / (WIDTH / 2) - 1), static_cast<float>(y / (HEIGHT / 2) - 1) };


				break;
			}
		}
		if (action == GLFW_RELEASE)
		{
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_RIGHT:
				glfwGetCursorPos(window, &x, &y);
				second_pos = { static_cast<float>(x / (WIDTH / 2) - 1), static_cast<float>(y / (HEIGHT / 2) - 1) };
				app->addStaticFat(app->old_mouse_pos, second_pos - app->old_mouse_pos);


				break;
			}
		}
	}
	void FirstApp::run()
	{
		MoveRenderSystem move_render_system(lve_device, lve_renderer.getSwapChainRenderPass());
		BounceRenderSystem bounce_render_system(lve_device, lve_renderer.getSwapChainRenderPass());
		//ScaleRenderSystem scale_render_system(lve_device, lve_renderer.getSwapChainRenderPass());
		ColorRenderSystem color_render_system(lve_device, lve_renderer.getSwapChainRenderPass());
		GravitationRenderSystem gravitation_render_system(lve_device, lve_renderer.getSwapChainRenderPass());
		PushbackRenderSystem pushback_render_system(lve_device, lve_renderer.getSwapChainRenderPass());
		HoldingRenerSystem holding_render_system(lve_device, lve_renderer.getSwapChainRenderPass());


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

						move_render_system.update(game_objects.begin(), game_objects.end());

						//pushback_render_system.update(game_objects.begin(), game_objects.end(), 0.015f);  // 0.03f is good
						gravitation_render_system.update(game_objects.begin(), game_objects.end());

						color_render_system.update(game_objects.begin(), game_objects.end());

						//bounce_render_system.update(game_objects.begin(), game_objects.end());
						//holding_render_system.update(game_objects.begin(), game_objects.end());
					}
					lve_renderer.beginSwapChainRenderPass(command_buffer);
					move_render_system.renderGameObjects(command_buffer, game_objects);
					lve_renderer.endSwapChainRenderPass(command_buffer);
					lve_renderer.endFrame();
				}
				timer = clock();
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

	void FirstApp::addCircle(glm::vec2 offset, glm::vec2 vector)
	{
		addObject(SimpleRenderSystem::createCircleModel(lve_device, 100), 0.5f, 1.f, offset, vector);
	}

	void FirstApp::addCircle(glm::vec2 pos)
	{
		addObject(SimpleRenderSystem::createCircleModel(lve_device, 100), 0.5f, 1.f, pos);
	}

	void FirstApp::addStaticFat(glm::vec2 pos, glm::vec2 vector)
	{

		addObject(SimpleRenderSystem::createCircleModel(lve_device, 6), 1.f, 1000.f, pos, vector);
	}

	void FirstApp::addObject(std::shared_ptr<LveModel> model)
	{
		addObject(model, 1.f, 2.f);
	}

	void FirstApp::addObject(std::shared_ptr<LveModel> model, float scale, float mass)
	{
		glm::vec2 offset
		{
			(rand() % 200) / 100.f - 1,
			(rand() % 200) / 100.f - 1
		};

		addObject(model, scale, mass, offset);
	}

	void FirstApp::addObject(std::shared_ptr<LveModel> model, float scale, float mass, glm::vec2 offset)
	{
		glm::vec2 direction{ (((rand() % 2) * 2) - 1) , (((rand() % 2) * 2) - 1) };
		addObject(model, scale, mass, offset, direction);
	}

	void FirstApp::addObject(std::shared_ptr<LveModel> model, float scale, float mass, glm::vec2 offset, glm::vec2 direction)
	{
		glm::vec3 color
		{
			(rand() % 50) / 100.f + 0.5f,
			(rand() % 50) / 100.f + 0.2f,
			(rand() % 50) / 100.f + 0.5f
		};

		auto object = LveGameObject::createGameObject();
		object.transform_2d.translation = offset;
		object.model = model;
		object.color = color;
		object.transform_2d.scale = { 0.02f * scale, 0.02f * scale };
		object.mass = mass;
		object.speed = 0.025f / mass;
		object.direction = direction;

		game_objects.push_back(std::move(object));
	}

	void FirstApp::setObjectTranslation(size_t index, glm::vec2 translation)
	{
		game_objects[index].transform_2d.translation = translation;
	}

	void FirstApp::eraseObjects()
	{
		game_objects.erase(game_objects.begin(), game_objects.end());
	}

	LveWindow* FirstApp::lveWindow()
	{
		return &lve_window;
	}
}