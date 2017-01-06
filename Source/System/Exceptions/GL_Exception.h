#pragma once
#include "Exception.h"
#include <string>
#include "../../glew/glew.h"

class GL_Exception : public Exception
{
public:
	GL_Exception(const std::string& func, GLenum code)
		:
		Exception(""),
		code(code)
	{
		m_msg = "OpenGL: " + func + ": " + std::to_string(code);
		AppendInfo();
	}
	GLenum GetCode() const
	{
		return code;
	}
private:
	void AppendInfo()
	{
		switch (code)
		{
		case GL_INVALID_ENUM:
			m_msg += ": GL_INVALID_ENUM";
			break;
		case GLU_INVALID_VALUE:
			m_msg += ": GLU_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			m_msg += ": GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			m_msg += ": GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			m_msg += ": GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			m_msg += ": GL_OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			m_msg += ": GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_TABLE_TOO_LARGE:
			m_msg += ": GL_TABLE_TOO_LARGE";
			break;
		}
		m_msg += " -> ";
		m_msg += (char*)glewGetErrorString(code);
	}
private:
	GLenum code;
};

class GL_Recreate : public GL_Exception
{
public:
	GL_Recreate()
		:
		GL_Exception("glEnd", GL_INVALID_OPERATION)
	{}
};

inline void glCheck(const char* func)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
#if _DEBUG
		std::string info = GL_Exception(func, err).what();
		__debugbreak();
#else
		throw GL_Exception(func, err);
#endif
	}
}

inline void glEndSafe()
{
	glEnd();
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		if (err == GL_INVALID_OPERATION)
		{
			throw GL_Recreate();
		}
		else
		{
			throw GL_Exception("glEnd", err);
		}
	}
}
