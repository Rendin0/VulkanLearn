#include <stdexcept>
#include <array>
#include <conio.h>
#include <iostream>
#include <thread>
#include <ctime>
#include "simple_render_system.hpp"
#include <memory>


namespace lve
{
	void SimpleRenderSystem::renderGameObjects(VkCommandBuffer command_buffer, std::vector<LveGameObject>& game_objects, const LveCamera& lve_camera)
	{
		lve_pipeline->bind(command_buffer);

		auto projection_view = lve_camera.getProjectionMatrix() * lve_camera.getView();

		for (auto& obj : game_objects)
		{
			SimplePushConstantData push{};
			push.color = obj.color;
			push.transform = projection_view * obj.transform.mat4();

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

	//std::unique_ptr<LveModel> SimpleRenderSystem::createTriangleModel(LveDevice& lve_device)
	//{
	//	std::vector<LveModel::Vertex> vertices =
	//	{
	//		{{0.0f, -0.5f}},
	//		{{0.5f, 0.5f}},
	//		{{-0.5f, 0.5f}}
	//	};
	//	return std::make_unique<LveModel>(lve_device, vertices);
	//}

	//std::unique_ptr<LveModel> SimpleRenderSystem::createSquareModel(LveDevice& lve_device)
	//{
	//	std::vector<LveModel::Vertex> vertices =
	//	{
	//		{{-0.5f, -0.5f}},
	//		{{0.5f, 0.5f}},
	//		{{-0.5f, 0.5f}},
	//		{{-0.5f, -0.5f}},
	//		{{0.5f, -0.5f}},
	//		{{0.5f, 0.5f}},
	//	};
	//	return std::make_unique<LveModel>(lve_device, vertices);
	//}

	//std::unique_ptr<LveModel> SimpleRenderSystem::createCircleModel(LveDevice& device, unsigned int numSides)
	//{
	//	std::vector<LveModel::Vertex> uniqueVertices{};
	//	for (int i = 0; i < numSides; i++) {
	//		float angle = i * glm::two_pi<float>() / numSides;
	//		uniqueVertices.push_back({ {glm::cos(angle), glm::sin(angle)} });
	//	}
	//	uniqueVertices.push_back({});  // adds center vertex at 0, 0
	//	std::vector<LveModel::Vertex> vertices{};
	//	for (int i = 0; i < numSides; i++) {
	//		vertices.push_back(uniqueVertices[i]);
	//		vertices.push_back(uniqueVertices[(i + 1) % numSides]);
	//		vertices.push_back(uniqueVertices[numSides]);
	//	}
	//	return std::make_unique<LveModel>(device, vertices);
	//}

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