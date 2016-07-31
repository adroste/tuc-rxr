#pragma once
#include "Shader.h"
#include <memory>
#include <cassert>
#include <vector>

class UniformBlock : public GLObject
{
public:
	class ExceptionBlockSize : public Exception
	{
	public:
		ExceptionBlockSize(const std::string& function ,size_t desiredSize, size_t availableSize)
			: Exception(function + " invalid uniform block size. Size should be "
			+ std::to_string(desiredSize) + " but is: " + std::to_string(availableSize))
		{}
		virtual ~ExceptionBlockSize()
		{}
	};
public:
	UniformBlock(std::initializer_list<Shader*> refShader, const std::string& blockName);
	virtual ~UniformBlock();

	// important: init shader first
	void create() override;
	void dispose() override;

	// send data to gpu
	void flush();
protected:
	template <class T>
	void updateVar(const T& var, size_t offset)
	{
		assert(offset + sizeof(T) <= size_t(m_blockSize));
		memcpy(m_pBuffer.get() + offset, &var, sizeof(T));
	}
	void updateArray(const void* src, size_t len, size_t offset)
	{
		assert(offset + len <= size_t(m_blockSize));
		memcpy(m_pBuffer.get() + offset, src, len);
	}
protected:
	std::vector<Shader*> m_shaders;
	const std::string m_blockName;
	GLint m_blockSize = 0;
	GLuint m_vbo = 0;
	GLuint m_bindingPoint;

	std::unique_ptr<char[]> m_pBuffer;
};
