#pragma once
#include <string>

class TextFileAppender
{
public:
	TextFileAppender();
	TextFileAppender(const std::string& filename);
	~TextFileAppender();

	TextFileAppender(const TextFileAppender&) = delete;
	TextFileAppender(TextFileAppender&& m);
	TextFileAppender& operator=(const TextFileAppender&) = delete;
	TextFileAppender& operator=(TextFileAppender&& m);
	
	void flush();
	void close();
	bool isOpen() const;
	void write(const std::string& text);
	void writeLine(const std::string& text);

private:
	void swap(TextFileAppender& o);

private:
	FILE* m_pFile = nullptr;
};

