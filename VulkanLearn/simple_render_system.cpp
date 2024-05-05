#include <stdexcept>
#include <array>
#include <conio.h>
#include <iostream>
#include <thread>
#include <ctime>
#include "simple_render_system.hpp"


namespace lve
{
	struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	void SimpleRenderSystem::renderGameObjects(VkCommandBuffer command_buffer, std::vector<LveGameObject>& game_objects)
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

	SimpleRenderSystem::SimpleRenderSystem(LveDevice& device, VkRenderPass render_pass)
		: lve_device(device)
	{
		createPipelineLayout();
		createPipeline(render_pass);
	}
	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}


	void SimpleRenderSystem::createPipelineLayout()
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

	void SimpleRenderSystem::createPipeline(VkRenderPass render_pass)
	{
		assert(pipeline_layout != nullptr && "Cannot create pipeline before pipeline layout ");

		PipelineConfigInfo pipeline_config{};
		LvePipeline::defaultPipelineConfigInfo(pipeline_config);

		pipeline_config.renderPass = render_pass;
		pipeline_config.pipelineLayout = pipeline_layout;
		lve_pipeline = std::make_unique<LvePipeline>(lve_device, "Shaders\\simple_shader.vert.spv", "Shaders\\simple_shader.frag.spv", pipeline_config);
	}

}