#include "dvd_render_system.hpp"

namespace lve
{
	DvdRenerSystem::DvdRenerSystem(LveDevice& device, VkRenderPass render_pass)
		: SimpleRenderSystem(device, render_pass)
	{
	}
	DvdRenerSystem::~DvdRenerSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}
	void DvdRenerSystem::update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end, const float& speed)
	{
		for (auto object = begin; object != end; object++)
		{
			object->changeDirection(object->transform_2d.translation);

			object->transform_2d.translation += (object->getDirection() * speed);

			object->transform_2d.rotation = atan2(object->transform_2d.translation.y, object->transform_2d.translation.x);
		}
	}
}
