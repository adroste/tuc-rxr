#pragma once
#include "GLObject.h"
#include "../../Utility/Rect.h"
#include "../Color.h"

class Draw : GLObject
{
public:
	Draw() {}
	~Draw() {}

	virtual void create() override;
	virtual void dispose() override;

	void rect(const RectF& r, const Color& c);

	
};