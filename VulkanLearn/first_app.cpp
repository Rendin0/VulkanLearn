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
				gravity.update(game_objects.begin(), game_objects.end(), frame_time);
				move.update(game_objects.begin(), game_objects.end(), frame_time);

				tmp_system.renderGameObjects(command_buffer, game_objects, lve_camera);
				lve_renderer.endSwapChainRenderPass(command_buffer);
				lve_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lve_device.device());
	}


	std::unique_ptr<LveModel> createFloorModel(LveDevice& device, glm::vec3 offset)
	{
		LveModel::Builder builder{};

		builder.vertices =
		{
			{{-.5f, .0f, -.5f}, {.25f, .25f, .25f}},
			{{.5f, .0f, .5f}, {.25f, .25f, .25f}},
			{{-.5f, .0f, .5f}, {.25f, .25f, .25f}},
			{{.5f, .0f, -.5f}, {.25f, .25f, .25f}},
		};

		builder.indices = { 0, 1, 2, 0, 3, 1 };

		for (auto& v : builder.vertices)
		{
			v.position += offset;
		}

		return std::make_unique<LveModel>(device, builder);
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
		std::shared_ptr<LveModel> cube_model = LveModel::createModelFromFile(lve_device, "Models/smooth_vase.obj");
		
		glm::vec3 large_scale = { 2.f, 2.f, 2.f };
		glm::vec3 small_scale = { .75f, .75f, .75f };

		auto game_object1 = LveGameObject::createGameObject();

		game_object1.transform.translation = { -2.5f, 10.f, -2.5f };
		game_object1.transform.scale = large_scale;
		game_object1.model = cube_model;
		game_object1.mass = 1.f;
		game_object1.speed = 0.1f / game_object1.mass;
		game_objects.push_back(std::move(game_object1));


		auto game_object2 = LveGameObject::createGameObject();

		game_object2.transform.scale = large_scale;
		game_object2.model = cube_model;
		game_object2.transform.translation = { -2.5f, -1.f, 0.f };
		game_object2.mass = 1.f;
		game_object2.speed = 0.1f / game_object2.mass;
		game_objects.push_back(std::move(game_object2));

		auto game_object3 = LveGameObject::createGameObject();

		game_object3.transform.scale = large_scale;
		game_object3.model = cube_model;
		game_object3.transform.translation = { 2.25f, -1.f, 2.25f };
		game_object3.mass = 1.f;
		game_object3.speed = 0.1f / game_object3.mass;
		game_objects.push_back(std::move(game_object3));

		auto game_object4 = LveGameObject::createGameObject();

		game_object4.transform.scale = small_scale;
		game_object4.model = cube_model;
		game_object4.transform.translation = { 3.25f, -5.f, 3.25f };
		game_object4.mass = .1f;
		game_object4.speed = 0.1f / game_object4.mass;
		game_objects.push_back(std::move(game_object4));

		auto game_object5 = LveGameObject::createGameObject();

		game_object5.transform.scale = small_scale;
		game_object5.model = cube_model;
		game_object5.transform.translation = { 4.25f, 5.f, 4.25f };
		game_object5.mass = .1f;
		game_object5.speed = 0.1f / game_object5.mass;
		game_objects.push_back(std::move(game_object5));

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