#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "StateDev.h"
#include "../GameEditor.h"
#include "../../../Framework/UI/UIItemLister.h"

class StateEditor : public GameState
{
public:
	StateEditor()
		:
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Back"),
		lblTest(Drawing::getFont(Font::Style::Text,Font::Size::M),"Hallo")
	{
		m_editor.registerMe(this);


		m_btnBack.adjustToFontHeadline();
		m_btnBack.setOrigin({ 10.0f, Framework::STD_DRAW_Y - (m_btnBack.getDim().y + 10.0f) });
		m_btnBack.registerMe(this);

		// material list
		m_listMaterial.addToList(&lblTest,nullptr);
		m_listMaterial.setDim({ 400,600 });
		m_listMaterial.setOrigin({ 600,100 });

		m_listMaterial.registerMe(this);
		m_listMaterial.orderItems();
	}

	virtual ~StateEditor() override
	{
	}

	virtual bool update(float dt) override
	{
		if (m_btnBack.isClicked(true))
			setNextState(TransitionState::Discard);

		return true;
	}
	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_editor.draw(draw);
		
		m_listMaterial.draw(draw);
		m_btnBack.draw(draw);
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

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		GameState::mouseMove(mpos, handled);
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
	GameEditor m_editor;

	UIButtonText m_btnBack;

	// material list
	UIItemLister m_listMaterial;

	UILabel lblTest;
};
