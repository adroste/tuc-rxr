#include "TextFileAppender.h"
#include <assert.h>


TextFileAppender::TextFileAppender()
{
	m_pFile = nullptr;
}

TextFileAppender::TextFileAppender(const std::string& filename)
{
	m_pFile = fopen(filename.c_str(), "a");
}


TextFileAppender::~TextFileAppender()
{
	close();
}

TextFileAppender::TextFileAppender(TextFileAppender&& m)
{
	swap(m);
}

TextFileAppender& TextFileAppender::operator=(TextFileAppender&& m)
{
	swap(m);
	return *this;
}

void TextFileAppender::close()
{
	if (m_pFile)
		fclose(m_pFile);
	m_pFile = nullptr;
}

bool TextFileAppender::isOpen() const
{
	return m_pFile != nullptr;
}

void TextFileAppender::write(const std::string& text)
{
	assert(m_pFile);
	fputs(text.c_str(), m_pFile);
}

void TextFileAppender::writeLine(const std::string& text)
{
	write(text);
	write("\n");
}

void TextFileAppender::swap(TextFileAppender& o)
{
	std::swap(m_pFile, o.m_pFile);
}
