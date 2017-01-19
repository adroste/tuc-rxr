#include "MovementSystem.h"

MovementSystem::MovementSystem(UniformGrid& grid)
	:
	m_grid(grid)
{
}

void MovementSystem::initQueries(ManagerT & m)
{
	m.addQuery<Transform, Movement>();
}

void MovementSystem::tick(float dt)
{
	getManager().forEach<Transform,Movement>([dt,this](GameEntity& e)
	{
		e.getComponent<Movement>().velocity += e.getComponent<Movement>().acceleration * dt;
		if(e.hasComponent<Collision>())
		{
			if (!m_grid.hasCollision(e.getComponent<Collision>().aabox.transform(e.getComponent<Transform>().getMatrix())))
			{
				e.getComponent<Transform>().pos += e.getComponent<Movement>().velocity * dt;
			}
			else
			{
				e.getComponent<Movement>().velocity = glm::vec3(0.0f);
			}
		}
		else e.getComponent<Transform>().pos += e.getComponent<Movement>().velocity * dt;
	});
}

void MovementSystem::onEntitySpawn(std::shared_ptr<EntityT> e)
{
	if(e->hasComponents<Transform,Collision>())
	{
		m_grid.setEntity(e);
	}
	else if(e->hasComponents<Transform,StaticCollision>())
	{
		m_grid.setEntity(e);
	}
}

void MovementSystem::onEntityDeath(std::shared_ptr<EntityT> e)
{

}
