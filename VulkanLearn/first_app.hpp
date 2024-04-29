#pragma once
#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_model.hpp"
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
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 800;

		void run();
		LveWindow* lveWindow();

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;
	private:
		static void keyProcess(GLFWwindow* window, int key, int scancode, int action, int mods);

		/*void reloadModels(const std::vector<LveModel::Vertex>& vertices);*/
		void loadGameObjects();
		void recreateSwapChain();
		void recordCommandBuffer(int image_index);
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		//void reloadCommandBuffers(); // Todo
		void drawFrame();
		static void drawing(FirstApp* this_app);
		void renderGameObjects(VkCommandBuffer command_buffer);

		LveWindow lve_window{ WIDTH, HEIGHT, "World!" };
		LveDevice lve_device{ lve_window };
		std::unique_ptr<LveSwapChain> lve_swap_chain;
		std::vector<LveModel::Vertex> vertices;

		std::unique_ptr<LvePipeline> lve_pipeline;
		std::vector<LveGameObject> game_objects;
		VkPipelineLayout pipeline_layout;
		std::vector<VkCommandBuffer> command_buffers;
	};
}