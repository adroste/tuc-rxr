#include "Drawing.h"
#include "../../glew/glew.h"

void Drawing::rect(const RectF & r, const Color & c)
{
	glColor4f(c.r, c.g, c.b, c.a);
	glRectf(r.x1, r.y1, r.x2, r.y2);

}

void Drawing::create()
{
}

void Drawing::dispose()
{
}
