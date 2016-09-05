/*
 * Der Code dieser Klasse bassiert teilweise auf Ideen von
 * http://learnopengl.com/#!Advanced-OpenGL/Cubemaps
 * die an unsere Umgebung angepasst sind.
 */

#include "Skybox.h"

/**
 * Constructor with texturename arrays and shader names
 *
 * @brief Skybox::Skybox
 * @param textures
 * @param vertexShader
 * @param fragmentShader
 */

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

/**
 * Constructor with texturename arrays, shader names and the cubesize
 *
 * @brief Skybox::Skybox
 * @param textures
 * @param vertexShader
 * @param fragmentShader
 * @param size                      halfe farplane distance.
 */

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
 * Function to load the skybox textures into a cubemap texture and store it.
 *
 * @brief Skybox::loadSkybox
 * @return bool if skybox is succesfully created
 */

bool Skybox::loadSkybox()
{
    //image information
    unsigned int width, height;
    unsigned char* data;

    //Bind a cubemap texture
    glGenTextures(1, &m_TextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    //Read the right images to the right texture positions
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

    //Bind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    delete[] data;

    //load the needed shader
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
    //Load the shader and continue onj success
    if(!sp.load(VertexShader, FragmentShader)){
        cout << "V or F" << endl;
        exit(2);
    }

    //Compile the shader, if there is an error print it
    string* ErrorMsg = new string();
    if(!sp.compile(ErrorMsg)){
        cout << *ErrorMsg << endl;
        exit(3);
    }
}


/**
 * Create vertices for the skybox and bind the buffer
 *
 * @brief Skybox::createVertices
 */
void Skybox::createVertices()
{

    //Setup skybox vertices
    GLfloat vertices[] = {
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


    //Generate buffer and vertex array pointer and fill them with information
    glGenBuffers(1, &skyboxVertexBuffer);
    glGenVertexArrays(1, &skyboxArrayBuffer);

    glBindVertexArray(skyboxArrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 6 * 3, &vertices, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (char *)NULL + 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/**
 * Bind texture for the skymap
 *
 * @brief Skybox::apply
 */

void Skybox::apply() const
{
    if(m_TextureID==0)
        return;

    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
}


/**
 * Function to draw the skybox with a shader
 *
 * @brief Skybox::draw
 */

void Skybox::draw()
{

    //Recreate viewmatrix from camera and remove the translation part
    Matrix viewMatrix;
    viewMatrix.lookAt(g_Camera.getTarget(), g_Camera.getUp(), g_Camera.getPosition());

    //Setup shader
    sp.activate();
    GLuint textureID = sp.getParameterID("skybox");
    GLuint vmId = sp.getParameterID("viewmatrix");
    sp.setParameter(vmId, viewMatrix);
    sp.setParameter(textureID, 0);

    //Bind arraybuffer
    glBindVertexArray(skyboxArrayBuffer);

    //Setup Texture for Shader
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    apply();

    //Draw Skybox
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //Disable
    glDisable(GL_TEXTURE_CUBE_MAP);
    glBindVertexArray(0);
    sp.deactivate();
}
