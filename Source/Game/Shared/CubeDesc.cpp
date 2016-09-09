#include "CubeDesc.h"
#include <map>

ENUM_CONVERT(CubeShader,CubeShader::Size, 
{ CubeShader::Default,"default" },
{ CubeShader::Special,"special" }
)

ENUM_CONVERT(BlockType, BlockType::Solid,
{ BlockType::Immortal,"immortal" },
{ BlockType::Transparent,"transparent" },
{ BlockType::Solid,"solid" },
{ BlockType::Liquid,"liquid" },
)
