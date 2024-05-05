#pragma once
#include "lve_pipeline.hpp"
#include "lve_device.hpp"
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
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		void renderGameObjects(VkCommandBuffer command_buffer, std::vector<LveGameObject>& game_objects);
	private:

		void createPipelineLayout();
		void createPipeline(VkRenderPass render_pass);

		LveDevice& lve_device;
		std::vector<LveModel::Vertex> vertices;

		std::unique_ptr<LvePipeline> lve_pipeline;
		VkPipelineLayout pipeline_layout;
	};
}