#pragma once
#include "simple_render_system.hpp"

namespace lve
{
	class DvdRenerSystem : public SimpleRenderSystem
	{
	public:
		DvdRenerSystem(LveDevice& device, VkRenderPass render_pass);
		~DvdRenerSystem();

		void update(std::vector<LveGameObject>& game_objects);

		void renderGameObjects(VkCommandBuffer command_buffer, std::vector<LveGameObject>& game_objects) override;
	};
}