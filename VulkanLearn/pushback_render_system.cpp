#include "pushback_render_system.hpp"

namespace lve
{
	PushbackRenderSystem::PushbackRenderSystem(LveDevice& device, VkRenderPass render_pass)
		: SimpleRenderSystem(device, render_pass)
	{
	}
	PushbackRenderSystem::~PushbackRenderSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}

	void PushbackRenderSystem::update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end, const float& factor)
	{

		for (auto object = begin; object != end; object++)
		{
			glm::vec2 pushback_factor{ 0.f };

			for (auto sub_object = begin; sub_object != object; sub_object++) // all sub objects before object 
			{
				pushback_factor.x += glm::pow(sub_object->transform_2d.translation.x + 1 - object->transform_2d.translation.x + 1, 2.f) / 2 - 1;
				pushback_factor.y += glm::pow(sub_object->transform_2d.translation.y + 1 - object->transform_2d.translation.y + 1, 2.f) / 2 - 1;
			}

			for (auto sub_object = object + 1; sub_object != end; sub_object++) // all sub objects after object
			{
				pushback_factor.x += glm::pow(sub_object->transform_2d.translation.x + 1- object->transform_2d.translation.x + 1, 2.f) / 2 - 1;
				pushback_factor.y += glm::pow(sub_object->transform_2d.translation.y + 1- object->transform_2d.translation.y + 1, 2.f) / 2 - 1;
			}

			object->transform_2d.translation -= pushback_factor * factor;
		}

	}

}
