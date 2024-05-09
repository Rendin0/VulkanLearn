#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class MoveRenderSystem : public SimpleRenderSystem
	{
	public:
		MoveRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~MoveRenderSystem();

		void update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end, float dt);

	private:
		float dt_factor{ 40.f };
	};
}