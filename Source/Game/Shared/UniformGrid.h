#pragma once
#include "../../Utility/Point3.h"
#include <vector>
#include "GameTypes.h"

class UniformGrid
{
	class Element
	{
		// they can fly around
		std::vector<std::shared_ptr<GameEntity>> dynamic;
		// they cant move
		std::vector<std::shared_ptr<GameEntity>> statics;
	};
public:
	UniformGrid() = default;
	// dimension is the dimension of the map grid
	// buffered is the size of the buffered region when something is outside of the map
	void init(Point3S dimension, Point3S buffered)
	{
		m_dim = dimension + buffered * 2;
		m_off = buffered;

		m_data.clear();
		m_data.assign(m_dim.size(),Element());
		m_outside = Element();
	}
	void setEntity(std::shared_ptr<GameEntity> e)
	{
		// static or dynamic Component?
		assert(e);
		assert(e->hasComponent<Transform>());
		assert(e->hasComponent<StaticCollision>() || e->hasComponent<Collision>());

		if(e->hasComponent<Collision>())
		{
			
		}
		else
		{
			// static
		}
	}
private:
	std::vector<Element> m_data;
	Element m_outside;
	Point3S m_dim;
	Point3S m_off;
};
