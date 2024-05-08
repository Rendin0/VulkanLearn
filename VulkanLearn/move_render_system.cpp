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
			object->transform.translation += (object->direction * object->speed);
			object->transform.rotation = -object->direction;
		}
	}
}
