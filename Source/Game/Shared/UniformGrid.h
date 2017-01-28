#pragma once
#include "../../Utility/Point3.h"
#include <vector>
#include "GameTypes.h"
#include "../../Framework/OpenGL/Drawing.h"

class UniformGrid
{
	struct StaticEntity
	{
		// precaculated aabox for faster lookup
		AABox aabox;
		// corresponding entity
		std::shared_ptr<GameEntity> ent;
	};
	struct Element
	{
		// they can fly around
		std::vector<std::shared_ptr<GameEntity>> dynamic;
		// they cant move
		std::vector<StaticEntity> statics;
	};
public:
	UniformGrid() = default;
	// dimension is the dimension of the map grid
	// buffered is the size of the buffered region when something is outside of the map
	void init(Point3S dimension, Point3S buffered)
	{
		m_dim = dimension + buffered * 2;
		m_off = buffered;
		m_min = -Point3I(buffered);
		m_max = dimension + buffered;

		m_data.clear();
		m_data.assign(m_dim.size(),Element());
		m_outside = Element();
	}
	// for debugging
	void draw(Drawing& draw)
	{
		for(const auto& e : m_data)
		{
			for(const auto& s : e.statics)
			{
				draw.lineBox(s.aabox.p1, s.aabox.p2, Color::Blue());
			}
		}
		for(const auto& s : m_outside.statics)
		{
			draw.lineBox(s.aabox.p1, s.aabox.p2, Color::Green());
		}
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
			StaticEntity s;
			s.ent = e;
			s.aabox = e->getComponent<StaticCollision>().aabox.transform(e->getComponent<Transform>().getMatrix());
			forColliding(s.aabox, [s](Element& e)
			{
				e.statics.push_back(s);
			});
		}
	}
	bool hasCollision(const AABox& aabox)
	{
		bool hasCol = false;
		forColliding(aabox,[&hasCol,&aabox](Element& e)
		{
			// test collission with objects
			for(const auto& i : e.statics)
			{
				if(aabox.intersectWith(i.aabox))
				{
					hasCol = true;
					return;
				}
			}
		});
		return hasCol;
	}
private:
	void forColliding(const AABox& aabox, std::function<void(Element&)> func)
	{
		// go through all colliding boxes
		bool m_out = false;
		int minx = int(aabox.xmin + 0.5f);
		int miny = int(aabox.ymin + 0.5f);
		int minz = int(aabox.zmin + 0.5f);
		int maxx = int(aabox.xmax + 0.5f);
		int maxy = int(aabox.ymax + 0.5f);
		int maxz = int(aabox.zmax + 0.5f);

		for(auto x = minx; x <= maxx; x++)
		{
			for (auto y = miny; y <= maxy; y++)
			{
				for (auto z = minz; z <= maxz; z++)
				{
					int res = toIndex(x,y,z);
					if(res == -1)
					{
						if(!m_out)
						{
							m_out = true;
							func(m_outside);
						}
					}
					else func(m_data[res]);
				}
			}
		}
	}
	int toIndex(int x, int y, int z) const
	{
		if(x < m_min.x || y < m_min.x || z < m_min.z
			|| x >= m_max.x || y >= m_max.y || z >= m_max.z)
		{
			return -1;
		}

		return m_dim.calcIndex(Point3S(Point3I(x, y, z) + Point3I(m_off)));
	}
private:
	std::vector<Element> m_data;
	Element m_outside;
	Point3S m_dim;
	Point3S m_off;
	Point3I m_min;
	Point3I m_max;
};
