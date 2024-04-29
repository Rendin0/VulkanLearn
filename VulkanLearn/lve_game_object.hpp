#pragma once

#include "lve_model.hpp"
#include <memory>

namespace lve
{
	struct Transform2dComponent
	{
		glm::vec2 translation{};
		glm::vec2 scale{1.f, 1.f};
		float rotation;

		glm::mat2 mat2()
		{
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotation_mat{ { c, s }, {-s, c} };

			glm::mat2 scale_mat{ {scale.x, 0.f}, {0.f, scale.y} };
			return rotation_mat * scale_mat;
		}

	};

	class LveGameObject
	{
	public:
		using id_t = unsigned int;

		static LveGameObject createGameObject();

		id_t getId() const { return id; }

		LveGameObject(const LveGameObject&) = delete;
		LveGameObject& operator=(const LveGameObject&) = delete;
		LveGameObject(LveGameObject&&) = default;
		LveGameObject& operator=(LveGameObject&&) = default;

		Transform2dComponent transform_2d{};
		std::shared_ptr<LveModel> model{};
		glm::vec3 color{};

	private:
		LveGameObject(id_t obj_id);



		id_t id;
	};

}
