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
	void MoveRenderSystem::update(std::vector<LveGameObject>& game_objects)
	{
		for (auto& object : game_objects)
		{
			object.transform.translation += (object.direction * object.speed);
		}
	}
}
