#include "first_app.hpp"
#include <stdexcept>
#include <array>
#include <conio.h>
#include <iostream>
#include <thread>
#include <ctime>

namespace lve
{
	struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	void FirstApp::keyProcess(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		FirstApp* app = static_cast<FirstApp*>(glfwGetWindowUserPointer(window));

		if (action == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_ENTER:
				//app->vertices = LveModel::Vertex::makeSerpinskiStep(app->vertices);
				//app->reloadModels(app->vertices);
				break;
			case GLFW_KEY_ESCAPE: // Todo: Closing window by key
				//app->lve_window.windowShouldClose();
				break;
			case GLFW_KEY_RIGHT:
				app->drawFrame();
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

		drawFrame();
		//std::thread drawin(drawing, this);
		while (!lve_window.shouldClose())
		{
			glfwPollEvents();
		}
		//drawin.join();
	}

	void FirstApp::drawing(FirstApp* this_app)
	{
		while (!this_app->lve_window.shouldClose())
		{
			this_app->drawFrame();
		}
	}

	void FirstApp::renderGameObjects(VkCommandBuffer command_buffer)
	{
		int i = 0;
		for (auto& obj : game_objects) {
			i += 1;
			//obj.transform_2d.rotation = glm::mod<float>(obj.transform_2d.rotation + 0.00001f * i, 2.f * glm::pi<float>());
		}

		lve_pipeline->bind(command_buffer);

		for (auto& obj : game_objects)
		{
			//obj.transform_2d.rotation = glm::mod(obj.transform_2d.rotation + .001f, glm::two_pi<float>());

			SimplePushConstantData push{};
			push.offset = obj.transform_2d.translation;
			push.color = obj.color;
			push.transform = obj.transform_2d.mat2();


			vkCmdPushConstants(command_buffer, pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			obj.model->bind(command_buffer);
			obj.model->draw(command_buffer);
		}
	}

	FirstApp::FirstApp()
	{
		loadGameObjects();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}
	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}
	/*void FirstApp::reloadModels(const std::vector<LveModel::Vertex>& vertices)
	{
		freeCommandBuffers();
		createCommandBuffers();
		lve_model.reset(new LveModel(lve_device, vertices));
	}*/
	void FirstApp::loadGameObjects()
	{
		vertices =
		{
			{{0.0f, -0.5f}, {0.1f, 0.0f, 0.34f}},
			{{0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}},
			{{-0.5f, 0.5f}, {0.0f, 0.5f, 0.5f}}
		};

		auto lve_model = std::make_shared<LveModel>(lve_device, vertices);


		std::vector<glm::vec3> colors =
		{
			{1.f, .7f, .73f},
			{1.f, .87f, .73f},
			{1.f, 1.f, .73f},
			{.73f, 1.f, .8f},
			{.73, .88f, 1.f}
		};

		std::vector<glm::vec2> translations =
		{
			{{0.f}, {-1.f}},
			{{1.f}, {1.f}},
			{{-1.f}, {1.f}}
		};

		for (int i = 0; i < 3; i++)
		{
			auto triangle = LveGameObject::createGameObject();
			triangle.model = lve_model;
			triangle.color = { colors[i % colors.size()] };
			triangle.transform_2d.translation.x = translations[i % translations.size()].x * 0.25f;
			triangle.transform_2d.translation.y = translations[i % translations.size()].y * 0.25f;
			triangle.transform_2d.scale *= glm::vec2(.5f);
			//triangle.transform_2d.rotation = i * glm::pi<float>() * .025f;

			game_objects.push_back(std::move(triangle));
		}


	}
	void FirstApp::recreateSwapChain()
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
			lve_swap_chain = std::make_unique<LveSwapChain>(lve_device, extend, std::move(lve_swap_chain));
			if (lve_swap_chain->imageCount() != command_buffers.size())
			{
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		createPipeline();
	}
	void FirstApp::recordCommandBuffer(int image_index)
	{
		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(command_buffers[image_index], &begin_info) != VK_SUCCESS)
			throw std::runtime_error("Failed to begind command buffer");


		VkRenderPassBeginInfo render_pass_info{};

		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = lve_swap_chain->getRenderPass();
		render_pass_info.framebuffer = lve_swap_chain->getFrameBuffer(image_index);

		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = lve_swap_chain->getSwapChainExtent();

		std::array<VkClearValue, 2> clear_values{};
		clear_values[0].color = { 0.05f, 0.05f, 0.05f, 1.0f };
		clear_values[1].depthStencil = { 1.0f, 0 };

		render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
		render_pass_info.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(command_buffers[image_index], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0;
		viewport.y = 0;
		viewport.height = static_cast<float>(lve_swap_chain->getSwapChainExtent().height);
		viewport.width = static_cast<float>(lve_swap_chain->getSwapChainExtent().width);
		viewport.maxDepth = 1.0f;
		viewport.minDepth = 0.0f;

		VkRect2D scissor{ {0, 0}, lve_swap_chain->getSwapChainExtent() };
		vkCmdSetViewport(command_buffers[image_index], 0, 1, &viewport);
		vkCmdSetScissor(command_buffers[image_index], 0, 1, &scissor);

		renderGameObjects(command_buffers[image_index]);


		vkCmdEndRenderPass(command_buffers[image_index]);
		if (vkEndCommandBuffer(command_buffers[image_index]) != VK_SUCCESS)
			throw std::runtime_error("Failed to end command buffer");
	}
	void FirstApp::createPipelineLayout()
	{
		VkPushConstantRange push_constant_range{};
		push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		push_constant_range.offset = 0;
		push_constant_range.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 0;
		pipeline_layout_info.pSetLayouts = nullptr;
		pipeline_layout_info.pushConstantRangeCount = 1;
		pipeline_layout_info.pPushConstantRanges = &push_constant_range;

		if (vkCreatePipelineLayout(lve_device.device(), &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipline layout");
	}
	void FirstApp::createPipeline()
	{
		assert(lve_swap_chain != nullptr && "Cannot create pipeline before swap chain ");
		assert(pipeline_layout != nullptr && "Cannot create pipeline before pipeline layout ");

		PipelineConfigInfo pipeline_config{};
		LvePipeline::defaultPipelineConfigInfo(pipeline_config);

		pipeline_config.renderPass = lve_swap_chain->getRenderPass();
		pipeline_config.pipelineLayout = pipeline_layout;
		lve_pipeline = std::make_unique<LvePipeline>(lve_device, "Shaders\\simple_shader.vert.spv", "Shaders\\simple_shader.frag.spv", pipeline_config);
	}
	void FirstApp::createCommandBuffers()
	{
		command_buffers.resize(lve_swap_chain->imageCount());

		VkCommandBufferAllocateInfo allocate_info{};

		allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocate_info.commandPool = lve_device.getCommandPool();
		allocate_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

		if (vkAllocateCommandBuffers(lve_device.device(), &allocate_info, command_buffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers");

	}
	void FirstApp::freeCommandBuffers()
	{
		vkFreeCommandBuffers(lve_device.device(), lve_device.getCommandPool(), static_cast<uint32_t>(command_buffers.size()), command_buffers.data());
		command_buffers.clear();
	}
	void FirstApp::drawFrame()
	{

		uint32_t image_index;
		auto result = lve_swap_chain->acquireNextImage(&image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
			recreateSwapChain();

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire next image");

		recordCommandBuffer(image_index);

		result = lve_swap_chain->submitCommandBuffers(&command_buffers.at(image_index), &image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lve_window.wasWindowResized())
		{
			lve_window.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to submit command buffers");
	}
	LveWindow* FirstApp::lveWindow()
	{
		return &lve_window;
	}
}