#include "simple_render_system.hpp"
#include "first_app.hpp"
#include "lve_camera.hpp"
#include "keyboard_movement_controller.hpp"
#include "mouse_contorller.hpp"

#include <stdexcept>
#include <array>
#include <conio.h>
#include <iostream>
#include <thread>
#include <chrono>

namespace lve
{
	void FirstApp::run()
	{
		SimpleRenderSystem tmp_system(lve_device, lve_renderer.getSwapChainRenderPass());

		glfwSetInputMode(lve_window.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(lve_window.getWindowPointer(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		LveCamera lve_camera{};
		lve_camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewer_object = LveGameObject::createGameObject();
		KeyboardMovementController camera_controller{};
		MouseContorller mouse{};

		auto current_time = std::chrono::high_resolution_clock::now();

		while (!lve_window.shouldClose())
		{
			glfwPollEvents();

			auto new_time = std::chrono::high_resolution_clock::now();
			float frame_time = std::chrono::duration<float, std::chrono::seconds::period>(new_time - current_time).count();
			current_time = new_time;

			camera_controller.moveInPlaneXZ(lve_window.getWindowPointer(), frame_time, viewer_object);
			mouse.rotateInPlane(lve_window.getWindowPointer(), frame_time, viewer_object);
			lve_camera.setViewYXZ(viewer_object.transform.translation, viewer_object.transform.rotation);

			float aspect = lve_renderer.getAspectRation();
			lve_camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 200.f);

			if (auto command_buffer = lve_renderer.beginFrame())
			{
				lve_renderer.beginSwapChainRenderPass(command_buffer);
				tmp_system.renderGameObjects(command_buffer, game_objects, lve_camera);
				lve_renderer.endSwapChainRenderPass(command_buffer);
				lve_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lve_device.device());
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
			{{.5f, -.5f, -.5f}, {.8f, .8f, .7f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .1f, .4f}},
			{{.5f, -.5f, -.5f}, {.8f, .1f, .7f}},
			{{.5f, .5f, -.5f}, {.4f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

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
		std::shared_ptr<LveModel> cube_model = createCubeModel(lve_device, glm::vec3{ 0.f });

		auto cube = LveGameObject::createGameObject();

		cube.transform.translation = { -0.5f, 0.f, 0.f };
		cube.transform.scale = { .5f, .5f, .5f };
		cube.model = cube_model;

		game_objects.push_back(std::move(cube));

		auto cube2 = LveGameObject::createGameObject();

		cube2.transform.scale = { .5f, .5f, .5f };
		cube2.model = cube_model;
		cube2.transform.translation = { .5f, 0.f, 0.f };
		game_objects.push_back(std::move(cube2));

		auto cube3 = LveGameObject::createGameObject();

		cube3.transform.translation = { 0.f, 0.f, 0.f };
		cube3.transform.scale = { .5f, .5f, .5f };
		cube3.model = cube_model;
		game_objects.push_back(std::move(cube3));

		auto cube4 = LveGameObject::createGameObject();
		cube4.transform.scale = { .5f, .5f, .5f };
		cube4.transform.translation = { 0.f, -0.5f, 0.f };
		cube4.model = cube_model;

		auto cube5 = LveGameObject::createGameObject();
		cube5.transform.scale = { .5f, .5f, .5f };
		cube5.transform.translation = { 0.f, -1.f, 0.f };
		cube5.model = cube_model;

		auto cube6 = LveGameObject::createGameObject();
		cube6.transform.scale = { .5f, .5f, .5f };
		cube6.transform.translation = { 0.f, -1.5f, 0.f };
		cube6.model = cube_model;

		game_objects.push_back(std::move(cube4));
		game_objects.push_back(std::move(cube5));
		game_objects.push_back(std::move(cube6));
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