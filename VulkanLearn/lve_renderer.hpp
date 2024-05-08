#pragma once
#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_model.hpp"

#include <memory>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLM/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve
{
	class LveRenderer
	{
	public:
		LveRenderer(LveWindow& window, LveDevice& device);
		~LveRenderer();

		LveRenderer(const LveRenderer&) = delete;
		LveRenderer& operator=(const LveRenderer&) = delete;

		bool isFrameInProgress() const;

		VkCommandBuffer getCurrentCommandBuffer() const;
		VkRenderPass getSwapChainRenderPass() const;
		float getAspectRation() const;

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer command_buffer);
		void endSwapChainRenderPass(VkCommandBuffer command_buffer);

		int getFrameIndex() const;

	private:
		void recreateSwapChain();
		void createCommandBuffers();
		void freeCommandBuffers();

		LveWindow& lve_window;
		LveDevice& lve_device;
		std::unique_ptr<LveSwapChain> lve_swap_chain;
		std::vector<VkCommandBuffer> command_buffers;

		uint32_t current_image_index;
		int current_frame_index;
		bool is_frame_started = false;
	};
}
