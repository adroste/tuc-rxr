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
