#include "Shader.h"
#include "consts.h"

Shader::Shader(map<unsigned int, const GLchar*> shaderNames)
{
	setlocale(LC_ALL, "Russian");
	this->Program = glCreateProgram();
	GLint success;
	GLchar infoLog[512];

	for (auto elem : shaderNames)
	{
		const GLchar * GLCharShaderCode,
			 * shaderPath = elem.second;
		string stringShaderCode;
		ifstream shaderFile;
		GLuint shader;

		shaderFile.exceptions(ifstream::badbit);
		
		try
		{
			shaderFile.open(shaderPath);
			stringstream shaderStream; //!!
			shaderStream << shaderFile.rdbuf(); //!!
			shaderFile.close();
			stringShaderCode = shaderStream.str();
		}
		catch (ifstream::failure e)
		{
			cout << "ќшибка обработки файла с кодом шейдера" << endl;
		}

		GLCharShaderCode = stringShaderCode.c_str();
		
		switch (elem.first)
		{
			case (VERTEX_SHADER):
			{
				shader = glCreateShader(GL_VERTEX_SHADER);
				break;
			}
			case (TCS_SHADER):
			{
				shader = glCreateShader(GL_TESS_CONTROL_SHADER);
				break;
			}
			case (TES_SHADER):
			{
				shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
				break;
			}
			case (GEOMETRY_SHADER):
			{
				shader = glCreateShader(GL_GEOMETRY_SHADER);
				break;
			}
			case (FRAGMENT_SHADER):
			{
				shader = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			}
		}

		glShaderSource(shader, 1, &GLCharShaderCode, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			cout << "ќшибка при компил€ции шейдера: \n" << infoLog << endl;
		}
		glAttachShader(this->Program, shader);

		glDeleteShader(shader);
	}

	//шейдерна€ программа:
	glLinkProgram(this->Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		cout << "ќшибка компил€ции шейдерной программы: \n" << infoLog << endl;
	}
}


void Shader::Use()
{
	glUseProgram(this->Program);
}

