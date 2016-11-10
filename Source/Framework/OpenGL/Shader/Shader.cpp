#include "Shader.h"
#include <fstream>
#include <assert.h>

#include "../../../System/Log.h"
#include "../../../Utility/Tools.h"
#include "../../../System/Exceptions/ExceptionMissingFile.h"

static const char* EXTENSION_VERTEX = ".vert";
static const char* EXTENSION_FRAGMENT = ".frag";

Shader::Shader()
{
	memset(m_shaders, 0, sizeof(m_shaders));
}

void Shader::create()
{
	assert(m_program == 0);
	m_program = glCreateProgram();

	m_shaders[size_t(Type::Vertex)] = createShader(Type::Vertex, GL_VERTEX_SHADER);
	m_shaders[size_t(Type::Fragment)] = createShader(Type::Fragment, GL_FRAGMENT_SHADER);

	for (size_t i = 0; i < size_t(Type::Size); i++)
	{
		if (m_shaders[i])
		{
			glAttachShader(m_program, m_shaders[i]);
		}
	}

	glLinkProgram(m_program);
	checkShaderError(m_program, GL_LINK_STATUS, true, "programm linking failed");

	glValidateProgram(m_program);
	checkShaderError(m_program, GL_VALIDATE_STATUS, true, "programm validation failed");

	glUseProgram(m_program);
	loadUniforms();
	glUseProgram(0);
}

void Shader::dispose()
{
	if (m_program)
	{
		for (size_t i = 0; i < size_t(Type::Size); i++)
		{
			if (m_shaders[i])
			{
				glDetachShader(m_program, m_shaders[i]);
				glDeleteShader(m_shaders[i]);
				m_shaders[i] = 0;
			}
		}

		glDeleteProgram(m_program);
		m_program = 0;
	}
}

void Shader::bind() const
{
	assert(m_isLoaded);
	assert(m_program);
	glUseProgram(m_program);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::load(const std::string& file)
{
	assert(!m_isLoaded);
	m_filename = file;

	Log::info("Shader::load " + file);
	loadShaderSource(file);

	m_isLoaded = true;
}

std::string Shader::loadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	std::string path; // (data/Shader/) filepath only (for include directives)
	std::string cleanName; // (myShader.vert) filename only 
	tool::splitFilenameToPathFile(&path, &cleanName, fileName);

	if (file.is_open())
	{
		size_t lineNumber = 0;
		while (file.good())
		{
			getline(file, line);
			lineNumber++;

			// check line for #include
			size_t includeStart = line.find("#include");
			size_t commentStart = std::string::npos;
			if (includeStart != std::string::npos) // commented out?
				commentStart = line.find("//");

			if(includeStart != std::string::npos && includeStart < commentStart)
			{
				// we have an include
				size_t parStart = line.find("\"", includeStart + strlen("#include"));
				if (parStart == std::string::npos)
					throw Exception("Shader::loadShader missing 1. \" for include in file " + fileName);

				size_t parEnd = line.find('\"', parStart + 1);
				if (parEnd == std::string::npos)
					throw Exception("Shader::loadShader missing 2. \" for include in file " + fileName);

				std::string includeFile = line.substr(parStart + 1, parEnd - parStart - 1);
				includeFile = path + includeFile;

				// parse this file
				output += "#line 1 \n";
				output += loadShader(includeFile);
				output += "\n#line " + std::to_string(lineNumber + 1) + "\n";
			}
			else
			{
				// no include -> parse normal
				output.append(line + "\n");
			}
		}
	}
	else
	{
		throw ExceptionMissingFile("Shader::loadShader", fileName);
	}

	return output;
}

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		throw Exception(std::string("Shader::checkShaderError") + (errorMessage + std::string(":\n") + error).c_str());
	}
}

GLuint Shader::createShader(Type ty, GLenum shadertype)
{
	const std::string& text = m_sources[size_t(ty)];

	auto shader = glCreateShader(shadertype);

	if (shader == 0)
		throw Exception("Shader::createShader creation failed");

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringsLength[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringsLength[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringsLength);
	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, false, "shader compiliation failed: " + m_filename);

	return shader;
}

void Shader::loadShaderSource(const std::string & filename)
{
	m_sources[size_t(Type::Vertex)] = loadShader(filename + EXTENSION_VERTEX);
	m_sources[size_t(Type::Fragment)] = loadShader(filename + EXTENSION_FRAGMENT);
}

GLint Shader::locateUniform(const char* name) const
{
	GLint x = glGetUniformLocation(m_program, name);
	if (x == -1)
		throw Exception("Shader::locateUniform for " + m_filename +
			" failed: " + std::string(name) + " uniform not found");
	return x;
}

