#include "first_app.hpp"
#include "lve_camera.hpp"
#include "keyboard_movement_controller.hpp"
#include "mouse_contorller.hpp"

#include "simple_render_system.hpp"
#include "camera_follow_render_system.hpp"
#include "pushback_render_system.hpp"
#include "gravitation_render_system.hpp"
#include "move_render_system.hpp"

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
		CameraFollowRenderSystem follow_system(lve_device, lve_renderer.getSwapChainRenderPass());
		PushbackRenderSystem pushback_system(lve_device, lve_renderer.getSwapChainRenderPass());
		GravitationRenderSystem gravity(lve_device, lve_renderer.getSwapChainRenderPass());
		MoveRenderSystem move(lve_device, lve_renderer.getSwapChainRenderPass());

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
			lve_camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 1000.f);

			if (auto command_buffer = lve_renderer.beginFrame())
			{
				lve_renderer.beginSwapChainRenderPass(command_buffer);
				//follow_system.update(game_objects, viewer_object);
				//pushback_system.update(game_objects, 0.1f);
				gravity.update(game_objects.begin() + 1, game_objects.end());
				move.update(game_objects.begin() + 1, game_objects.end());

				tmp_system.renderGameObjects(command_buffer, game_objects, lve_camera);
				lve_renderer.endSwapChainRenderPass(command_buffer);
				lve_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lve_device.device());
	}


	std::unique_ptr<LveModel> createFloorModel(LveDevice& device, glm::vec3 offset)
	{
		std::vector<LveModel::Vertex> vertices
		{
			{{-.5f, .0f, -.5f}, {.25f, .25f, .25f}},
			{{.5f, .0f, .5f}, {.25f, .25f, .25f}},
			{{-.5f, .0f, .5f}, {.25f, .25f, .25f}},
			{{-.5f, .0f, -.5f}, {.25f, .25f, .25f}},
			{{.5f, .0f, -.5f}, {.25f, .25f, .25f}},
			{{.5f, .0f, .5f}, {.25f, .25f, .25f}},
		};

		for (auto& v : vertices)
		{
			v.position += offset;
		}

		return std::make_unique<LveModel>(device, vertices);
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
		std::shared_ptr<LveModel> floor_model = createFloorModel(lve_device, glm::vec3{ 0.f });

		auto floor = LveGameObject::createGameObject();
		floor.transform.translation.y += 50.f;
		floor.transform.scale = { 300.f, 1.f, 300.f };
		floor.model = floor_model;
		game_objects.push_back(std::move(floor));

		auto cube = LveGameObject::createGameObject();

		cube.transform.translation = { -2.5f, 10.f, -2.5f };
		cube.transform.scale = { .25f, .25f, .25f };
		cube.model = cube_model;
		cube.mass = 1.f;
		cube.speed = 0.1f / cube.mass;
		game_objects.push_back(std::move(cube));


		auto cube2 = LveGameObject::createGameObject();

		cube2.transform.scale = { .25f, .25f, .25f };
		cube2.model = cube_model;
		cube2.transform.translation = { -2.5f, -1.f, 0.f };
		cube2.mass = 1.f;
		cube2.speed = 0.1f / cube2.mass;
		game_objects.push_back(std::move(cube2));

		auto cube3 = LveGameObject::createGameObject();

		cube3.transform.scale = { .25f, .25f, .25f };
		cube3.model = cube_model;
		cube3.transform.translation = { 2.25f, -1.f, 2.25f };
		cube3.mass = 1.f;
		cube3.speed = 0.1f / cube3.mass;
		game_objects.push_back(std::move(cube3));

		auto cube4 = LveGameObject::createGameObject();

		cube4.transform.scale = { .125f, .125f, .125f };
		cube4.model = cube_model;
		cube4.transform.translation = { 3.25f, -5.f, 3.25f };
		cube4.mass = .1f;
		cube4.speed = 0.1f / cube4.mass;
		game_objects.push_back(std::move(cube4));

		auto cube5 = LveGameObject::createGameObject();

		cube5.transform.scale = { .125f, .125f, .125f };
		cube5.model = cube_model;
		cube5.transform.translation = { 4.25f, 5.f, 4.25f };
		cube5.mass = .1f;
		cube5.speed = 0.1f / cube5.mass;
		game_objects.push_back(std::move(cube5));

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