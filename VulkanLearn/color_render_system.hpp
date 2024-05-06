#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class ColorRenderSystem : public SimpleRenderSystem
	{
	public:
		ColorRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~ColorRenderSystem();

		void update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end);
	};
}