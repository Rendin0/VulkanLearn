#pragma once

#include "simple_render_system.hpp"

namespace lve
{
	class BounceRenderSystem : public SimpleRenderSystem
	{
	public:
		BounceRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~BounceRenderSystem();

		void update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end);
	};

}