#pragma once

#include "lve_model.hpp"
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

namespace lve
{
	struct Transform
	{
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f, 1.f};
		glm::vec3 rotation;

		glm::mat4 mat4() {
			const float c3 = glm::cos(rotation.z);
			const float s3 = glm::sin(rotation.z);
			const float c2 = glm::cos(rotation.x);
			const float s2 = glm::sin(rotation.x);
			const float c1 = glm::cos(rotation.y);
			const float s1 = glm::sin(rotation.y);
			return glm::mat4{
				{
					scale.x * (c1 * c3 + s1 * s2 * s3),
					scale.x * (c2 * s3),
					scale.x * (c1 * s2 * s3 - c3 * s1),
					0.0f,
				},
				{
					scale.y * (c3 * s1 * s2 - c1 * s3),
					scale.y * (c2 * c3),
					scale.y * (c1 * c3 * s2 + s1 * s3),
					0.0f,
				},
				{
					scale.z * (c2 * s1),
					scale.z * (-s2),
					scale.z * (c1 * c2),
					0.0f,
				},
				{translation.x, translation.y, translation.z, 1.0f} };
		}
	};

	class LveGameObject
	{
	public:
		using id_t = unsigned int;
		static id_t current_id;

		static LveGameObject createGameObject();
		static LveGameObject createGameObject(const LveGameObject& ref);

		id_t getId() const { return id; }

		LveGameObject(const LveGameObject&) = delete;
		LveGameObject& operator=(const LveGameObject&) = delete;
		LveGameObject(LveGameObject&&) = default;
		LveGameObject& operator=(LveGameObject&&) = default;

		Transform transform{};
		std::shared_ptr<LveModel> model{};
		glm::vec3 color{};
		glm::vec3 direction{0.f};

		float speed{};
		float mass{};

	private:
		LveGameObject(id_t obj_id);
		LveGameObject(id_t obj_id, const LveGameObject& ref);

		id_t id;
	};

}
