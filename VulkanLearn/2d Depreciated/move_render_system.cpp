#include "move_render_system.hpp"

namespace lve
{
	MoveRenderSystem::MoveRenderSystem(LveDevice& device, VkRenderPass render_pass)
		: SimpleRenderSystem(device, render_pass)
	{
	}
	MoveRenderSystem::~MoveRenderSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}
	void MoveRenderSystem::update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end)
	{
		for (auto object = begin; object != end; object++)
		{
			object->transform_2d.translation += (object->direction * object->speed);

			//object->speed -= object->speed * 0.001f;

			object->transform_2d.rotation = atan2(object->transform_2d.translation.y, object->transform_2d.translation.x);
		}
	}
}
