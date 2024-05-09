#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class GravitationRenderSystem : public SimpleRenderSystem
	{
	public:
		GravitationRenderSystem(LveDevice& device, VkRenderPass render_pass);
		~GravitationRenderSystem();

		void update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end, float dt);

	private:
		float dt_factor{ 40.f };
	};
}