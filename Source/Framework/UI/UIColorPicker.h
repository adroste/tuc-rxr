#pragma once
#include "UIObject.h"
#include <math.h>
#include "Interfaces/IValueHolder.h"

class UIColorPicker : public UIObject, public IValueHolder<Color>
{
public:
	UIColorPicker()
		:
		IValueHolder(Color::Red())
	{
		UIColorPicker::setDim(300.0f);
		calcSquarePick(getMidpoint());
	}

	virtual ~UIColorPicker()
	{}

	void draw(Drawing& draw) override
	{
		float r = getDim().x / 2.0f;
		PointF mid = getMidpoint();
		draw.hsvPicker(getMidpoint(), r, m_hueColor);
		draw.disc(m_pick, 7.0f, Color::White());
		draw.disc(m_pick, 5.5f, getValue());
		
		PointF sel(1.0f, 0.0f);
		sel = sel.rotate(-m_angle);
		draw.line(sel * r * 0.8f + mid, sel * r + mid, 5.0f, Color::White());
	}

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		if (button == Input::Mouse::Left)
		{
			// outer circle
			float r = getDim().x / 2.0f;
			PointF p = ((mpos - getOrigin()) / r) - PointF(1.0f, 1.0f);
			float len = p.length();

			if (len <= 1.0f && len >= 0.8f)
			{
				m_isMouseLeftDownCircle = true;
				calcCircleAngle(mpos);
				return true;
			}

			// inner square
			PointF mid = getMidpoint();
			float d = r * 0.75f * 0.707107f;
			PointF d2(d, d);

			RectF rect = RectF(mid - d2, mid + d2);
			if (rect.isPointInside(mpos))
			{
				m_isMouseLeftDownSquare = true;
				calcSquarePick(mpos);
				return true;
			}
		}
		return false;
	}
	
	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		m_isMouseLeftDownCircle = false;
		m_isMouseLeftDownSquare = false;
		return false;
	}

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		if (!handled)
		{
			if (m_isMouseLeftDownCircle)
			{
				calcCircleAngle(mpos);
				return true;
			}
			if (m_isMouseLeftDownSquare)
			{
				calcSquarePick(mpos);
				return true;
			}
		}
		return false;
	}

	virtual void setDim(float d)
	{
		UIObject::setDim({ d, d });
	}

	virtual void setValue(const Color& color) override
	{
		// convert rgb to hsv
		float r = color.r, g = color.g, b = color.b;
		float K = 0.0f;

		if (g < b)
		{
			std::swap(g, b);
			K = -1.0f;
		}

		if (r < g)
		{
			std::swap(r, g);
			K = -2.0f / 6.0f - K;
		}

		float chroma = r - std::min(g, b);
		float h = fabs(K + (g - b) / (6.0f * chroma + 1e-20f));

		m_saturation = tool::clamp(chroma / (r + 1e-20f), 0.0f, 1.0f);
		m_value = tool::clamp(r, 0.0f, 1.0f);
		m_angle = h * float(M_PI) * 2.0f;

		// set pick
		float radius = getDim().x / 2.0f;
		PointF mid = getMidpoint();
		float d = radius * 0.75f * 0.707107f;
		PointF d2(d, d);
		RectF rect = RectF(mid - d2, mid + d2);
		m_pick.x = (rect.x2 - rect.x1) * m_saturation + rect.x1;
		m_pick.y = (rect.y2 - rect.y1) * (1.0f - m_value) + rect.y1;
		
		calcColor();
	}

private:
	void calcCircleAngle(const PointF& mpos)
	{
		// p: [-1.0f; 1.0f]
		float r = getDim().x / 2.0f;
		PointF p = ((mpos - getOrigin()) / r) - PointF(1.0f, 1.0f);

		PointF pnorm = p.normalize();
		float angle = acos(pnorm * PointF(1.0f, 0.0f));
		if (pnorm * PointF(0.0f, 1.0f) > 0.0f)
			angle = float(M_PI) * 2.0f - angle;
		setAngle(angle);
	}
	
	void calcSquarePick(const PointF& mpos)
	{
		float r = getDim().x / 2.0f;
		PointF mid = getMidpoint();
		float d = r * 0.75f * 0.707107f;
		PointF d2(d, d);

		RectF rect = RectF(mid - d2, mid + d2);

		if (mpos.x < rect.x1)
			m_pick.x = rect.x1;
		else if (mpos.x > rect.x2)
			m_pick.x = rect.x2;
		else
			m_pick.x = mpos.x;

		if (mpos.y < rect.y1)
			m_pick.y = rect.y1;
		else if (mpos.y > rect.y2)
			m_pick.y = rect.y2;
		else
			m_pick.y = mpos.y;

		// calc saturation & value
		float difx = rect.x2 - rect.x1;
		float dify = rect.y2 - rect.y1;
		float pickx = m_pick.x - rect.x1;
		float picky = m_pick.y - rect.y1;

		m_saturation = tool::clamp(pickx / difx, 0.0f, 1.0f);
		m_value = tool::clamp(1.0f - picky / dify, 0.0f, 1.0f);

		calcColor();
	}

	void calcColor()
	{
		float hue = m_angle / (float(M_PI) * 2.0f);
		// angle to hue
		m_hueColor.r = tool::clamp(abs(6.0f * hue - 3.0f) - 1.0f, 0.0f, 1.0f);
		m_hueColor.g = tool::clamp(2.0f - abs(6.0f * hue - 2.0f), 0.0f, 1.0f);
		m_hueColor.b = tool::clamp(2.0f - abs(6.0f * hue - 4.0f), 0.0f, 1.0f);

		// hsv to rgb
		Color c;
		c.r = ((m_hueColor.r - 1.0f) * m_saturation + 1.0f) * m_value;
		c.g = ((m_hueColor.g - 1.0f) * m_saturation + 1.0f) * m_value;
		c.b = ((m_hueColor.b - 1.0f) * m_saturation + 1.0f) * m_value;

		IValueHolder::setValue(c);
	}

	void setAngle(float angle)
	{
		m_angle = angle;
		calcColor();
	}

private:
	bool m_isMouseLeftDownCircle = false;
	bool m_isMouseLeftDownSquare = false;

	Color m_hueColor = Color::Red();
	float m_angle = 0.0f;
	PointF m_pick;
	float m_saturation = 0.0f;
	float m_value = 0.0f;
};
