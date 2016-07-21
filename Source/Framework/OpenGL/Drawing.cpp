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

void Drawing::button(const RectF& r, float border)
{
	m_shButton.bind();

	glBegin(GL_TRIANGLE_STRIP);
	{
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(r.x2, r.y1, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(r.x1, r.y1, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(r.x2, r.y2, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(r.x1, r.y2, 0.0f);
	}
	glEndSafe();

	m_shButton.unbind();
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

Font& Drawing::getFont()
{
	return m_fontText;
}

Drawing::Drawing()
{}

void Drawing::create()
{
	m_meshCube.create();
	m_shCube.create();
	m_shButton.create();

	m_fontText.create();
}

void Drawing::dispose()
{
	m_meshCube.dispose();
	m_shCube.dispose();
	m_shButton.dispose();

	m_fontText.dispose();
}

void Drawing::init(FT_Library ftlib)
{
	m_shCube.load();
	m_shButton.load();

	// TODO set appropriate scalar
	m_fontText.load(ftlib, "data/Font/Iron_Latch.ttf", 50.0f);
}
