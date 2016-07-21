#include "Graphics.h"
#include "../Framework.h"
#include "../../System/Log.h"
#include "../../glew/glew.h"
#include "../../Utility/GLEW_Exception.h"
#include "../../Utility/GL_Exception.h"
#include "../../Utility/SDL_Exception.h"
#include <ft2build.h>
#include FT_FREETYPE_H

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
	close();
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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
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
	if (m_needsResize)
		doResize();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&m_projectMat[0][0]);
	m_draw.setProjection(m_projectMat);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&m_camMat[0][0]);
	m_draw.setCamera(m_camMat);

	m_draw.setModel(glm::mat4(1.0f));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCheck("Graphics::beginFrame");
}

void Graphics::endFrame()
{
	glCheck("endFrame");
	glFlush();
}

void Graphics::resize(PointI dim)
{
	m_wndSize = dim;

	// set projection
	float scaleX = float(dim.x) / float(Framework::STD_DRAW_X);
	float scaleY = float(dim.y) / float(Framework::STD_DRAW_Y);

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

	float newWidth = float(Framework::STD_DRAW_X) * scaleX;
	float newHeight = float(Framework::STD_DRAW_Y) * scaleY;

	static const float DIST_TO_CAMERA = 1000.0f;
	float aspect = newWidth / newHeight;
	float fovY = 2 * atanf(float(newHeight / 2) / DIST_TO_CAMERA);

	Framework::setWindowSize(dim, { newWidth, newHeight }, scale);

	m_projectMat = glm::perspective(fovY, aspect, DIST_TO_CAMERA / 2.0f, DIST_TO_CAMERA * 2.0f);

	static const PointF midpoint(Framework::STD_DRAW_X / 2, Framework::STD_DRAW_Y / 2);
	m_camMat = glm::lookAt(
		glm::vec3(midpoint.x, midpoint.y, -DIST_TO_CAMERA), // camera pos
		glm::vec3(midpoint.x, midpoint.y, 0.0f), // point to look at (z)
		glm::vec3(0.0f, -1.0f, 0.0f)); // up vector (-1.0f, because opengl is mirrored horizontally)

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
	glViewport(0, 0, GLsizei(m_wndSize.x), GLsizei(m_wndSize.y));
	m_needsResize = false;
	glCheck("Graphics::doResize");
}
