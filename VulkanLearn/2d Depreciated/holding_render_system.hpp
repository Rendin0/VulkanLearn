#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class HoldingRenerSystem : public SimpleRenderSystem
	{
	public:
		HoldingRenerSystem(LveDevice& device, VkRenderPass render_pass);
		~HoldingRenerSystem();

		void update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end);
	};
}
