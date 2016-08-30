#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "StateDev.h"
#include "../GameEditor.h"
#include "../../../Framework/UI/UIItemLister.h"
#include "../../../Framework/UI/Editor/UIContainerMaterial.h"

class StateEditor : public GameState
{
public:
	StateEditor()
		:
		m_uiList({ &m_btnBack, &m_listMaterial }),
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Back")
	{
		m_editor.registerMe(this);


		m_btnBack.adjustToFontHeadline();
		m_btnBack.setOrigin({ 10.0f, Framework::STD_DRAW_Y - (m_btnBack.getDim().y + 10.0f) });
		m_btnBack.setZIndex(1);

		// material list
		m_listMaterial.setDim({ 400,600 });
		m_listMaterial.setOrigin({ 800,100 });
		m_listMaterial.orderItems();
		m_listMaterial.setZIndex(2);

		m_uiList.registerAll(this);

		m_editor.setCurrentBlock(m_listMaterial.getCubeDesc());
		m_listMaterial.setOnMaterialChangeCallback([this](CubeDesc d)
		{
			m_editor.setCurrentBlock(d);
		});
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
		
		m_uiList.draw(draw);
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
	UIObjectList m_uiList;

	GameEditor m_editor;

	UIButtonText m_btnBack;

	// material list
	UIContainerMaterial m_listMaterial;
};
