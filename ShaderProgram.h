#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Color.h"
#include "Vector.h"
#include "Matrix.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#ifdef WIN32
    #include <windows.h>
    #include <GL/glew.h>
    #include <GL/GLUT.h>
    #include <GL/GL.h>
#elif __APPLE__
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

using namespace std;

class ShaderProgram{
	public:
		ShaderProgram();
		~ShaderProgram();
		bool load(const char* VertexShader, const char* FragmentShader);
		bool loadVertexShader(const char* VertexShader);
		bool loadFragmentShader(const char* FragmentShader);
		bool compile(string* CompileErrors=nullptr);

		GLint getParameterID(const char* ParameterName) const;

		void setParameter( GLint ID, float param) const;
        void setParameter( GLint ID, int param) const;
        void setParameter( GLint ID, const Vector& param)const;
		void setParameter( GLint ID, const Color& param) const;
        void setParameter( GLint ID, const Matrix& param)const;

		void activate() const;
		void deactivate() const;
// Custom functionss
        bool compileShader(GLuint shader, string *errorMessage);
		bool hasLinked(GLuint program);
        GLuint getId() const;

	private:
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
		GLuint m_ShaderProgram;

		bool vLoaded = false;
		bool fLoaded = false;
};

#endif
