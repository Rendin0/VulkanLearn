#include "follow_render_system.hpp"

namespace lve
{
	FollowRenderSystem::FollowRenderSystem(LveDevice& device, VkRenderPass render_pass)
		: SimpleRenderSystem(device, render_pass)
	{
	}
	FollowRenderSystem::~FollowRenderSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}
	void FollowRenderSystem::update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end, const float& factor)
	{
		for (auto object = begin + 1; object != end; object++)
		{
			object->transform_2d.translation += (begin->transform_2d.translation - object->transform_2d.translation) * factor;
		}
	}
}
