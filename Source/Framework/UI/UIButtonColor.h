#pragma once
#include "UIButton.h"
#include "Interfaces/IValueHolder.h"

class UIButtonColor : public UIButton, public IValueHolder<Color>
{
public:
	UIButtonColor(const Color& c)
		: UIButton(Style::Royal),
		m_colorPicker()
	{
		IValueHolder::setValue(c);
	}
	virtual void draw(Drawing& draw) override
	{
		// TODO replace with fancy button
		draw.rect(getRect(), getValue());

		m_colorPicker.draw(draw);
	}

	virtual void registerMe(Input::IBroadcaster* broadcaster) override
	{
		Input::IReceiver::registerMe(broadcaster);

		m_colorPicker.registerMe(broadcaster);
	}
protected:
	virtual void clickEvent() override
	{
		// show color dialog
	
		m_colorPicker.setOrigin({ 10,10 });
		m_colorPicker.show();
	}

private:
	UIDialogColorPicker m_colorPicker;
};
