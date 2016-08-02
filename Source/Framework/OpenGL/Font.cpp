#include "Font.h"
#include "../../System/Exceptions/ExceptionMissingFile.h"
#include "../../System/Exceptions/ExceptionInvalidFile.h"

Font::Font()
{
	memset(m_metrics, 0, sizeof(FT_Glyph_Metrics) * m_nChars);
}

void Font::load(FT_LibraryRec_ * ft, const std::string & fileName, float scalar)
{
	m_ft = ft;
	long filesize = 0;

	// read data from file
	{
		FILE* pFile = fopen(fileName.c_str(), "rb");
		if (!pFile)
			throw ExceptionMissingFile("Font::load", fileName);

		// obtain file size:
		fseek(pFile, 0, SEEK_END);
		filesize = ftell(pFile);
		rewind(pFile);

		m_rawData = std::unique_ptr<char[]>(new char[filesize]);

		auto count = fread(m_rawData.get(), 1, filesize, pFile);
		fclose(pFile);
		
		if (count != filesize)
			throw ExceptionInvalidFile("Font::load could not read all bytes", fileName);
	}

	// create new memory face
	auto fterr = FT_New_Memory_Face(m_ft, 
		reinterpret_cast<FT_Byte*>(m_rawData.get()), filesize,
		0, &m_face);
	if (fterr)
		throw ExceptionInvalidFile("Font::load", fileName);

	// TODO think about it
	// setting texture size int pixel
	fterr = FT_Set_Pixel_Sizes(m_face, 0, int(scalar * 1.5f));

	findMaxBearing(scalar);

	// Shader
	Shader::load("data/Shader/text");
}

void Font::create()
{
	Shader::create();

	// set uniform default values
	glUseProgram(m_program);
	glUniform1i(m_uniLocTexture, 0);
	glUniform4f(m_uniLocColor, 1.0f, 1.0f, 1.0f, 1.0f);
	glUseProgram(0);

	glCheck("Font::create uniforms");

	loadTextures();

	glCheck("Font::create textures");
}

void Font::dispose()
{
	glDeleteTextures(CHAR_END - CHAR_START, m_textureArray);
	memset(m_textureArray, 0, sizeof(m_textureArray));

	Shader::dispose();
}

void Font::write(const std::string& txt, PointF pos) const
{
	// bind shader
	bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);

	// Loop through all chars
	for (const auto& c : txt)
	{
		if (c < CHAR_START || c > CHAR_END)
			continue;

		const FT_Glyph_Metrics& m = m_metrics[c];

		// calculate position things
		float bmpHeight = m.height * m_scaleFactor;
		float offHeight = (m_maxBearing - m.horiBearingY) * m_scaleFactor;

		float bmpWidth = m.width * m_scaleFactor;
		float advance = m.horiAdvance * m_scaleFactor;
		float bearX = m.horiBearingX * m_scaleFactor;

		if(c != 0x1f) // unit seperator
		{
			// bind and draw texture
			assert(m_textureArray[c - CHAR_START]);
			glBindTexture(GL_TEXTURE_2D, m_textureArray[c - CHAR_START]);

			glCheck("font::write before glBegin");
			glBegin(GL_TRIANGLE_STRIP);
			{

				glVertex4f(pos.x + bearX, pos.y + offHeight, 0, 0);
				glVertex4f(pos.x + bearX, pos.y + offHeight + bmpHeight, 0, 1);
				glVertex4f(pos.x + bearX + bmpWidth, pos.y + offHeight, 1, 0);
				glVertex4f(pos.x + bearX + bmpWidth, pos.y + offHeight + bmpHeight, 1, 1);
			}
			glEndSafe();
		}
		

		// Advance the cursor to the start of the next character
		pos.x += advance;
	}

	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, 0);

	unbind();
	glCheck("font::write after drawing");
}

PointF Font::getDim(const char* txt) const
{
	float width = 0.0f;
	while(*txt != '\0')
	{
		assert(*txt < m_nChars);
		if(*txt < m_nChars)
			width += m_metrics[*txt].horiAdvance;
		txt++;
	}
	// return actual width + max font height (is better for positioning etc.)
	return PointF(width * m_scaleFactor, m_fontHeight);
}

PointF Font::getDim(const std::string& txt) const
{
	return getDim(txt.c_str());
}

void Font::setColor(const Color& c)
{
	glUseProgram(m_program);
	glUniform4f(m_uniLocColor, c.r, c.g, c.b, c.a);
	glUseProgram(0);
}

void Font::loadUniforms()
{
	// locate unitforms
	m_uniLocTexture = glGetUniformLocation(m_program, "tex");
	if (m_uniLocTexture == -1)
		throw Exception("Font::loadUniforms tex uniform not found");

	m_uniLocColor = glGetUniformLocation(m_program, "color");
	if (m_uniLocColor == -1)
		throw Exception("Font::loadUniforms tex uniform not found");
}

void Font::load()
{
	// dummy function dont call
	throw Exception("Font::load stupid, don't call this on font...");
}

void Font::findMaxBearing(float scalar)
{
	m_fontHeight = scalar;

	// find max character height to determine texture stretch factor

	m_maxBearing = 0;
	int maxDown = 0;
	for(int i = CHAR_START; i <= CHAR_END; i++)
	{
		if(FT_Load_Char(m_face,i,FT_LOAD_RENDER))
			continue;

		// get Metrics
		m_metrics[i] = m_face->glyph->metrics;
		m_metrics[i].horiAdvance = m_face->glyph->advance.x;
		m_maxBearing = std::max(m_maxBearing, int(m_metrics[i].horiBearingY));
		maxDown = std::max(maxDown, int(m_metrics[i].height - m_metrics[i].horiBearingY));
	}

	m_metrics[0x1f] = m_metrics['|'];

	m_maxHeight = m_maxBearing + maxDown;
	m_scaleFactor = scalar / m_maxHeight;
}

void Font::loadTextures()
{
	memset(m_textureArray, 0, sizeof(m_textureArray));
	glGenTextures(CHAR_END - CHAR_START, m_textureArray);

	// glyph : info for out character
	auto g = m_face->glyph;

	for(int i = CHAR_START; i <= CHAR_END; i++)
	{
		int index = i - CHAR_START;

		if (FT_Load_Char(m_face, i, FT_LOAD_RENDER))
		{
			// could not load char
			m_textureArray[index] = 0;
			continue;
		}

		glBindTexture(GL_TEXTURE_2D, m_textureArray[index]);

		// We require 1 byte alignment when uploading texture data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Clamping to edges is important to prevent artifacts when scaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Linear filtering usually looks best for text 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
	}
}
