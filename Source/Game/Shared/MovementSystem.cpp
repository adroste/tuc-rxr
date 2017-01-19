#include "MovementSystem.h"

void MovementSystem::initQueries(ManagerT & m)
{
	m.addQuery<Transform, Movement>();
}

void MovementSystem::tick(float dt)
{
	getManager().forEach<Transform,Movement>([dt](GameEntity& e)
	{
		e.getComponent<Movement>().velocity += e.getComponent<Movement>().acceleration * dt;
		e.getComponent<Transform>().pos += e.getComponent<Movement>().velocity * dt;
	});
}
