#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "../../../Framework/UI/UIMessageBox.h"
#include "../../../Framework/UI/UITextBox.h"
#include "../../../Framework/UI/UIObjectList.h"
#include "../../../Framework/UI/UICheckBox.h"
#include "../../../Framework/UI/UITabView.h"
#include "../../../Framework/UI/Editor/UIContainerMaterial.h"

class StateDev : public GameState
{
public:
	StateDev()
		:
		m_pUiLayer1(new WindowLayer()),
		//m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Back"),
		//m_material(false, *m_pUiLayer1),
		m_material2(true, *m_pUiLayer1)
	{
		//m_btnBack.adjustToFontHeadline();
		//m_pUiLayer1->addWindow(&m_btnBack, Anchor::Left | Anchor::Bottom);
		//m_pUiLayer1->addWindow(&m_material, Anchor::Right);
		m_pUiLayer1->addWindow(&m_material2, Anchor::Right | Anchor::Top);
		addLayer(m_pUiLayer1);
	}

	virtual ~StateDev() override
	{}

	virtual bool update(float dt) override
	{
		//if (m_btnBack.isClicked(true))
		//	//setNextState(TransitionState::Discard);
		//	m_material.show();

		

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
	WindowLayer* m_pUiLayer1;
	//UIButtonText m_btnBack;
	//UIContainerMaterial m_material;
	UIContainerMaterial m_material2;
};
