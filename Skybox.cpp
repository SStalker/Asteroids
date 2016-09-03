#include "Skybox.h"

Skybox::Skybox()
{

}

Skybox::Skybox(vector<string> textures , const char* vertexShader, const char* fragmentShader)
{
    VertexShader = vertexShader;
    FragmentShader = fragmentShader;

    for(string tex: textures){
        Textures.push_back(tex);
    }

    createVertices();
    loadSkybox();
}

Skybox::Skybox(vector<string> textures, const char *vertexShader, const char *fragmentShader, float size)
{
    VertexShader = vertexShader;
    FragmentShader = fragmentShader;

    for(string tex: textures){
        Textures.push_back(tex);
    }

    skyboxSize = size;

    createVertices();
    loadSkybox();
}

/**
 * Idee von http://learnopengl.com/#!Advanced-OpenGL/Cubemaps an unsere Umgebung angepasst
 *
 * Läd die texturen für eine Cubemap und erstellt diese im speicher.
 *
 * @brief Skybox::loadSkybox
 * @return bool if skybox is succesfully created
 */

bool Skybox::loadSkybox()
{
    unsigned int width, height;
    unsigned char* data;

    glGenTextures(1, &m_TextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    for(unsigned int i = 0; i < Textures.size(); i++){
        data = LoadBMP(Textures[i].c_str(), width, height);

        if(data==NULL)
            exit(6);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    delete[] data;

    loadShader();
    return true;
}

/**
 * Funktion zum laden der Shader mithilfe der ShaderProgram Klasse
 *
 * @brief Skybox::loadShader
 * @param VertexShader
 * @param FragmentShader
 */

void Skybox::loadShader()
{
    if(!sp.load(VertexShader, FragmentShader)){
        cout << "V or F" << endl;
        exit(2);
    }

    string* ErrorMsg = new string();

    if(!sp.compile(ErrorMsg)){
        cout << *ErrorMsg << endl;
        exit(3);
    }
}

void Skybox::createVertices()
{
    float vertices[] = {
            -skyboxSize,  skyboxSize, -skyboxSize,
            -skyboxSize, -skyboxSize, -skyboxSize,
             skyboxSize, -skyboxSize, -skyboxSize,
             skyboxSize, -skyboxSize, -skyboxSize,
             skyboxSize,  skyboxSize, -skyboxSize,
            -skyboxSize,  skyboxSize, -skyboxSize,

            -skyboxSize, -skyboxSize,  skyboxSize,
            -skyboxSize, -skyboxSize, -skyboxSize,
            -skyboxSize,  skyboxSize, -skyboxSize,
            -skyboxSize,  skyboxSize, -skyboxSize,
            -skyboxSize,  skyboxSize,  skyboxSize,
            -skyboxSize, -skyboxSize,  skyboxSize,

             skyboxSize, -skyboxSize, -skyboxSize,
             skyboxSize, -skyboxSize,  skyboxSize,
             skyboxSize,  skyboxSize,  skyboxSize,
             skyboxSize,  skyboxSize,  skyboxSize,
             skyboxSize,  skyboxSize, -skyboxSize,
             skyboxSize, -skyboxSize, -skyboxSize,

            -skyboxSize, -skyboxSize,  skyboxSize,
            -skyboxSize,  skyboxSize,  skyboxSize,
             skyboxSize,  skyboxSize,  skyboxSize,
             skyboxSize,  skyboxSize,  skyboxSize,
             skyboxSize, -skyboxSize,  skyboxSize,
            -skyboxSize, -skyboxSize,  skyboxSize,

            -skyboxSize,  skyboxSize, -skyboxSize,
             skyboxSize,  skyboxSize, -skyboxSize,
             skyboxSize,  skyboxSize,  skyboxSize,
             skyboxSize,  skyboxSize,  skyboxSize,
            -skyboxSize,  skyboxSize,  skyboxSize,
            -skyboxSize,  skyboxSize, -skyboxSize,

            -skyboxSize, -skyboxSize, -skyboxSize,
            -skyboxSize, -skyboxSize,  skyboxSize,
             skyboxSize, -skyboxSize, -skyboxSize,
             skyboxSize, -skyboxSize, -skyboxSize,
            -skyboxSize, -skyboxSize,  skyboxSize,
             skyboxSize, -skyboxSize,  skyboxSize
        };

    glGenBuffers(1, &skyboxVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 36, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::apply() const
{
    if(m_TextureID==0)
        return;

    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
}

/**
 * ToDo: Überarbeiten
 *
 * @brief Skybox::draw
 */

void Skybox::draw()
{
    sp.activate();


    glDepthMask(GL_FALSE);

    //Setup Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexBuffer);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 36, 0);

    //Setup Texture for Shader
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);

    //Setup shader
    GLuint textureID = sp.getParameterID("skybox");
    sp.setParameter(textureID, 0);
    apply();
    CheckGLErrorsSkybox();

    //?????
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisable(GL_TEXTURE_CUBE_MAP);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDepthMask(GL_TRUE);

    sp.deactivate();
}



void CheckGLErrorsSkybox()
{
    GLenum Error = glGetError();
    if(Error !=0)
    {
        std::cout << "Skybox: " << gluErrorString(Error) << std::endl;
        throw std::exception();
    }
}
