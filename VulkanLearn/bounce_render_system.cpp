#include "bounce_render_system.hpp"

namespace lve
{

	BounceRenderSystem::BounceRenderSystem(LveDevice& device, VkRenderPass render_pass)
		: SimpleRenderSystem(device, render_pass)
	{
	}

	BounceRenderSystem::~BounceRenderSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}

	void BounceRenderSystem::update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end)
	{
		for (auto object = begin; object != end; object++)
		{
			object->changeDirection(object->transform_2d.translation);
		}
	}


}