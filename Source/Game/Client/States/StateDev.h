#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "../../../Framework/UI/UIMessageBox.h"
#include "../../../Framework/UI/UITextBox.h"
#include "../../../Framework/UI/UIObjectList.h"
#include "../../../Framework/UI/UICheckBox.h"
#include "../../../Framework/UI/UITabView.h"
#include "../../../Framework/UI/Editor/UIContainerMaterial.h"
#include "../../../Framework/UI/UITrackbar.h"

class StateDev : public GameState
{
public:
	StateDev()
		:
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Back"),
		m_material(false, *this),
		m_material2(false, *this),
		m_trackbar(),
		m_label(Drawing::getFont(Font::Style::Text, Font::Size::M), "-1.0")
	{
		m_btnBack.adjustToFontHeadline();

		m_trackbar.setDim(PointF(100.0f));
		m_trackbar.setOnValueCallback([this](IValueHolder<float>* o)
		{
			m_label.setText(std::to_string(o->getValue()));
		});

		m_label.setZIndex(10);

		addWindow(&m_btnBack, Anchor::Left | Anchor::Bottom);
		addWindow(&m_material, Anchor::Right);
		addWindow(&m_material2, Anchor::Right | Anchor::Top);
		addWindow(&m_trackbar, Anchor::Bottom);
		addWindow(&m_label, Anchor::Left, PointF(10.0f, 40.0f));
	}

	virtual ~StateDev() override
	{}

	virtual bool update(float dt) override
	{
		if (m_btnBack.isClicked(true))
			setNextState(TransitionState::Discard);

		

		return false;
	}

	// Input handling
	virtual bool keyDown(SDL_Scancode s) override
	{
		GameState::keyDown(s);
		return true;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		GameState::keyUp(s);
		return true;
	}

	virtual bool charDown(char c) override
	{
		GameState::charDown(c);
		return true;
	}

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		GameState::mouseMove(mpos, mdiff, handled);
		return true;
	}

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseDown(mpos, button);
		return true;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseUp(mpos, button);
		return true;
	}

	virtual bool wheel(const PointF& mpos, float amount) override
	{
		GameState::wheel(mpos, amount);
		return true;
	}

private:
	UIButtonText m_btnBack;
	UIContainerMaterial m_material;
	UIContainerMaterial m_material2;
	UITrackbar m_trackbar;
	UILabel m_label;
};
