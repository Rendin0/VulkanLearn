#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class PushbackRenderSystem : public SimpleRenderSystem
	{
	public:
		PushbackRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~PushbackRenderSystem();

		void update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end, const float& factor);
	};
}