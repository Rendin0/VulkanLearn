#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class ScaleRenderSystem : public SimpleRenderSystem
	{
	public:
		ScaleRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~ScaleRenderSystem();

		void update(std::vector<LveGameObject>& game_objects);
	};

}