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

	glm::vec3 gravityVector(LveGameObject& obj1, LveGameObject& obj2, const double& gravity)
	{
		glm::vec3 vector = obj1.transform.translation - obj2.transform.translation;

		float force = gravity * ((obj1.mass * obj2.mass) / glm::length(vector));

		return vector * force;
	}

	void GravitationRenderSystem::update(std::vector<LveGameObject>& game_objects)
	{
		const double GRAVITY = 6.6742E-2;

		for (auto& object : game_objects)
		{
			float object_radius = object.transform.scale.x * .5f;
			for (auto& sub_object : game_objects) // all sub objects before object 
			{
				if (&sub_object == &object)
				{
					continue;
				}
				sub_object.direction += gravityVector(object, sub_object, GRAVITY);

			}
		}
	}
}
