#include "UniformBlock.h"
#include <cassert>

UniformBlock::UniformBlock(std::initializer_list<Shader*> refShader, const std::string& blockName)
	:
	m_shaders(refShader),
	m_blockName(blockName)
{
	static GLuint bindPoint = 0;
	m_bindingPoint = bindPoint++; // these numbers must be uniquely given by the application
}

UniformBlock::~UniformBlock()
{
}
 
void UniformBlock::create()
{
	for(auto& s : m_shaders)
	{
		assert(s->m_program);

		auto blockLocation = glGetUniformBlockIndex(s->m_program, m_blockName.c_str());
		if (blockLocation == GL_INVALID_INDEX)
			throw Exception("UniformBlock::create cannot find uniform block " + m_blockName + " in shader: " + s->m_filename);

		glUniformBlockBinding(s->m_program, blockLocation, m_bindingPoint);
	}

	// retrieve block size from first programm
	assert(m_shaders.size());

	Shader* first = *m_shaders.begin();
	auto firstLoc = glGetUniformBlockIndex(first->m_program, m_blockName.c_str());

	glGetActiveUniformBlockiv(first->m_program, firstLoc, GL_UNIFORM_BLOCK_DATA_SIZE, &m_blockSize);

	m_pBuffer = std::unique_ptr<char[]>(new char[m_blockSize]);

	// create buffer object
	assert(m_vbo == 0);
	glGenBuffers(1, &m_vbo);

	glCheck("UniformBlock::create");
}

void UniformBlock::dispose()
{
	if(m_vbo)
	{
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
	}
}

void UniformBlock::flush()
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_vbo);
	glBufferData(GL_UNIFORM_BUFFER, m_blockSize, m_pBuffer.get(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_vbo);
	glCheck("UniformBlock::flush");
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}