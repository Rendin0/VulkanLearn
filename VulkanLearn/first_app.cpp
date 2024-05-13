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
		SimpleRenderSystem render_system(lve_device, lve_renderer.getSwapChainRenderPass());
		CameraFollowRenderSystem follow_system(lve_device, lve_renderer.getSwapChainRenderPass());
		PushbackRenderSystem pushback_system(lve_device, lve_renderer.getSwapChainRenderPass());
		GravitationRenderSystem gravity(lve_device, lve_renderer.getSwapChainRenderPass());
		MoveRenderSystem move(lve_device, lve_renderer.getSwapChainRenderPass());

		bool paused = false;

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

			camera_controller.keyboardController(lve_window.getWindowPointer(), frame_time, viewer_object, paused);
			mouse.rotateInPlane(lve_window.getWindowPointer(), frame_time, viewer_object);
			lve_camera.setViewYXZ(viewer_object.transform.translation, viewer_object.transform.rotation);

			float aspect = lve_renderer.getAspectRation();
			lve_camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 1000.f);

			if (auto command_buffer = lve_renderer.beginFrame())
			{
				lve_renderer.beginSwapChainRenderPass(command_buffer);

				if (!paused)
				{
					//follow_system.update(game_objects, viewer_object);
					//pushback_system.update(game_objects, 0.1f);
					gravity.update(game_objects.begin() + 1, game_objects.end(), frame_time);
					move.update(game_objects.begin() + 1, game_objects.end(), frame_time);
				}

				render_system.renderGameObjects(command_buffer, game_objects, lve_camera);
				lve_renderer.endSwapChainRenderPass(command_buffer);
				lve_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lve_device.device());
	}

	FirstApp::FirstApp()
	{
		loadGameObjects();
	}

	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<LveModel> floor_model = LveModel::createModelFromFile(lve_device, "Models/square.obj");

		std::shared_ptr<LveModel> game_model1 = LveModel::createModelFromFile(lve_device, "Models/FinalBaseMesh.obj");

		std::shared_ptr<LveModel> game_model2 = LveModel::createModelFromFile(lve_device, "Models/smooth_vase.obj");


		auto floor_object = LveGameObject::createGameObject();
		floor_object.model = floor_model;
		floor_object.transform.translation = {0.f, 50.f, 0.f};
		floor_object.transform.scale = { 100.f, 0.f, 100.f };
		game_objects.push_back(std::move(floor_object));

		auto simple_object_large = LveGameObject::createGameObject();
		simple_object_large.transform.scale = glm::vec3(-.05f);
		simple_object_large.model = game_model1;
		simple_object_large.mass = 1.f;
		simple_object_large.speed = 0.1f / simple_object_large.mass;

		auto simple_object_small = LveGameObject::createGameObject(simple_object_large);
		simple_object_small.model = game_model2;
		simple_object_small.transform.scale = glm::vec3(1.f);
		simple_object_small.mass = .1f;

		auto game_object1 = LveGameObject::createGameObject(simple_object_large);
		game_object1.transform.translation = { -2.5f, 10.f, -2.5f };
		game_objects.push_back(std::move(game_object1));

		auto game_object2 = LveGameObject::createGameObject(simple_object_large);
		game_object2.transform.translation = { -2.5f, -1.f, 0.f };
		game_objects.push_back(std::move(game_object2));

		auto game_object3 = LveGameObject::createGameObject(simple_object_large);
		game_object3.transform.translation = { 2.25f, -1.f, 2.25f };
		game_objects.push_back(std::move(game_object3));

		auto game_object4 = LveGameObject::createGameObject(simple_object_small);
		game_object4.transform.translation = { 3.25f, -5.f, 3.25f };
		game_objects.push_back(std::move(game_object4));

		auto game_object5 = LveGameObject::createGameObject(simple_object_small);
		game_object5.transform.translation = { 4.25f, 5.f, 4.25f };
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