#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(){


	 /*GLenum error = glGetError(); 
	 if( error != GL_NO_ERROR ) { 
	 	printf( "Error binding shader! %s\n", gluErrorString( error ) ); 
	 }*/
}

ShaderProgram::~ShaderProgram(){
}

bool ShaderProgram::load(const char* VertexShader, const char* FragmentShader){

    GLenum error = glGetError();

	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
    error = glGetError();
         if( error != GL_NO_ERROR ) {
            printf( "Error binding shader! %s\n", gluErrorString( error ) );
         }

	if(loadVertexShader(VertexShader))
		vLoaded = true;

	if(loadFragmentShader(FragmentShader))
		fLoaded = true;

	if(!vLoaded && !fLoaded)
		return false;

	return true;
}

bool ShaderProgram::loadVertexShader(const char* VertexShader){
	ifstream file;
    string str;
	
	file.open(VertexShader);

    if(!file.is_open()){
    	cout << "Could not open file: " << VertexShader << endl;
    	return false;
    }

	file.seekg(0, std::ios::end);   
	str.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	str.assign((istreambuf_iterator<char>(file)),
	            istreambuf_iterator<char>());

    const char *c_str = str.c_str();
    int length = str.length();

    //cout << "File: " << c_str << endl;
    //cout << "Lenght: " << length << endl;

    glShaderSource(m_VertexShader, 1, &c_str, &length);
    file.close();
    return true;
}

bool ShaderProgram::loadFragmentShader(const char* FragmentShader){
	ifstream file;
    string str;
	
	file.open(FragmentShader);

    if(!file.is_open()){
    	cout << "Could not open file: " << FragmentShader << endl;
    	return false;
    }

	file.seekg(0, std::ios::end);   
	str.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	str.assign((istreambuf_iterator<char>(file)),
	            istreambuf_iterator<char>());

    const char *c_str = str.c_str();
    int length = str.length();

    //cout << "File: " << c_str << endl;
    //cout << "Lenght: " << length << endl;

    glShaderSource(m_FragmentShader, 1, &c_str, &length);
    file.close();
    return true;
}

bool ShaderProgram::compile(string* CompileErrors){
	cout << "Compile Shaders" << endl;
	
    if(vLoaded && !compileShader(m_VertexShader, CompileErrors))
    {
        return false;
    }
//    cout << "VertexShader compiled successful" << endl;

    if(fLoaded && !compileShader(m_FragmentShader, CompileErrors))
    {
        return false;
    }
//    cout << "FragmentShader compiled successful" << endl;

	m_ShaderProgram = glCreateProgram();
	
	if(vLoaded)
		glAttachShader(m_ShaderProgram, m_VertexShader);

	if(fLoaded)
		glAttachShader(m_ShaderProgram, m_FragmentShader);

	glLinkProgram(m_ShaderProgram);

	if(hasLinked(m_ShaderProgram))
//		cout << "ShaderProgram successful linked" << endl;

	if(vLoaded)
		glDeleteShader(m_VertexShader);	
	

	if(fLoaded)
		glDeleteShader(m_FragmentShader);

	return true;
}

bool ShaderProgram::compileShader(GLuint shader, string* errorMessage){

	if(shader == 0){
		cout << "Shader ID is 0" << endl;
		return false;
	}

	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

//		cout << "Error compile shader: " << &errorLog[0] << endl;
		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
        if(errorMessage != nullptr){
            errorMessage->append(&errorLog[0]);
        }
		glDeleteShader(shader); // Don't leak the shader.
		return false;
	}

	return true;
}

bool ShaderProgram::hasLinked(GLuint program){

	if(program == 0){
		cout << "Program ID is 0" << endl;
		return false;
	}

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		
		cout << "Error link program: " << &infoLog[0] << endl;
		//The program is useless now. So delete it.
		glDeleteProgram(program);

		//Provide the infolog in whatever manner you deem best.
		//Exit with failure.
		return false;
	}

    return true;
}

GLuint ShaderProgram::getId() const
{
    return m_ShaderProgram;
}

GLint ShaderProgram::getParameterID(const char* ParameterName) const{
	return glGetUniformLocation(m_ShaderProgram, ParameterName);
}


void ShaderProgram::setParameter( GLint ID, float param) const{
	glUniform1f(ID, param);
}

void ShaderProgram::setParameter( GLint ID, int param) const{
	glUniform1i(ID, param);
}

void ShaderProgram::setParameter( GLint ID, const Vector& param) const{
    glUniform3f(ID, param.X, param.Y, param.Z);
}

void ShaderProgram::setParameter( GLint ID, const Color& param) const{
    glUniform3f(ID, param.R, param.G, param.B);
}

void ShaderProgram::setParameter( GLint ID, const Matrix& param) const{
	Vector right = param.right();
	Vector up = param.up();
	Vector forward = param.forward();
	Vector position = param.translation();

	float values[16] = {
		right.X,up.X,forward.X,position.X,
		right.Y,up.Y,forward.Y,position.Y,
		right.Z,up.Z,forward.Z,position.Z,
		0,0,0,1
	};
	glUniformMatrix4fv(ID, 1, true, values);
}


void ShaderProgram::activate() const{
	glUseProgram(m_ShaderProgram);
}

void ShaderProgram::deactivate() const{
	glUseProgram(0);  // Ausschalten des Shaders
  
}
