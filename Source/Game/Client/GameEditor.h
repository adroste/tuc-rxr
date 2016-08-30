#pragma once
#include "../../Framework/UI/UIObject.h"
#include "../Shared/Map.h"
#include "LightManager.h"
#include "../../System/Input.h"

class GameEditor : public IDrawable, public Input::IReceiver
{
public:
	GameEditor();
	virtual ~GameEditor() override;
	virtual void draw(Drawing& draw) override;

	virtual bool mouseMove(const PointF& mpos, bool handled) override;
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override;
	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override;
	virtual bool wheel(const PointF& mpos, float amount) override;
	virtual bool keyUp(SDL_Scancode s) override;

	void setCurrentBlock(const CubeDesc& c);
private:
	void drawGrid(Drawing& draw) const;
	void releaseCapture();
	void takeCapture();

private:
	std::unique_ptr<Map> m_pMap;

	std::unique_ptr<Camera> m_pCam;
	std::unique_ptr<LightManager> m_pLight;
	bool m_hasCapture = false;
	bool m_hover = false;
	bool m_zdown = false;

	// mouse positioning
	PointF m_prevClientMouse;
	PointF m_mouseDiff;
	float m_wheelDiff = 0.0f;
	Point3I m_blockPos;

	CubeDesc m_curCubeDesc;
};
