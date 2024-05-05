#include "lve_renderer.hpp"
#include <cassert>
#include "first_app.hpp"
#include <stdexcept>
#include <array>
#include <conio.h>
#include <iostream>
#include <thread>
#include <ctime>

namespace lve
{
	bool LveRenderer::isFrameInProgress() const
	{	
		return is_frame_started;
	}

	VkCommandBuffer LveRenderer::getCurrentCommandBuffer() const
	{
		assert(is_frame_started && "Cannot get command buffer, not in progress.");

		return command_buffers[current_frame_index];
	}
	VkRenderPass LveRenderer::getSwapChainRenderPass() const
	{
		return lve_swap_chain->getRenderPass();
	}

	VkCommandBuffer LveRenderer::beginFrame()
	{
		assert(!is_frame_started && "Frame is already started");
		auto result = lve_swap_chain->acquireNextImage(&current_image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire next image");

		is_frame_started = true;
		
		auto command_buffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
			throw std::runtime_error("Failed to begind command buffer");


		return command_buffer;
	}

	void LveRenderer::endFrame()
	{
		assert(is_frame_started && "Frame is not started yet.");

		auto command_buffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
			throw std::runtime_error("Failed to end command buffer");

		auto result = lve_swap_chain->submitCommandBuffers(&command_buffer, &current_image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lve_window.wasWindowResized())
		{
			lve_window.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to submit command buffers");

		is_frame_started = false;
		current_frame_index = (current_frame_index + 1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void LveRenderer::beginSwapChainRenderPass(VkCommandBuffer command_buffer)
	{
		assert(is_frame_started && "Cant begin swap chain while frame is not in progress");
		assert(command_buffer == getCurrentCommandBuffer() && "Cant begin render pass on different frame");

		VkRenderPassBeginInfo render_pass_info{};

		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = lve_swap_chain->getRenderPass();
		render_pass_info.framebuffer = lve_swap_chain->getFrameBuffer(current_image_index);

		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = lve_swap_chain->getSwapChainExtent();

		std::array<VkClearValue, 2> clear_values{};
		clear_values[0].color = { 0.05f, 0.05f, 0.05f, 1.0f };
		clear_values[1].depthStencil = { 1.0f, 0 };

		render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
		render_pass_info.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0;
		viewport.y = 0;
		viewport.height = static_cast<float>(lve_swap_chain->getSwapChainExtent().height);
		viewport.width = static_cast<float>(lve_swap_chain->getSwapChainExtent().width);
		viewport.maxDepth = 1.0f;
		viewport.minDepth = 0.0f;

		VkRect2D scissor{ {0, 0}, lve_swap_chain->getSwapChainExtent() };
		vkCmdSetViewport(command_buffer, 0, 1, &viewport);
		vkCmdSetScissor(command_buffer, 0, 1, &scissor);
	}

	void LveRenderer::endSwapChainRenderPass(VkCommandBuffer command_buffer)
	{
		assert(is_frame_started && "Cant end swap chain while frame is not in progress");
		assert(command_buffer == getCurrentCommandBuffer() && "Cant end render pass on different frame");

		vkCmdEndRenderPass(command_buffer);


	}

	int LveRenderer::getFrameIndex() const
	{
		assert(is_frame_started && "Cant get frame index while frame is not in progress");
		return current_frame_index;
	}

	LveRenderer::LveRenderer(LveWindow& window, LveDevice& device)
		: lve_window(window), lve_device(device)
	{
		recreateSwapChain();
		createCommandBuffers();
	}

	LveRenderer::~LveRenderer()
	{
		freeCommandBuffers();
	}

	void LveRenderer::recreateSwapChain()
	{
		auto extend = lve_window.getExtend();

		while (extend.width == 0 || extend.height == 0)
		{
			extend = lve_window.getExtend();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(lve_device.device());

		if (lve_swap_chain == nullptr)
			lve_swap_chain = std::make_unique<LveSwapChain>(lve_device, extend);
		else
		{
			std::shared_ptr<LveSwapChain> old_swap_chain = std::move(lve_swap_chain); 

			lve_swap_chain = std::make_unique<LveSwapChain>(lve_device, extend, old_swap_chain);

			if (!old_swap_chain->compareSwapFormats(*lve_swap_chain.get()))
			{
				throw std::runtime_error("Swap chain image format has changed");
			}
		}

	}

	void LveRenderer::createCommandBuffers()
	{
		command_buffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocate_info{};

		allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocate_info.commandPool = lve_device.getCommandPool();
		allocate_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

		if (vkAllocateCommandBuffers(lve_device.device(), &allocate_info, command_buffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers");

	}
	void LveRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(lve_device.device(), lve_device.getCommandPool(), static_cast<uint32_t>(command_buffers.size()), command_buffers.data());
		command_buffers.clear();
	}

	LveWindow* LveRenderer::lveWindow()
	{
		return &lve_window;
	}
}