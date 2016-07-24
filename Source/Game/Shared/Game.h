#pragma once
#include <memory>
#include "Map.h"
#include "../../System/Input.h"

#ifdef _CLIENT
#include "../../Framework/OpenGL/Camera.h"
#endif // _CLIENT

class Game : public Input::IReceiver, public IDrawable
{
public:
	Game();
	virtual ~Game();

#ifdef _CLIENT
	virtual void draw(Drawing& draw) override;
	virtual bool keyDown(SDL_Scancode s) override;
#endif // _CLIENT

private:
	std::unique_ptr<Map> m_pMap;

#ifdef _CLIENT
	std::unique_ptr<Camera> m_pCam;
#endif // _CLIENT
};
