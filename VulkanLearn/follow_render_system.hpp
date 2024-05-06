#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class FollowRenderSystem : public SimpleRenderSystem
	{
	public:
		FollowRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~FollowRenderSystem();

		void update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end, const float& factor);
	};
}