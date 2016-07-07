#pragma once

class GLObject
{
public:
	virtual ~GLObject() {}

	virtual void create() = 0;
	virtual void dispose() = 0;
};