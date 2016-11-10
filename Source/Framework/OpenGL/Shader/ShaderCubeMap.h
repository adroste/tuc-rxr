#pragma once
#include "Shader.h"
#include "../../Color.h"

enum class AssetAnimation;

class ShaderCubeMap : public Shader, public Shader::ILoadable
{
public:
	virtual ~ShaderCubeMap() {}
	void load() override;
	void setAnimation(AssetAnimation a);
	void setTransparent(bool isTrans);
protected:
	virtual void loadUniforms() override;

private:
	GLint m_animation = -1;
	GLint m_transparent = -1;
};
