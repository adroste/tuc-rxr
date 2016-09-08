#pragma once
#include "../UIContainer.h"

class UIContainerBucket : public UIContainer
{
	class DescBox : public UIButton
	{
	public:
		DescBox(const CubeDesc& c)
			: UIButton(Style::Royal),
			m_cd(c)
		{}
		virtual ~DescBox(){}
		virtual void draw(Drawing& draw) override
		{
			draw.rect(RectF(m_pos, m_pos + PointF(m_dim.x, m_dim.y / 2.0f)), Color(m_cd.diffuse));
			draw.rect(RectF(m_pos + PointF(0.0f,m_dim.y / 2.0f), m_pos + m_dim), Color(m_cd.spec));
		}
		const CubeDesc& getCubeDesc() const
		{
			return m_cd;
		}
	private:
		const CubeDesc m_cd;
	};
public:
	UIContainerBucket()
		:
		UIContainer(true)
	{}
	void addMaterial(const CubeDesc& c)
	{
		LockGuard g(m_muCubes);

		m_cubes.push_back(std::unique_ptr<DescBox>(new DescBox(c)));
	}
	void removeMaterial(size_t index)
	{
		if(index < m_cubes.size())
		{
			LockGuard g(m_muCubes);

			m_cubes.erase(m_cubes.begin() + index);
		}
	}
private:
	std::vector<std::unique_ptr<DescBox>> m_cubes;
	Mutex m_muCubes;
};
