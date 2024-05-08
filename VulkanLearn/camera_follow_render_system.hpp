#pragma once

#include "simple_render_system.hpp"

namespace lve
{
	class CameraFollowRenderSystem : public SimpleRenderSystem
	{
	public:
		CameraFollowRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~CameraFollowRenderSystem();

		void update(std::vector<LveGameObject>& game_objects, LveGameObject& camera);
	};

}