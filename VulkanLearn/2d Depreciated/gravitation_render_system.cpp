#include "gravitation_render_system.hpp"

namespace lve
{
	GravitationRenderSystem::GravitationRenderSystem(LveDevice& device, VkRenderPass render_pass)
		: SimpleRenderSystem(device, render_pass)
	{
	}
	GravitationRenderSystem::~GravitationRenderSystem()
	{
		vkDestroyPipelineLayout(lve_device.device(), pipeline_layout, nullptr);
	}

	glm::vec2 gravityVector(std::vector<LveGameObject>::iterator obj1, std::vector<LveGameObject>::iterator obj2, const double& gravity)
	{
		glm::vec2 vector = obj1->transform_2d.translation - obj2->transform_2d.translation;

		float force = gravity * ((obj1->mass * obj2->mass) / glm::length(vector));

		return vector * force;
	}

	void GravitationRenderSystem::update(const std::vector<LveGameObject>::iterator& begin, const std::vector<LveGameObject>::iterator& end)
	{
		const double GRAVITY = 6.6742E-5;

		for (auto object = begin; object != end; object++)
		{
			for (auto sub_object = begin; sub_object != object; sub_object++) // all sub objects before object 
			{
				sub_object->direction += gravityVector(object, sub_object, GRAVITY);
			}

			for (auto sub_object = object + 1; sub_object != end; sub_object++) // all sub objects after object
			{
				sub_object->direction += gravityVector(object, sub_object, GRAVITY);
			}
		}
	}
}
