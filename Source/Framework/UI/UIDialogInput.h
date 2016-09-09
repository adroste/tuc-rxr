#pragma once
#include "UIDialog.h"
#include "UITextBox.h"
#include "UIInputBox.h"
#include "UILabel.h"

class UIDialogInput : public UIDialog
{
public:
	UIDialogInput()
		:
		UIDialog(Buttons::OKCancel),
		m_title(Drawing::getFont(Font::Style::Headline, Font::Size::M), ""),
		m_inpBox(Drawing::getFont(Font::Style::Text, Font::Size::M), 128)
	{
		UIDialog::setDim({300,300});
		m_inpBox.registerMe(this);

		m_title.setOrigin(PointF(10.0f));
		m_inpBox.setOrigin(m_title.getRect().getBottomLeft() + PointF(0.0f, 10.0f));
		auto& inpFont = Drawing::getFont(Font::Style::Text, Font::Size::M);
		m_inpBox.setDim(inpFont.getDim("Sample Text") + PointF(10.0f));
	}

	void setTitle(const std::string& s)
	{
		m_title.setText(s);
		recalcDim();
	}

	virtual void show() override
	{
		m_inpBox.clear();
		m_inpBox.select();
		UIDialog::show();
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;
		UIDialog::draw(draw);
		pushDrawTransforms(draw);
		m_title.draw(draw);
		m_inpBox.draw(draw);
		popDrawTransforms(draw);
	}

	std::string getInput() const
	{
		return m_inpBox.getText();
	}

protected:
	virtual void setResult(Result dlgResult) override
	{
		if (dlgResult == Result::OK && getInput().length() == 0)
			return;

		UIDialog::setResult(dlgResult);
	}
private:
	void recalcDim()
	{
		float width = std::max(150.0f, m_title.getDim().x);
		m_inpBox.setWidth(width);

		setClientArea({width , m_title.getDim().y + m_inpBox.getDim().y + 20.0f});
	}

private:
	UILabel m_title;
	UIInputBox m_inpBox;
};
