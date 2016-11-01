#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/Sound/Sound.h"
#include "../../../Framework/UI/UIButton.h"
#include "StateGame.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "../../../Framework/UI/UIDialogColorPicker.h"
#include "../../../Framework/UI/UIInputField.h"
#include "../../../Framework/UI/UILabel.h"
#include "StateEditor.h"
#include "../../../Framework/UI/UITrackbar.h"

class StateMenu : public GameState
{
public:
	StateMenu()
		:
		m_title(Drawing::getFont(Font::Style::Headline, Font::Size::L), "MainWindow"),
		m_btnSingle(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::M), "Can't Touch This"),
		m_btnEdit(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::M), "Editor"),
		m_btnDev(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Dev"),
		m_tbUiSize(0.0f, 0.25f)
	{
		m_btnSingle->adjustToFontHeadline();
		m_btnEdit->adjustToFontHeadline();
		m_btnEdit->setWidth(m_btnSingle->getDim().x);
		m_btnDev->adjustToFontHeadline();

		m_tbUiSize->setDim(PointF(400.0f, 25.0f));
		m_tbUiSize->setOnValueCallback([this](IValueHolder<float>* o)
		{
			// 50% slider = def std draw
			PointF sd = PointF(Framework::getDefStdDraw()) * (o->getValue() + 1.0f);
			Framework::setStdDraw(PointI(sd));
			//updateWindows();
		});

		m_mpos = { 500,500 };
		//Sound::playMusic(Sound::Music::Hey);

		addWindow(m_title.getRef(), 0, PointF(0.0f, -250.0f));
		addWindow(m_btnSingle.getRef(), 0, PointF(0.0f, -50.0f));
		addWindow(m_btnEdit.getRef(), 0, PointF(0.0f, +50.0f));
		addWindow(m_btnDev.getRef(), Anchor::Right | Anchor::Bottom, PointF(10.0f));
		addWindow(m_tbUiSize.getRef(), Anchor::Bottom);
	}
	virtual ~StateMenu()
	{}

	virtual bool update(float dt) override
	{
		if (m_btnSingle->isClicked(true))
		{	
			Sound::playMusic(Sound::Music::Theme);
			//Sound::playSound(Sound::Sfx::Plop);
			setNextState(TransitionState::ForcePreserve, std::unique_ptr<GameState>(new StateGame()));
		}

		if (m_btnEdit->isClicked(true))
		{
			setNextState(TransitionState::ForcePreserve, std::unique_ptr<GameState>(new StateEditor()));
		}

		if (m_btnDev->isClicked(true))
			setNextState(TransitionState::Preserve, std::unique_ptr<GameState>(new StateDev()));

		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		draw.line(m_mpos, Framework::getScreenCenter(), 10.0f, Color::Red());
		drawWindows(draw);
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

		m_mpos = mpos;
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
	refable<UILabel>	m_title;

	PointF m_mpos;
	refable<UIButtonText> m_btnSingle;
	refable<UIButtonText> m_btnEdit;
	refable<UIButtonText> m_btnDev;
	refable<UITrackbar> m_tbUiSize;

	//UIDialogColorPicker m_colorPicker;
};
