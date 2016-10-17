#include "LightSource.h"
#include <map>

ENUM_CONVERT(LightType, LightType::Unknown,
{ LightType::Directional,"directional" },
{ LightType::PointLight,"pointLight" },
{ LightType::Ambient,"ambient" },
{ LightType::Unknown,"unknown" }
)