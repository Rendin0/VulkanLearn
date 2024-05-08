#include "camera_follow_render_system.hpp"

namespace lve
{

	CameraFollowRenderSystem::CameraFollowRenderSystem(LveDevice& device, VkRenderPass render_pass)
		:SimpleRenderSystem(device, render_pass)
	{
	}

	CameraFollowRenderSystem::~CameraFollowRenderSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}

	void CameraFollowRenderSystem::update(std::vector<LveGameObject>& game_objects, LveGameObject& camera)
	{
		for (auto& object : game_objects)
		{
			object.transform.translation += ((camera.transform.translation - object.transform.translation) / 2.f) * 0.01f;
		}
	}


}