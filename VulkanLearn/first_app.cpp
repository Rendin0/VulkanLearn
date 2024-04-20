#include "first_app.hpp"
#include <stdexcept>
#include <array>
#include <conio.h>
#include <iostream>

namespace lve
{
	void FirstApp::keyProcess(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		FirstApp* app = static_cast<FirstApp*>(glfwGetWindowUserPointer(window));

		if (action == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_ENTER:
				app->vertices = LveModel::Vertex::makeSerpinskiStep(app->vertices);
				app->reloadModels(app->vertices);
				app->createCommandBuffers();
				break;
			case GLFW_KEY_ESCAPE: // Todo: Closing window by key
				//app->lve_window.windowShoulsClose();
				break;
			default:
				break;
			}
		}
	}
	void FirstApp::run()
	{
		lve_window.setWindowUserPointer(this);
		lve_window.setKeyCallback(keyProcess);

		while (!lve_window.shouldClose())
		{
			glfwPollEvents();
			drawFrame();

		}
	}
	FirstApp::FirstApp()
	{
		loadModels();
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}
	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}
	void FirstApp::reloadModels(const std::vector<LveModel::Vertex>& vertices)
	{
		lve_model.reset(new LveModel(lve_device, vertices));
	}
	void FirstApp::loadModels()
	{
		vertices =
		{
			{{0.0f, -0.95f}},
			{{0.95f, 0.95f}},
			{{-0.95f, 0.95f}}
		};

		lve_model = std::make_unique<LveModel>(lve_device, vertices);
	}
	void FirstApp::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 0;
		pipeline_layout_info.pSetLayouts = nullptr;
		pipeline_layout_info.pushConstantRangeCount = 0;
		pipeline_layout_info.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(lve_device.device(), &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipline layout");
	}
	void FirstApp::createPipeline()
	{
		auto pipeline_config = LvePipeline::defaultPipelineConfigInfo(lve_swap_chain.width(), lve_swap_chain.height());
		pipeline_config.renderPass = lve_swap_chain.getRenderPass();
		pipeline_config.pipelineLayout = pipeline_layout;
		lve_pipeline = std::make_unique<LvePipeline>(lve_device, "Shaders\\simple_shader.vert.spv", "Shaders\\simple_shader.frag.spv", pipeline_config);
	}
	void FirstApp::createCommandBuffers()
	{
		command_buffers.resize(lve_swap_chain.imageCount());

		VkCommandBufferAllocateInfo allocate_info{};

		allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocate_info.commandPool = lve_device.getCommandPool();
		allocate_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

		if (vkAllocateCommandBuffers(lve_device.device(), &allocate_info, command_buffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers");

		for (int i = 0; i < command_buffers.size(); i++)
		{
			VkCommandBufferBeginInfo begin_info{};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(command_buffers[i], &begin_info) != VK_SUCCESS)
				throw std::runtime_error("Failed to begind command buffer");


			VkRenderPassBeginInfo render_pass_info{};

			render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			render_pass_info.renderPass = lve_swap_chain.getRenderPass();
			render_pass_info.framebuffer = lve_swap_chain.getFrameBuffer(i);

			render_pass_info.renderArea.offset = { 0, 0 };
			render_pass_info.renderArea.extent = lve_swap_chain.getSwapChainExtent();

			std::array<VkClearValue, 2> clear_values{};
			clear_values[0].color = { 0.05f, 0.05f, 0.05f, 1.0f };
			clear_values[1].depthStencil = { 1.0f, 0 };

			render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
			render_pass_info.pClearValues = clear_values.data();

			vkCmdBeginRenderPass(command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

			lve_pipeline->bind(command_buffers[i]);
			lve_model->bind(command_buffers[i]);
			lve_model->draw(command_buffers[i]);

			vkCmdEndRenderPass(command_buffers[i]);
			if (vkEndCommandBuffer(command_buffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to end command buffer");
		}

	}
	void FirstApp::drawFrame()
	{

		uint32_t image_index;
		auto result = lve_swap_chain.acquireNextImage(&image_index);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire next image");

		result = lve_swap_chain.submitCommandBuffers(&command_buffers.at(image_index), &image_index);

		if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to submit command buffers");
	}
}