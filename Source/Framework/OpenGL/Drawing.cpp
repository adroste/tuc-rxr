#include "Drawing.h"
#include "../../glew/glew.h"
#include "../../glm/gtc/matrix_transform.inl"

void Drawing::rect(const RectF & r, const Color & c)
{
	glColor4f(c.r, c.g, c.b, c.a);
	//glRectf(r.x1, r.y1, r.x2, r.y2);
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f(r.x2, r.y1, 0.0f);
		glVertex3f(r.x1, r.y1, 0.0f);
		glVertex3f(r.x2, r.y2, 0.0f);
		glVertex3f(r.x1, r.y2, 0.0f);
	}
	glEndSafe();
}

void Drawing::coloredCube(const PointF& pos, float scalar, const Color& c)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(pos.x, pos.y, 0.0f);
	glScalef(scalar, scalar, scalar);

	m_shCube.setColor(c);
	m_shCube.bind();
	m_meshCube.draw();
	m_shCube.unbind();

	glPopMatrix();
}

Drawing::Drawing()
{
	m_shCube.load();
}

void Drawing::create()
{
	m_meshCube.create();
	m_shCube.create();
}

void Drawing::dispose()
{
	m_meshCube.dispose();
	m_shCube.dispose();
}
