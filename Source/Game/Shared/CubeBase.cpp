#include "CubeBase.h"

CubeBase::CubeBase(const CubeDesc& cd)
	: m_cd(cd)
{
}

CubeBase::~CubeBase()
{
}

const CubeDesc& CubeBase::getDesc() const
{
	return m_cd;
}

bool CubeBase::isOpaque() const
{
	return m_cd.shader == CubeShader::Default;
}

bool CubeBase::hasTransparency() const
{
	switch (m_cd.shader)
	{
	case CubeShader::Default: 
		return true;
	default:
		return false;
	}
}
