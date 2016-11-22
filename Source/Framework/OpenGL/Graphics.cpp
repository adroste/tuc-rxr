#include "Graphics.h"
#include "../Framework.h"
#include "../../System/Log.h"
#include "../../glew/glew.h"
#include "../../System/Exceptions/GLEW_Exception.h"
#include "../../System/Exceptions/GL_Exception.h"
#include "../../System/Exceptions/SDL_Exception.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../../System/Input.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
	
}


void Graphics::init(SDL_Window* wnd, PointI dim)
{
	assert(!m_isInit);
	assert(m_glContext == nullptr);
	
	m_glContext = SDL_GL_CreateContext(wnd);
	if (!m_glContext)
		throw SDL_Exception("Graphics::init open gl context could not be created");

	static bool isGlewInit = false;
	if (!isGlewInit)
	{
		Log::info("Graphics::init glew");

		auto status = glewInit();
		if (status != GLEW_OK)
			throw GLEW_Exception("Graphics::init glew init", status);

		Log::info("Graphics::init freetype");
		if (FT_Init_FreeType(&m_ft))
			throw Exception("Graphics::init freetype init failed");

		m_draw.init(m_ft);

		isGlewInit = true;
	}

	resize(dim);
	doResize();
	// auto correct gamma:
	//glEnable(GL_FRAMEBUFFER_SRGB);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	

	m_draw.create();

	m_isInit = true;
}

void Graphics::close()
{
	m_draw.dispose();

	if (m_glContext)
	{
		Log::info("Graphics::close deleting openGL context");
		SDL_GL_DeleteContext(m_glContext);
		m_glContext = nullptr;
	}

	m_isInit = false;
}

void Graphics::beginFrame()
{
	// dispose old objects in draw
	if(m_draw.m_disposeStack.size())
	{
		LockGuard g(m_draw.m_muDisposeStack);
		while (m_draw.m_disposeStack.size())
			m_draw.m_disposeStack.pop();
	}

	if (m_needsResize)
		doResize();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_draw.m_blockFramework.setMouse(Input::getMouse());
	m_draw.m_blockFramework.updateRandom();
	m_draw.m_blockFramework.flush();

	m_draw.beginFrame();
	m_draw.getUiCam().apply(m_draw);

	glCheck("Graphics::beginFrame");
}

void Graphics::endFrame()
{
	m_draw.endFrame();
	glCheck("endFrame");
	glFinish();
}

void Graphics::resize(PointI dim)
{
	m_wndSize = dim;

	auto sd = Framework::getStdDraw();
	float scaleX = float(dim.x) / float(sd.x);
	float scaleY = float(dim.y) / float(sd.y);

	float scale = std::min(scaleX, scaleY);

	if (scaleY > scaleX)
	{
		scaleY /= scaleX;
		scaleX = 1.0f;
	}
	else
	{
		scaleX /= scaleY;
		scaleY = 1.0f;
	}

	float newWidth = float(sd.x) * scaleX;
	float newHeight = float(sd.y) * scaleY;

	Framework::setWindowSize(dim, { newWidth, newHeight }, scale);

	m_needsResize = true;
	Log::info("Graphics::resize");
}

Drawing& Graphics::getDraw()
{
	return m_draw;
}

void Graphics::doResize()
{
	Log::info("Graphics::doResize");
	m_needsResize = false;
	glViewport(0, 0, GLsizei(m_wndSize.x), GLsizei(m_wndSize.y));
	m_draw.getUiCam().setLookAt(Framework::getScreenCenter());
	m_draw.getUiCam().setHeight(Framework::getCamDim().y);
	m_draw.resize(GLsizei(m_wndSize.x), GLsizei(m_wndSize.y));
	glCheck("Graphics::doResize");
}
