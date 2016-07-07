#pragma once
#include "GLObject.h"
#include "../../Utility/Rect.h"
#include "../Color.h"

class Drawing : GLObject
{
public:
	Drawing() {}
	~Drawing() {}

	virtual void create() override;
	virtual void dispose() override;

	void rect(const RectF& r, const Color& c);

	
};