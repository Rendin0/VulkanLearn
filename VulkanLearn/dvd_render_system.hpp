#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class DvdRenerSystem : public SimpleRenderSystem
	{
	public:
		DvdRenerSystem(LveDevice& device, VkRenderPass render_pass);
		~DvdRenerSystem();

		void update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end, const float& speed);
	};
}