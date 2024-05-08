#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class PushbackRenderSystem : public SimpleRenderSystem
	{
	public:
		PushbackRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~PushbackRenderSystem();

		void update(std::vector<LveGameObject>& game_objects, const float& factor);
	};

}