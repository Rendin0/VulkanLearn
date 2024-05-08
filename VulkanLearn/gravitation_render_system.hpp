#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class GravitationRenderSystem : public SimpleRenderSystem
	{
	public:
		GravitationRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~GravitationRenderSystem();

		void update(std::vector<LveGameObject>& game_objects);
	};
}