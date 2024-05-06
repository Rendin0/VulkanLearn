#include "color_render_system.hpp"

namespace lve
{

	ColorRenderSystem::ColorRenderSystem(LveDevice& device, VkRenderPass render_pass)
		: SimpleRenderSystem(device, render_pass)
	{
	}

	ColorRenderSystem::~ColorRenderSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}

	void ColorRenderSystem::update(std::vector<LveGameObject>& game_objects)
	{
		for (auto& object : game_objects)
		{
			object.color.r = (object.transform_2d.translation.x + 1) / 2;
			//object.color.g = (object.transform_2d.translation.x + 1) / 2;
			object.color.b = (object.transform_2d.translation.y + 1) / 2;
		}
	}

}