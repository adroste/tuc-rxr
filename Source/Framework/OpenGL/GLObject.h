#pragma once
#include "../../glew/glew.h"
#include "glhelper.h"

class GLObject
{
public:
	virtual ~GLObject() {}

	virtual void create() = 0;
	virtual void dispose() = 0;
};
