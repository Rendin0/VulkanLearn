#include "lve_game_object.hpp"

namespace lve
{
	LveGameObject::id_t LveGameObject::current_id = 0;

	LveGameObject LveGameObject::createGameObject()
	{
		return LveGameObject(current_id++);
	}

	LveGameObject LveGameObject::createGameObject(const LveGameObject& ref)
	{
		return LveGameObject(current_id++, ref);
	}

	LveGameObject::LveGameObject(id_t obj_id)
		: id(obj_id)
	{
	}
	LveGameObject::LveGameObject(id_t obj_id, const LveGameObject& ref)
		: id(obj_id)
	{
		transform = ref.transform;
		model = ref.model;
		mass = ref.mass;
		color = ref.color;
		direction = ref.direction;
		speed = ref.speed;
	}
}