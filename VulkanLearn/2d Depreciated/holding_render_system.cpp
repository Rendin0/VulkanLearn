#include "holding_render_system.hpp"

namespace lve
{

	HoldingRenerSystem::HoldingRenerSystem(LveDevice& device, VkRenderPass render_pass)
		: SimpleRenderSystem(device, render_pass)
	{
	}
	HoldingRenerSystem::~HoldingRenerSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}
	void HoldingRenerSystem::update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end)
	{
		glm::vec2 min{};
		glm::vec2 max{};

		for (auto object = begin; object != end; object++)
		{
			min = {  -1.f + (object->transform_2d.scale * .5f) };
			max = { 1.f - (object->transform_2d.scale * .5f) };
			object->transform_2d.translation = glm::clamp(object->transform_2d.translation, min, max);
		}
	}
}
