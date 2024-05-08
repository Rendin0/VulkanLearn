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

	glm::vec3 directionVector(const glm::vec3& point1, const glm::vec3& point2, const float& repulsionRadius, const float& factor)
	{
		glm::vec3 vector = point2 - point1;

		float distance = glm::length(vector);

		float coefficient = glm::tanh((distance - repulsionRadius) / factor);

		return vector * (1.0f - coefficient);
	}

	void PushbackRenderSystem::update(std::vector<LveGameObject>& game_objects, const float& factor)
	{
		for (auto& object : game_objects)
		{
			float object_radius = object.transform.scale.x * .5f;
			for (auto& sub_object : game_objects) // all sub objects before object 
			{
				if (&sub_object == &object)
				{
					continue;
				}

				glm::vec3 dir_vec = directionVector(object.transform.translation, sub_object.transform.translation, object_radius, factor);
				//std::cout << dir_vec.x << ";" << dir_vec.y << std::endl;
				object.transform.translation -= dir_vec;
			}
		}
	}



}