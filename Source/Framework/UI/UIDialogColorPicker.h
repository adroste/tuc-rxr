#pragma once
#include "UIDialog.h"
#include "UIColorPicker.h"
#include "UIObjectList.h"
#include "UINumUpDownInt.h"
#include "UILabel.h"

class UIDialogColorPicker : public UIDialog
{
public:
	UIDialogColorPicker()
		:
		m_uiObjects({ 
		&m_colorPicker, &m_btnOk, &m_btnCancel, &m_numR, &m_numG, &m_numB, &m_lblR, &m_lblG, &m_lblB
	}),
		m_btnOk(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "OK"),
		m_btnCancel(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Cancel"),
		m_numR(Drawing::getFont(Font::Style::Text, Font::Size::M), 0, 0, 255, 1),
		m_numG(Drawing::getFont(Font::Style::Text, Font::Size::M), 0, 0, 255, 1),
		m_numB(Drawing::getFont(Font::Style::Text, Font::Size::M), 0, 0, 255, 1),
		m_lblR(Drawing::getFont(Font::Style::Text, Font::Size::M), "R:"),
		m_lblG(Drawing::getFont(Font::Style::Text, Font::Size::M), "G:"),
		m_lblB(Drawing::getFont(Font::Style::Text, Font::Size::M), "B:")
	{
		UIDialog::setDim({ 320.0f, 450.0f });
		setFixedDim(true);
		
		m_colorPicker.setOrigin({ 10.0f, 10.0f });
		m_colorPicker.setZIndex(1);

		m_btnCancel.adjustToFontHeadline();
		m_btnCancel.setOrigin(UIDialogColorPicker::getDim() - m_btnCancel.getDim() - PointF(10.0f));
		m_btnOk.adjustToFontHeadline();
		m_btnOk.setOrigin(UIDialogColorPicker::getDim() - PointF({ m_btnCancel.getDim().x + 10.0f, 0.0f}) - m_btnOk.getDim() - PointF(10.0f));

		m_numR.setOrigin({ 30.0f, 330.0f });
		m_numR.setDim({ 60.0f, 40.0f });
		m_lblR.setOrigin({ 10.0f, 335.0f });
		m_numG.setOrigin({ 120.0f, 330.0f });
		m_numG.setDim({ 60.0f, 40.0f });
		m_lblG.setOrigin({ 100.0f, 335.0f });
		m_numB.setOrigin({ 210.0f, 330.0f });
		m_numB.setDim({ 60.0f, 40.0f });
		m_lblB.setOrigin({ 190.0f, 335.0f });

		m_uiObjects.registerAll(this);

		// Events
		m_btnCancel.setOnClickCallback([this](IClickable*)
		{
			setResult(Result::Cancel);
		});

		m_btnOk.setOnClickCallback([this](IClickable*)
		{
			m_color = m_colorPicker.getColor();
			setResult(Result::OK);
		});

		m_colorPicker.setOnColorChangeCallback([this](UIColorPicker*) { onColorChange(); });

		m_numR.setOnValueCallback([this](IValueHolder<int>*) { onColorChangeNum(); });
		m_numG.setOnValueCallback([this](IValueHolder<int>*) { onColorChangeNum(); });
		m_numB.setOnValueCallback([this](IValueHolder<int>*) { onColorChangeNum(); });
	}

	virtual ~UIDialogColorPicker() override
	{
		m_colorPicker.unregisterMe();
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;
		
		UIDialog::draw(draw);

		pushDrawTransform(draw);
		m_uiObjects.draw(draw);
		
		// color box
		draw.rect(RectF(280.0f, 330.0f, 310.0f, 370.0f), m_colorPicker.getColor());

		popDrawTransform(draw);
	}

	using UIDialog::show;
	virtual void show(const Color& color)
	{
		setColor(color);
		show();
	}

	void setColor(const Color& color)
	{
		m_color = color;
		m_colorPicker.setColor(m_color);
	}

	Color getColor() const
	{
		return m_color;
	}

private:
	void onColorChange()
	{
		if (m_suppressColorUpdate) return;

		m_suppressColorUpdate = true;
		Color c = m_colorPicker.getColor();
		m_numR.setValue(tool::clamp(int(c.r * 255.0f), 0, 255));
		m_numG.setValue(tool::clamp(int(c.g * 255.0f), 0, 255));
		m_numB.setValue(tool::clamp(int(c.b * 255.0f), 0, 255));
		m_suppressColorUpdate = false;
	}

	void onColorChangeNum()
	{
		if (m_suppressColorUpdate) return;

		m_suppressColorUpdate = true;
		Color c;
		c.r = tool::clamp(float(m_numR.getValue()) / 255.0f, 0.0f, 1.0f);
		c.g = tool::clamp(float(m_numG.getValue()) / 255.0f, 0.0f, 1.0f);
		c.b = tool::clamp(float(m_numB.getValue()) / 255.0f, 0.0f, 1.0f);
		m_colorPicker.setColor(c);
		m_suppressColorUpdate = false;
	}

private:
	UIObjectList m_uiObjects;

	UIColorPicker m_colorPicker;
	UIButtonText m_btnOk;
	UIButtonText m_btnCancel;

	UINumUpDownInt m_numR;
	UINumUpDownInt m_numG;
	UINumUpDownInt m_numB;

	UILabel m_lblR;
	UILabel m_lblG;
	UILabel m_lblB;

	Color m_color = Color::Red();

	bool m_suppressColorUpdate = false;
};
