#include "lve_game_object.hpp"

namespace lve
{
	LveGameObject LveGameObject::createGameObject()
	{
		static id_t current_id = 0;

		return LveGameObject(current_id++);
	}
	void LveGameObject::changeDirection(glm::vec2 changes)
	{
		int cut_zero[2] = {glm::abs(changes.x) + (transform_2d.scale.x * .5f), glm::abs(changes.y) + (transform_2d.scale.y * .5f)};

		cut_zero[0] = (cut_zero[0] * -2) + 1;
		cut_zero[1] = (cut_zero[1] * -2) + 1;

		direction *= glm::vec2{ cut_zero[0], cut_zero[1] };
	}

	glm::vec2 LveGameObject::getDirection()
	{
		return direction;
	}

	LveGameObject::LveGameObject(id_t obj_id)
		: id(obj_id)
	{
	}
}