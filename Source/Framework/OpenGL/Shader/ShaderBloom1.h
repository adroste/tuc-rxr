#pragma once
#include "Shader.h"
#include "../../../glm/detail/type_vec2.hpp"

class ShaderBloom1 : public Shader, public Shader::ILoadable
{
public:
	ShaderBloom1() = default;
	virtual ~ShaderBloom1() override = default;

	void load() override
	{
		Shader::load("data/Shader/bloom1");
	}
};

class ShaderBloom2 : public Shader, public Shader::ILoadable
{
public:
	ShaderBloom2() = default;
	virtual ~ShaderBloom2() override = default;

	void load() override
	{
		Shader::load("data/Shader/bloom2");
	}
	void setDir(const glm::vec2& dir)
	{
		bind();
		glUniform2f(m_dir, dir.x, dir.y);
		unbind();
	}
protected:
	virtual void loadUniforms() override
	{
		m_dir = locateUniform("dir");
	}

private:
	GLint m_dir;
};

class ShaderBloom3 : public Shader, public Shader::ILoadable
{
public:
	ShaderBloom3() = default;
	virtual ~ShaderBloom3() override = default;

	void load() override
	{
		Shader::load("data/Shader/bloom3");
	}
};