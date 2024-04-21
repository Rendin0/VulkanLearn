#pragma once
#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_model.hpp"

#include <memory>
#include <vector>

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

		void reloadModels(const std::vector<LveModel::Vertex>& vertices);
		void loadModels();
		void recreateSwapChain();
		void recordCommandBuffer(int image_index);
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void reloadCommandBuffers(); // Todo
		void drawFrame();


		LveWindow lve_window{ WIDTH, HEIGHT, "World!" };
		LveDevice lve_device{ lve_window };
		std::unique_ptr<LveSwapChain> lve_swap_chain;
		std::vector<LveModel::Vertex> vertices;

		std::unique_ptr<LvePipeline> lve_pipeline;
		std::unique_ptr<LveModel> lve_model;
		VkPipelineLayout pipeline_layout;
		std::vector<VkCommandBuffer> command_buffers;
	};
}