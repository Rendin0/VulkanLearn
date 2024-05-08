#include "lve_game_object.hpp"

namespace lve
{


	LveGameObject LveGameObject::createGameObject()
	{
		static id_t current_id = 0;

		return LveGameObject(current_id++);
	}

	LveGameObject::LveGameObject(id_t obj_id)
		: id(obj_id)
	{
	}
}