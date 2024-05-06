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
	struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~SimpleRenderSystem();

		static std::unique_ptr<LveModel> createTriangleModel(LveDevice& lve_device);
		static std::unique_ptr<LveModel> createSquareModel(LveDevice& lve_device);
		static std::unique_ptr<LveModel> createCircleModel(LveDevice& device, unsigned int numSides);

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		void renderGameObjects(VkCommandBuffer command_buffer, std::vector<LveGameObject>& game_objects);
	protected:

		void createPipelineLayout();
		void createPipeline(VkRenderPass render_pass);

		LveDevice& lve_device;

		std::unique_ptr<LvePipeline> lve_pipeline;
		VkPipelineLayout pipeline_layout;
	};
}