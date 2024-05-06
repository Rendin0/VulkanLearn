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
	void DvdRenerSystem::update(std::vector<LveGameObject>& game_objects, const float& speed)
	{
		for (auto& object : game_objects)
		{
			object.changeDirection(object.transform_2d.translation);

			object.transform_2d.translation += (object.getDirection() * speed);

			object.transform_2d.rotation = atan2(object.transform_2d.translation.y, object.transform_2d.translation.x);

		}
	}
	void DvdRenerSystem::renderGameObjects(VkCommandBuffer command_buffer, std::vector<LveGameObject>& game_objects)
	{
		lve_pipeline->bind(command_buffer);

		for (auto& obj : game_objects)
		{
			SimplePushConstantData push{};
			push.offset = obj.transform_2d.translation;
			push.color = obj.color;
			push.transform = obj.transform_2d.mat2();

			vkCmdPushConstants(command_buffer, pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			obj.model->bind(command_buffer);
			obj.model->draw(command_buffer);
		}
	}
}
