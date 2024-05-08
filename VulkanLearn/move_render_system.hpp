#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class MoveRenderSystem : public SimpleRenderSystem
	{
	public:
		MoveRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~MoveRenderSystem();

		void update(std::vector<LveGameObject>& game_objects);
	};
}