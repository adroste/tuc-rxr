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

private:
	void drawGrid(Drawing& draw) const;

private:
	Point3S m_mapDim;

	std::unique_ptr<Camera> m_pCam;
	std::unique_ptr<LightManager> m_pLight;
};
