#include "scale_render_system.hpp"

namespace lve
{
	ScaleRenderSystem::ScaleRenderSystem(LveDevice& device, VkRenderPass render_pass)
		: SimpleRenderSystem(device, render_pass)
	{
	}

	ScaleRenderSystem::~ScaleRenderSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}

	void ScaleRenderSystem::update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end)
	{
		for (auto object = begin; object != end; object++)
		{
			object->transform_2d.scale = glm::vec2(((1.f - glm::abs(object->transform_2d.translation.x)) * (1.f - glm::abs(object->transform_2d.translation.y))) * 0.5);
		}
	}
}
