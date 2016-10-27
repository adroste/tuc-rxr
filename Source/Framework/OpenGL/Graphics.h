#pragma once
#include "../../SDL/SDL.h"
#include "../../Utility/Point.h"
#include "../../glm/gtx/transform.hpp"
#include "Drawing.h"

class Graphics
{
public:
	Graphics();
	~Graphics();
	void init(SDL_Window* wnd, PointI dim);
	void close();

	void beginFrame();
	void endFrame();

	// calculations for resizing
	void resize(PointI dim);
	Drawing& getDraw();

private:
	// actual resize
	void doResize();

private:
	SDL_GLContext m_glContext = nullptr;
	bool m_isInit = false;
	glm::mat4 m_projectMat;
	glm::mat4 m_camMat;

	PointI m_wndSize;
	bool m_needsResize = false;

	struct FT_LibraryRec_* m_ft = nullptr;

	Drawing m_draw;
};