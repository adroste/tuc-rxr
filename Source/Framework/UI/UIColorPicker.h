#pragma once
#include "UIObject.h"
#include <math.h>

class UIColorPicker : public UIObject
{
public:
	UIColorPicker()
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
		draw.hsvPicker({ r, r }, r, m_hueColor);
		PointF o(2.0f, 2.0f);
		draw.rect(RectF(m_pick - o, m_pick + o), Color::White());
		
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
			}

			// inner square
			PointF mid = getMidpoint();
			float d = r * 0.8f * 0.707107f;
			PointF d2(d, d);

			RectF rect = RectF(mid - d2, mid + d2);
			if (rect.isPointInside(mpos))
			{
				m_isMouseLeftDownSquare = true;
				calcSquarePick(mpos);
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

	virtual bool mouseMove(const PointF& mpos, bool handled) override
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

	Color getColor() const
	{
		return m_color;
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
		float d = r * 0.8f * 0.707107f;
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
		m_hueColor.r = tool::clamp(abs(6.0f * hue - 3.0f) - 1.0f, 0.0f, 1.0f);
		m_hueColor.g = tool::clamp(2.0f - abs(6.0f * hue - 2.0f), 0.0f, 1.0f);
		m_hueColor.b = tool::clamp(2.0f - abs(6.0f * hue - 4.0f), 0.0f, 1.0f);

		m_color.r = ((m_hueColor.r - 1.0f) * m_saturation + 1.0f) * m_value;
		m_color.g = ((m_hueColor.g - 1.0f) * m_saturation + 1.0f) * m_value;
		m_color.b = ((m_hueColor.b - 1.0f) * m_saturation + 1.0f) * m_value;
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
	Color m_color = Color::Red();
	float m_angle = 0.0f;
	PointF m_pick;
	float m_saturation = 0.0f;
	float m_value = 0.0f;
};
