#pragma once
#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"

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

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;
	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();


		LveWindow lve_window{ WIDTH, HEIGHT, "World!" };
		LveDevice lve_device{ lve_window };
		LveSwapChain lve_swap_chain{ lve_device, lve_window.getExtend() };

		std::unique_ptr<LvePipeline> lve_pipeline;
		VkPipelineLayout pipeline_layout;
		std::vector<VkCommandBuffer> command_buffers;
	};
}