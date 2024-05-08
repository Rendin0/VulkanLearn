#include "pushback_render_system.hpp"
#include <iostream>

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

	glm::vec2 directionVector(const glm::vec2& point1, const glm::vec2& point2, const float& repulsionRadius, const float& factor)
	{
		glm::vec2 vector = point2 - point1;

		float distance = glm::length(vector);

		float coefficient = glm::tanh((distance - repulsionRadius) / factor);

		return vector * (1.0f - coefficient);
	}

	void PushbackRenderSystem::update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end, const float& factor)
	{
		for (auto object = begin; object != end; object++)
		{
			float object_radius = object->transform_2d.scale.x * .5f;
			for (auto sub_object = begin; sub_object != object; sub_object++) // all sub objects before object 
			{
				glm::vec2 dir_vec = directionVector(object->transform_2d.translation, sub_object->transform_2d.translation, object_radius, factor);
				//std::cout << dir_vec.x << ";" << dir_vec.y << std::endl;
				object->transform_2d.translation -= dir_vec;
			}

			for (auto sub_object = object + 1; sub_object != end; sub_object++) // all sub objects after object
			{
				glm::vec2 dir_vec = directionVector(object->transform_2d.translation, sub_object->transform_2d.translation, object_radius, factor);
				//std::cout << dir_vec.x << ";" << dir_vec.y << std::endl;
				object->transform_2d.translation -= dir_vec;
			}
		}
	}
}
