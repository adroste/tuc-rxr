#include "Graphics.h"
#include "../Framework.h"
#include "../../System/Log.h"
#include "../../glew/glew.h"
#include "../../Utility/GLEW_Exception.h"
#include "../../Utility/GL_Exception.h"
#include "../../Utility/SDL_Exception.h"

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
		isGlewInit = true;
	}

	resize(dim);

	m_isInit = true;
}

void Graphics::close()
{
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
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&m_projectMat[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&m_camMat[0][0]);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);//| GL_DEPTH_BUFFER_BIT);

	glCheck("Graphics::beginFrame");
}

void Graphics::endFrame()
{
	glCheck("endFrame");
	glFlush();
}

void Graphics::resize(PointI dim)
{
	glViewport(0, 0, (GLsizei)dim.x, (GLsizei)dim.y);

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

	float aspect = newWidth / newHeight;
	float fovY = 2 * atanf(float(newHeight / 2) / 10.0f);

	Framework::setWindowSize(dim, { newWidth, newHeight }, scale);

	m_projectMat = glm::perspective(fovY, aspect, 0.1f, 100.0f);

	static const PointF midpoint(Framework::STD_DRAW_X / 2, Framework::STD_DRAW_Y / 2);
	m_camMat = glm::lookAt(
		glm::vec3(midpoint.x, midpoint.y, -10.0f), // camera pos
		glm::vec3(midpoint.x, midpoint.y, 0.0f), // point to look at (z)
		glm::vec3(0.0f, -1.0f, 0.0f)); // up vector (-1.0f, because opengl is mirrored horizontally)
}
